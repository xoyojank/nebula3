//------------------------------------------------------------------------------
//  internalpointlightentity.h
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalpointlightentity.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalPointLightEntity, 'IPLE', Lighting::InternalAbstractLightEntity);

using namespace Math;
using namespace Threading;
using namespace CoreGraphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
InternalPointLightEntity::InternalPointLightEntity()
{
    this->SetLightType(LightType::Point);
    // patch bounding box to fully cover point light volume
    this->localBox.set(point::origin(), vector(1.0,1.0,1.0));
}

//------------------------------------------------------------------------------
/**
*/
Math::ClipStatus::Type
InternalPointLightEntity::ComputeClipStatus(const Math::bbox& box)
{
    // compute pointlight clip status
    Math::bbox pointLightBox(this->GetTransform().get_position(), this->GetTransform().get_zaxis());
    ClipStatus::Type clipStatus = box.clipstatus(pointLightBox);
    return clipStatus;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalPointLightEntity::OnTransformChanged()
{
    InternalAbstractLightEntity::OnTransformChanged();

    // compute the spot light's perspective projection matrix from
    // its transform matrix (spot direction is along -z, and goes
    // throught the rectangle formed by the x and y components
    // at the end of -z
    float widthAtFarZ  = this->transform.getrow0().length() * 2.0f;
    float heightAtFarZ = this->transform.getrow1().length() * 2.0f;
    widthAtFarZ *= 2.0f;
    heightAtFarZ *= 2.0f;

    float nearZ = 0.001f; // put the near plane at 0.001cm 
    float farZ = this->transform.getrow2().length();    
    n_assert(farZ > 0.0f);
    if (nearZ >= farZ)
    {
        nearZ = farZ / 2.0f;
    }
    float widthAtNearZ  = (widthAtFarZ / farZ) * nearZ;
    float heightAtNearZ = (heightAtFarZ / farZ) * nearZ;
    this->projTransform = matrix44::persprh(widthAtNearZ, heightAtNearZ, nearZ, farZ);

    this->shadowTransform.set_position(this->shadowTransform.get_position() + this->shadowTransform.get_zaxis() * 0.25f);
    this->shadowTransformsDirty = true;

    // compute the invLightProj matrix which transforms from world space to light space
    this->invLightProjTransform = matrix44::multiply(this->invTransform, this->projTransform);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalPointLightEntity::OnRenderDebug()
{
    float4 color(this->GetColor() * 0.8f);
    color.w() = 0.25f;
    ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Sphere, this->GetTransform(), color));
    // mark position
    color.w() = 1.0f;
    matrix44 centerTransform = matrix44::scaling(0.1f, 0.1f, 0.1f);
    centerTransform.set_position(this->GetTransform().get_position());
    ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Sphere, centerTransform, color));

    // render bounding box
    ShapeRenderer::Instance()->AddWireFrameBox(this->GetGlobalBoundingBox(), color, Thread::GetMyThreadId());

    InternalAbstractLightEntity::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalPointLightEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalPointLightEntity, this, msg);
}

} // namespace Lighting
