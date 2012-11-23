//------------------------------------------------------------------------------
//  internalabstractlightentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/internalabstractlightentity.h" 
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"

namespace Lighting
{
__ImplementClass(Lighting::InternalAbstractLightEntity, 'ALIE', InternalGraphics::InternalGraphicsEntity);

using namespace Math;        
using namespace CoreGraphics;
using namespace Threading;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
InternalAbstractLightEntity::InternalAbstractLightEntity() :
    lightType(LightType::InvalidLightType),
    invTransform(matrix44::identity()),
    projTransform(matrix44::identity()),
    invLightProjTransform(matrix44::identity()),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    shadowBufferUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    castShadows(false),
    castShadowsThisFrame(false),
    shadowIntensity(1.0f)
{
    this->SetType(InternalGraphics::InternalGraphicsEntityType::Light);
}

//------------------------------------------------------------------------------
/**
*/
InternalAbstractLightEntity::~InternalAbstractLightEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the the internalview comes to its Render method. 
    Add light entities to the LightServer or to the ShadowServer.
*/
void
InternalAbstractLightEntity::OnResolveVisibility()
{
    if (this->GetCastShadows())
    {
        // maybe cast shadows
        ShadowServer::Instance()->AttachVisibleLight(this);
    }
    else
    {
        // casts no shadows by default, can go directly into lightserver
        LightServer::Instance()->AttachVisibleLight(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InternalAbstractLightEntity::OnTransformChanged()
{
    InternalGraphicsEntity::OnTransformChanged();

    // update inverse transform
    this->invTransform = matrix44::inverse(this->transform);
    this->shadowTransform = this->transform;

    this->shadowTransformsDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalAbstractLightEntity::OnRenderDebug()
{
    if (this->GetCastShadowsThisFrame())
    {      
        // render shadow frustum
        float4 color(0.5f, 0.5f, 0.5f, 0.2f);

        // remove scaling, its also considered in projection matrix
        matrix44 unscaledTransform = this->shadowTransform;
        unscaledTransform.set_xaxis(float4::normalize(unscaledTransform.get_xaxis()));
        unscaledTransform.set_yaxis(float4::normalize(unscaledTransform.get_yaxis()));
        unscaledTransform.set_zaxis(float4::normalize(unscaledTransform.get_zaxis()));
        matrix44 frustum = matrix44::multiply(matrix44::inverse(this->GetShadowProjTransform()), unscaledTransform);
        ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Box, frustum, color));
                                   
        const point unitCube[24] = {point(1,1,1),  point(1,1,-1),  point(1,1,-1),  point(-1,1,-1),  point(-1,1,-1),  point(-1,1,1),   point(-1,1,1),  point(1,1,1),
                                    point(1,-1,1), point(1,-1,-1), point(1,-1,-1), point(-1,-1,-1), point(-1,-1,-1), point(-1,-1,1),  point(-1,-1,1), point(1,-1,1),
                                    point(1,1,1),  point(1,-1,1),  point(1,1,-1),  point(1,-1,-1),  point(-1,1,-1),  point(-1,-1,-1), point(-1,1,1),  point(-1,-1,1)};
        
        RenderShape shape;
        color.set(0.5f, 0.5f, 0.5f, 0.9f);
        shape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
                            frustum,
                            PrimitiveTopology::LineList,
                            12,
                            unitCube,
                            4,
                            color);
        ShapeRenderer::Instance()->AddShape(shape); 
    }
}

//------------------------------------------------------------------------------
/**
*/
const Math::matrix44& 
InternalAbstractLightEntity::GetShadowInvTransform()
{
    if (this->shadowTransformsDirty)
    {
        this->UpdateShadowTransforms();
    }
    return this->shadowInvTransform;
}

//------------------------------------------------------------------------------
/**
*/
const Math::matrix44& 
InternalAbstractLightEntity::GetShadowProjTransform()
{
    if (this->shadowTransformsDirty)
    {
        this->UpdateShadowTransforms();
    }
    return this->shadowProjTransform;
}   

//------------------------------------------------------------------------------
/**
*/
const Math::matrix44& 
InternalAbstractLightEntity::GetShadowInvLightProjTransform()
{       
    if (this->shadowTransformsDirty)
    {
        this->UpdateShadowTransforms();
    }
    return this->shadowInvLightProjTransform;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalAbstractLightEntity::UpdateShadowTransforms()
{
    this->shadowInvTransform = matrix44::inverse(this->shadowTransform);
    this->shadowInvLightProjTransform = matrix44::multiply(this->shadowInvTransform, this->projTransform);
    // @todo set extra optimal near and far plane
    this->shadowProjTransform = this->GetProjTransform();
    this->shadowTransformsDirty = false;
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalAbstractLightEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalAbstractLightEntity, this, msg);
}

} // namespace Lighting
