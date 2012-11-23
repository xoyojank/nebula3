#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GlobalLightEntity
    
    Client-side proxy of a InternalGraphics::InternalGlobalLightEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/abstractlightentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GlobalLightEntity : public AbstractLightEntity
{
    __DeclareClass(GlobalLightEntity);
public:
    /// constructor
    GlobalLightEntity();
    /// set the backlight color
    void SetBackLightColor(const Math::float4& c);
    /// get the backlight color
    const Math::float4& GetBackLightColor() const;
    /// set AmbientLightColor
    void SetAmbientLightColor(const Math::float4& val);
    /// get AmbientLightColor	
    const Math::float4& GetAmbientLightColor() const;
    /// set BackLightOffset
    void SetBackLightOffset(float val);
    /// get BackLightOffset	
    float GetBackLightOffset() const;
    /// set all params (only sends 1 msg to render thread instead of 4)
    void SetAllParams(const Math::float4& color, const Math::float4& backColor, const Math::float4& ambColor, float backOffset, bool castShadows);

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);

    Math::float4 backLightColor;
    Math::float4 ambientLightColor;
    float backLightOffset;
};

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
GlobalLightEntity::GetBackLightColor() const
{
    return this->backLightColor;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4& 
GlobalLightEntity::GetAmbientLightColor() const
{
    return this->ambientLightColor;
}

//------------------------------------------------------------------------------
/**
*/
inline float
GlobalLightEntity::GetBackLightOffset() const
{
    return this->backLightOffset;
}

} // namespace Graphics
//------------------------------------------------------------------------------

    