#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::AbstractLightEntity

    Public interface to InternalGraphics::InternalAbstractLightEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "lighting/lighttype.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class AbstractLightEntity : public GraphicsEntity
{
    __DeclareClass(AbstractLightEntity);
public:
    /// constructor
    AbstractLightEntity();
    /// destructor
    virtual ~AbstractLightEntity();

    /// get the light type (defined by subclass)
    Lighting::LightType::Code GetLightType() const;
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
    
protected:
    /// set the light type (must be called from sub-classes constructor
    void SetLightType(Lighting::LightType::Code c);

    Lighting::LightType::Code lightType;
    Math::float4 color;
    Math::float4 projMapUvOffsetAndScale;
    bool castShadows;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AbstractLightEntity::SetLightType(Lighting::LightType::Code t)
{
    this->lightType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Lighting::LightType::Code
AbstractLightEntity::GetLightType() const
{
    return this->lightType;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
AbstractLightEntity::GetColor() const
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AbstractLightEntity::GetCastShadows() const
{
    return this->castShadows;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AbstractLightEntity::SetProjMapUvOffsetAndScale(const Math::float4& v)
{
    this->projMapUvOffsetAndScale = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
AbstractLightEntity::GetProjMapUvOffsetAndScale() const
{
    return this->projMapUvOffsetAndScale;
}

} // namespace Graphics
//------------------------------------------------------------------------------

    