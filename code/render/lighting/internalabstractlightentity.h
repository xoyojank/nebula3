#pragma once
//------------------------------------------------------------------------------
/**
    @class Lighting::InternalAbstractLightEntity
    
    Base class for light sources. Light sources do not directly influence
    the render pipeline (like manipulating shader variables, etc...). This
    will be handled by the LightServer and ShadowServer singletons which
    may implement platform specific lighting models.
    
    (C) 2007 Radon Labs GmbH
*/
#include "internalgraphics/internalgraphicsentity.h"
#include "lighting/lighttype.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class InternalAbstractLightEntity : public InternalGraphics::InternalGraphicsEntity
{
    __DeclareClass(InternalAbstractLightEntity);
public:
    /// constructor
    InternalAbstractLightEntity();
    /// destructor
    virtual ~InternalAbstractLightEntity();

    /// get the light type
    LightType::Code GetLightType() const;
    /// set primary light color
    void SetColor(const Math::float4& c);
    /// get primary light color
    const Math::float4& GetColor() const;
    /// enable/disable shadow casting
    void SetCastShadows(bool b);
    /// get shadow casting flag
    bool GetCastShadows() const;   
    /// set projection map UV offset and scale (xy->offset, zw->scale)
    void SetProjMapUvOffsetAndScale(const Math::float4& v);
    /// get projection map UV offset and scale
    const Math::float4& GetProjMapUvOffsetAndScale() const;

    /// get inverse transform (transforms from world to light space)
    const Math::matrix44& GetInvTransform() const;
    /// get light-projection matrix (transforms from light space to light projection space)
    const Math::matrix44& GetProjTransform() const;
    /// get world-to-light-projection transform (transform from world to light projection space)
    const Math::matrix44& GetInvLightProjTransform() const;

    /// set shadow buffer uv rectangle (optionally set by light/shadow servers)
    void SetShadowBufferUvOffsetAndScale(const Math::float4& uvOffset);
    /// get shadow buffer uv rectangle
    const Math::float4& GetShadowBufferUvOffsetAndScale() const;   

    /// get cast shadows this frame
    bool GetCastShadowsThisFrame() const;
    /// set CastShadowsThisFrame
    void SetCastShadowsThisFrame(bool val); 
    /// set shadow transform
    void SetShadowTransform(const Math::matrix44& val);
    /// get shadow transform
    const Math::matrix44& GetShadowInvTransform();  
    /// get shadow projection transform
    const Math::matrix44& GetShadowInvLightProjTransform();  
    /// get shadow projection transform
    const Math::matrix44& GetShadowProjTransform();      
    /// get ShadowIntensity	
    float GetShadowIntensity() const;
    /// set ShadowIntensity
    void SetShadowIntensity(float val);
   
    /// handle a message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// called from internalview
    virtual void OnResolveVisibility();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// set the light type (must be called from sub-classes constructor
    void SetLightType(LightType::Code c);    
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    /// update shadow transforms
    void UpdateShadowTransforms();

    LightType::Code lightType;
    Math::matrix44 invTransform;
    Math::matrix44 projTransform;
    Math::matrix44 invLightProjTransform;        
    Math::float4 color;
    Math::float4 projMapUvOffsetAndScale;
    Math::float4 shadowBufferUvOffsetAndScale;
    bool castShadows;
    bool castShadowsThisFrame; 
    float shadowIntensity;
    Math::matrix44 shadowTransform;
    Math::matrix44 shadowInvTransform;   
    Math::matrix44 shadowInvLightProjTransform;   
    Math::matrix44 shadowProjTransform;
    bool shadowTransformsDirty;    
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetLightType(LightType::Code c)
{
    this->lightType = c;
}

//------------------------------------------------------------------------------
/**
*/
inline LightType::Code
InternalAbstractLightEntity::GetLightType() const
{
    return this->lightType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetColor(const Math::float4& c)
{
    this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetColor() const
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetProjMapUvOffsetAndScale(const Math::float4& v)
{
    this->projMapUvOffsetAndScale = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetProjMapUvOffsetAndScale() const
{
    return this->projMapUvOffsetAndScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetShadowBufferUvOffsetAndScale(const Math::float4& v)
{
    this->shadowBufferUvOffsetAndScale = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalAbstractLightEntity::GetShadowBufferUvOffsetAndScale() const
{
    return this->shadowBufferUvOffsetAndScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalAbstractLightEntity::SetCastShadows(bool b)
{
    this->castShadows = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalAbstractLightEntity::GetCastShadows() const
{
    return this->castShadows;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetInvTransform() const
{
    return this->invTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetProjTransform() const
{
    return this->projTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalAbstractLightEntity::GetInvLightProjTransform() const
{
    return this->invLightProjTransform;
}       

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalAbstractLightEntity::GetCastShadowsThisFrame() const
{
    return this->castShadowsThisFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalAbstractLightEntity::SetCastShadowsThisFrame(bool val)
{
    this->castShadowsThisFrame = val;
}   

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalAbstractLightEntity::SetShadowTransform(const Math::matrix44& val)
{
    this->shadowTransform = val;
    this->shadowTransformsDirty = true;
}   

//------------------------------------------------------------------------------
/**
*/
inline float 
InternalAbstractLightEntity::GetShadowIntensity() const
{
    return this->shadowIntensity;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalAbstractLightEntity::SetShadowIntensity(float val)
{
    this->shadowIntensity = val;
}

} // namespace Lighting
//------------------------------------------------------------------------------

    