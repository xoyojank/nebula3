//------------------------------------------------------------------------------
//  properties/chasecameraproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/chasecameraproperty.h"
#include "basegametiming/inputtimesource.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "graphics/stage.h"
#include "graphics/cameraentity.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "math/quaternion.h"
#include "io/console.h"

#if __USE_PHYSICS__
#include "properties/physicsproperty.h"
#include "physics/physicsserver.h"
#include "physics/filterset.h"
#include "physics/physicsutil.h"
#endif

using namespace Math;

// chase camera specific attributes
namespace Attr
{
    DefineFloatWithDefault(CameraDistance, 'CADI', ReadWrite, 1000.0f);
    DefineFloatWithDefault(CameraMinDistance, 'CMID', ReadWrite, 500.0f);
    DefineFloatWithDefault(CameraMaxDistance, 'CMAD', ReadWrite, 1200.0f);
    DefineFloatWithDefault(CameraDistanceStep, 'CDST', ReadWrite, 100.0f);
    DefineFloat4WithDefault(CameraOffset, 'COFF', ReadWrite, Math::float4(0.0f, 500.0f, 0.0f,0.0f));
    DefineFloatWithDefault(CameraLowStop, 'CLOS', ReadWrite, -90.0f);
    DefineFloatWithDefault(CameraHighStop, 'CHIS', ReadWrite, 90.0f);
    DefineFloatWithDefault(CameraAngularVelocity, 'CAVE', ReadWrite, 50.0f);
    DefineFloatWithDefault(CameraLinearGain, 'CLIG', ReadWrite, -30.0f);
    DefineFloatWithDefault(CameraAngularGain, 'CANG', ReadWrite, -50.0f);
    DefineFloatWithDefault(CameraDefaultTheta, 'CTHE', ReadWrite, n_deg2rad(-45.0f));    
    DefineFloatWithDefault(CameraDefaultRho, 'CRHO', ReadWrite, n_deg2rad(45.0f));  
};

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::ChaseCameraProperty, 'PCHC', GraphicsFeature::CameraProperty);

using namespace Game;
using namespace Messaging;
using namespace BaseGameFeature;
using namespace Math;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
ChaseCameraProperty::ChaseCameraProperty() :
    cameraDistance(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ChaseCameraProperty::~ChaseCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property.
*/
void
ChaseCameraProperty::SetupDefaultAttributes()
{
    CameraProperty::SetupDefaultAttributes();

    SetupAttr(Attr::CameraDistance);
    SetupAttr(Attr::CameraMinDistance);
    SetupAttr(Attr::CameraMaxDistance);
    SetupAttr(Attr::CameraOffset);
    SetupAttr(Attr::CameraLowStop);
    SetupAttr(Attr::CameraHighStop);
    SetupAttr(Attr::CameraAngularVelocity);
    SetupAttr(Attr::CameraDistanceStep);
    SetupAttr(Attr::CameraLinearGain);
    SetupAttr(Attr::CameraAngularGain);
    SetupAttr(Attr::CameraDefaultRho);
    SetupAttr(Attr::CameraDefaultTheta);
}

//------------------------------------------------------------------------------
/**
    This method is called once when the property is attached to the entity.
*/
void
ChaseCameraProperty::OnActivate()
{
    CameraProperty::OnActivate();

    // setup the collide exclude set
#if __USE_PHYSICS__
    this->collideExcludeSet.Clear();
    this->collideExcludeSet.AddMaterialType(Physics::MaterialTable::StringToMaterialType("Probe"));
    this->collideExcludeSet.AddMaterialType(Physics::MaterialTable::StringToMaterialType("Character"));
#endif

    // reset the camera
    this->HandleCameraReset();
}

//------------------------------------------------------------------------------
/**
    This method is called when the entity obtains the camera focus.
*/
void
ChaseCameraProperty::OnObtainCameraFocus()
{
    // initialize the feedback loops with the current camera values so
    // that we get a smooth interpolation to the new position    
    const matrix44 m = this->entity->GetMatrix44(Attr::Transform);
    Timing::Time time = InputTimeSource::Instance()->GetTime();
    this->cameraPos.Reset(time, 0.0001f, this->entity->GetFloat(Attr::CameraLinearGain), m.get_position()- (m.get_zaxis() * 5.0f));
    this->cameraLookat.Reset(time, 0.0001f, this->entity->GetFloat(Attr::CameraAngularGain), m.get_position() - (m.get_zaxis() * 10.0f));
    this->UpdateCamera(false);
    CameraProperty::OnObtainCameraFocus();
}

//------------------------------------------------------------------------------
/**
    This method is called before the camera is "rendered".
*/
void
ChaseCameraProperty::OnRender()
{
    // only do something if we have the camera focus
    if (FocusManager::Instance()->GetCameraFocusEntity() == this->entity)
    {
        // update the camera
        this->UpdateCamera(false);
    }

    // important: call parent class
    CameraProperty::OnRender();
}

//------------------------------------------------------------------------------
/**
*/
void
ChaseCameraProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(CameraOrbit::Id);
    this->RegisterMessage(CameraReset::Id);
    this->RegisterMessage(CameraDistance::Id);
    CameraProperty::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
    This method handles pending messages for the property. It is called
    by the OnRender() method.
*/
void
ChaseCameraProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(CameraOrbit::Id))
    {
        CameraOrbit* cameraOrbitMsg = msg.downcast<CameraOrbit>();
        this->HandleCameraOrbit(cameraOrbitMsg->GetHorizontalRotation(), cameraOrbitMsg->GetVerticalRotation());
    }
    else if (msg->CheckId(CameraReset::Id))
    {
        this->HandleCameraReset();
    }
    else if (msg->CheckId(CameraDistance::Id))
    {
        CameraDistance* cameraDistanceMsg = msg.downcast<CameraDistance>();
        this->HandleCameraDistanceChange(cameraDistanceMsg->GetRelativeDistanceChange());
    }
    CameraProperty::HandleMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Handle a camera distance change.
