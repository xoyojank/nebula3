#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::InternalSpotLightEntity
    
    Implements a local spot light. The spot light's cone is computed
    from the transformation matrix of the light entity. The spot light
    cone will point along -z, and the cone's angle will be defined
    by the length of x and y component at the end of the z component.
    
    (C) 2007 Radon Labs GmbH
*/
#include "lighting/internalabstractlightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class InternalSpotLightEntity : public InternalAbstractLightEntity
{
    __DeclareClass(InternalSpotLightEntity);
public:
    /// constructor
    InternalSpotLightEntity();
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);
    /// handle a message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
};

} // namespace Lighting
//------------------------------------------------------------------------------
