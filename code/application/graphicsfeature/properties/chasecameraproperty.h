#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::ChaseCameraProperty

    A chase camera for 3rd person camera control.
    
    (C) 2005 Radon Labs GmbH
*/
#include "properties/cameraproperty.h"
#include "math/polar.h"
#include "math/pfeedbackloop.h"
#include "math/point.h"
#include "appgame/appconfig.h"
#if __USE_PHYSICS__
#include "physics/filterset.h"
#endif
//------------------------------------------------------------------------------
namespace Attr
{
    // chase camera specific attributes
    DeclareFloat(CameraDistance, 'CADI', ReadWrite);
    DeclareFloat(CameraMinDistance, 'CMID', ReadWrite);
    DeclareFloat(CameraMaxDistance, 'CMAD', ReadWrite);
    DeclareFloat(CameraDistanceStep, 'CDST', ReadWrite);
    DeclareFloat4(CameraOffset, 'COFF', ReadWrite);
    DeclareFloat(CameraLowStop, 'CLOS', ReadWrite);
    DeclareFloat(CameraHighStop, 'CHIS', ReadWrite);
    DeclareFloat(CameraAngularVelocity, 'CAVE', ReadWrite);
    DeclareFloat(CameraLinearGain, 'CLIG', ReadWrite);
    DeclareFloat(CameraAngularGain, 'CANG', ReadWrite);
    DeclareFloat(CameraDefaultTheta, 'CTHE', ReadWrite);    
    DeclareFloat(CameraDefaultRho, 'CRHO', ReadWrite);  
    DeclareFloat(CameraTheta, 'CATH', ReadWrite);    
    DeclareFloat(CameraRho, 'CARH', ReadWrite);  
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{

class ChaseCameraProperty : public CameraProperty
{
	__DeclareClass(ChaseCameraProperty);
public:
    /// constructor
    ChaseCameraProperty();
    /// destructor
    virtual ~ChaseCameraProperty();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called when camera focus is obtained
    virtual void OnObtainCameraFocus();
    /// called before camera is "rendered"
    virtual void OnRender();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// do a collision check
    virtual Math::point DoCollideCheck(const Math::point& from, const Math::point& to);
    /// handle a camera reset message
    virtual void HandleCameraReset();
    /// handle a camera orbit message
    virtual void HandleCameraOrbit(float dRho, float dTheta);
    /// handle a camera distance change
    virtual void HandleCameraDistanceChange(float z);
    /// update the camera matrix
    virtual void UpdateCamera(bool interpolate);

    Math::polar cameraAngles;
    float cameraDistance;

#if __USE_PHYSICS__
    Physics::FilterSet collideExcludeSet;
#endif
    Math::PFeedbackLoop<Math::point> cameraPos;
    Math::PFeedbackLoop<Math::point> cameraLookat;
};
__RegisterClass(ChaseCameraProperty);

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