*/
void
ChaseCameraProperty::HandleCameraDistanceChange(float d)
{
    float minDist  = this->entity->GetFloat(Attr::CameraMinDistance);
    float maxDist  = this->entity->GetFloat(Attr::CameraMaxDistance);
    float zoomStep = this->entity->GetFloat(Attr::CameraDistanceStep);
    this->cameraDistance = n_clamp(this->cameraDistance + (d * zoomStep), minDist, maxDist);
}

//------------------------------------------------------------------------------
/**
    Handle a camera reset.
*/
void
ChaseCameraProperty::HandleCameraReset()
{
    float curTheta = this->entity->GetFloat(Attr::CameraDefaultTheta);     
    float curRho = this->entity->GetFloat(Attr::CameraDefaultRho);     
    const matrix44 m = this->entity->GetMatrix44(Attr::Transform);
    this->cameraAngles.set(m.get_zaxis());
    this->cameraAngles.theta = curTheta;
    this->cameraAngles.rho = curRho;
    this->cameraDistance = this->entity->GetFloat(Attr::CameraDistance);
}

//------------------------------------------------------------------------------
/**
    Handle a camera orbit.
*/
void
ChaseCameraProperty::HandleCameraOrbit(float dRho, float dTheta)
{
    float angularVelocity = this->entity->GetFloat(Attr::CameraAngularVelocity);
    float lowStop = n_deg2rad(this->entity->GetFloat(Attr::CameraLowStop));
    float hiStop  = n_deg2rad(this->entity->GetFloat(Attr::CameraHighStop));
    
    float frameTime = (float) InputTimeSource::Instance()->GetFrameTime();;
    this->cameraAngles.rho += dRho * angularVelocity * frameTime;
    this->cameraAngles.theta += -dTheta * angularVelocity * frameTime;
    this->cameraAngles.theta = n_clamp(this->cameraAngles.theta, lowStop, hiStop);
}

//------------------------------------------------------------------------------
/**
    Do a ray check between 'from' and 'to' and return a replacement
    point for 'to'.
*/
point
ChaseCameraProperty::DoCollideCheck(const point& from, const point& to)
{
    static const vector up(0.0f, 1.0f, 0.0f);
    matrix44 m = matrix44::lookatrh(from, to, up);
    float outContactDist = 1.0f;
#if __USE_PHYSICS__    
    Physics::PhysicsUtil::RayBundleCheck(from, to, up, m.get_xaxis(), 0.25f, this->collideExcludeSet, outContactDist);
#endif    
    vector vec = vector::normalize(to - from);    
    point newTo = from + vec * outContactDist;
    return newTo;
}

//------------------------------------------------------------------------------
/**
    Update the camera position and orientation from the current
    orbit polar angles.
*/
void
ChaseCameraProperty::UpdateCamera(bool interpolate)
{
    // compute the lookat point in global space
    matrix44 m44 = this->entity->GetMatrix44(Attr::Transform);    
    const point pos = m44.get_position();
    matrix44 rot = m44;
    rot.set_position(point(0,0,0));
    vector lookatPoint = pos + matrix44::transform(this->entity->GetFloat4(Attr::CameraOffset), rot);    

    matrix44 m = matrix44::translation(0, 0, this->cameraDistance);
    m = matrix44::multiply(m, matrix44::rotationx(this->cameraAngles.theta));
    m = matrix44::multiply(m, matrix44::rotationy(this->cameraAngles.rho));
    float4 newpos = m.get_position();
    newpos += pos;
    m.set_position(newpos);

    // compute the collided goal position        
    point goalPos = m.get_position();
#if __USE_PHYSICS__    
    goalPos = this->DoCollideCheck(lookatPoint, goalPos);
#endif	    
    if (!interpolate)
    {
        Timing::Time time = InputTimeSource::Instance()->GetTime();
        this->cameraPos.Reset(time, 0.0001f, this->entity->GetFloat(Attr::CameraLinearGain), goalPos);
        this->cameraLookat.Reset(time, 0.0001f, this->entity->GetFloat(Attr::CameraAngularGain), lookatPoint);
    }

    // feed and update the feedback loops
    Timing::Time time = InputTimeSource::Instance()->GetTime();
    this->cameraPos.SetGoal(goalPos);
    this->cameraLookat.SetGoal(lookatPoint);
    this->cameraPos.Update(time);
    this->cameraLookat.Update(time);

    // construct the new camera matrix
    matrix44 cameraMatrix = matrix44::lookatrh(this->cameraPos.GetState(), this->cameraLookat.GetState(), vector::upvec());

    // update the graphics subsystem camera
    this->cameraEntity->SetTransform(cameraMatrix);
}

} // namespace GraphicsFeature