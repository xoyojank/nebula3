//------------------------------------------------------------------------------
//  internalspotlightentity.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalspotlightentity.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalSpotLightEntity, 'SPLE', Lighting::InternalAbstractLightEntity);

using namespace Math;
using namespace CoreGraphics;
using namespace Threading;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
InternalSpotLightEntity::InternalSpotLightEntity()
{
    this->SetLightType(LightType::Spot);
}

//------------------------------------------------------------------------------
/**
*/
Math::ClipStatus::Type
InternalSpotLightEntity::ComputeClipStatus(const Math::bbox& box)
{
    // compute spot light clip status
    ClipStatus::Type clipStatus = box.clipstatus(this->invLightProjTransform);
    return clipStatus;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalSpotLightEntity::OnTransformChanged()
{
    InternalAbstractLightEntity::OnTransformChanged();

    // compute the spot light's perspective projection matrix from
    // its transform matrix (spot direction is along -z, and goes
    // throught the rectangle formed by the x and y components
    // at the end of -z
    float widthAtFarZ  = this->transform.getrow0().length() * 2.0f;
    float heightAtFarZ = this->transform.getrow1().length() * 2.0f;
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

    // compute the invLightProj matrix which transforms from world space to light space
    this->invLightProjTransform = matrix44::multiply(this->invTransform, this->projTransform);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSpotLightEntity::OnRenderDebug()
{
    // render spot light frustum
    float4 color(this->GetColor() * 0.8f);
    color.w() = 0.25f;

    // remove scaling, its also considered in projection matrix
    matrix44 unscaledTransform = this->GetTransform();
    unscaledTransform.set_xaxis(float4::normalize(unscaledTransform.get_xaxis()));
    unscaledTransform.set_yaxis(float4::normalize(unscaledTransform.get_yaxis()));
    unscaledTransform.set_zaxis(float4::normalize(unscaledTransform.get_zaxis()));
    matrix44 frustum = matrix44::multiply(matrix44::inverse(this->GetProjTransform()), unscaledTransform);
    ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Box, frustum, color));

    // mark position
    color.w() = 1.0f;
    matrix44 centerTransform = matrix44::scaling(0.1f, 0.1f, 0.1f);
    centerTransform.set_position(this->GetTransform().get_position());
    ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Sphere, centerTransform, color));

    InternalAbstractLightEntity::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalSpotLightEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalSpotLightEntity, this, msg);
}

} // namespace Lighting
