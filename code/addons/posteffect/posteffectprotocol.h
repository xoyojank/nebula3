#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "timing/time.h"
#include "posteffect/rt/params/colorparams.h"
#include "posteffect/rt/params/depthoffieldparams.h"
#include "posteffect/rt/params/fogparams.h"
#include "posteffect/rt/params/hdrparams.h"
#include "posteffect/rt/params/lightparams.h"
#include "posteffect/rt/params/skyparams.h"

//------------------------------------------------------------------------------
namespace PostEffect
{
//------------------------------------------------------------------------------
class SetupPostEffectSystem : public Messaging::Message
{
    __DeclareClass(SetupPostEffectSystem);
    __DeclareMsgId;
public:
    SetupPostEffectSystem() 
    { };
};
//------------------------------------------------------------------------------
class ResetPostEffectSystem : public Messaging::Message
{
    __DeclareClass(ResetPostEffectSystem);
    __DeclareMsgId;
public:
    ResetPostEffectSystem() 
    { };
};
//------------------------------------------------------------------------------
class PreloadTexture : public Messaging::Message
{
    __DeclareClass(PreloadTexture);
    __DeclareMsgId;
public:
    PreloadTexture() 
    { };
public:
    void SetResource(const Util::String& val)
    {
        n_assert(!this->handled);
        this->resource = val;
    };
    const Util::String& GetResource() const
    {
        return this->resource;
    };
private:
    Util::String resource;
};
//------------------------------------------------------------------------------
class UnloadTexture : public Messaging::Message
{
    __DeclareClass(UnloadTexture);
    __DeclareMsgId;
public:
    UnloadTexture() 
    { };
public:
    void SetResource(const Util::String& val)
    {
        n_assert(!this->handled);
        this->resource = val;
    };
    const Util::String& GetResource() const
    {
        return this->resource;
    };
private:
    Util::String resource;
};
//------------------------------------------------------------------------------
class BlendBase : public Messaging::Message
{
    __DeclareClass(BlendBase);
    __DeclareMsgId;
public:
    BlendBase() 
    { };
public:
    void SetBlendTime(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->blendtime = val;
    };
    const Timing::Time& GetBlendTime() const
    {
        return this->blendtime;
    };
private:
    Timing::Time blendtime;
};
//------------------------------------------------------------------------------
class FadeOut : public BlendBase
{
    __DeclareClass(FadeOut);
    __DeclareMsgId;
public:
    FadeOut() 
    { };
};
//------------------------------------------------------------------------------
class FadeIn : public BlendBase
{
    __DeclareClass(FadeIn);
    __DeclareMsgId;
public:
    FadeIn() 
    { };
};
//------------------------------------------------------------------------------
class BlendColor : public BlendBase
{
    __DeclareClass(BlendColor);
    __DeclareMsgId;
public:
    BlendColor() 
    { };
public:
    void SetSaturation(float val)
    {
        n_assert(!this->handled);
        this->saturation = val;
    };
    float GetSaturation() const
    {
        return this->saturation;
    };
private:
    float saturation;
public:
    void SetBalance(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->balance = val;
    };
    const Math::float4& GetBalance() const
    {
        return this->balance;
    };
private:
    Math::float4 balance;
};
//------------------------------------------------------------------------------
class BlendDepthOfField : public BlendBase
{
    __DeclareClass(BlendDepthOfField);
    __DeclareMsgId;
public:
    BlendDepthOfField() 
    { };
public:
    void SetFocusDistance(float val)
    {
        n_assert(!this->handled);
        this->focusdistance = val;
    };
    float GetFocusDistance() const
    {
        return this->focusdistance;
    };
private:
    float focusdistance;
public:
    void SetFocusLength(float val)
    {
        n_assert(!this->handled);
        this->focuslength = val;
    };
    float GetFocusLength() const
    {
        return this->focuslength;
    };
private:
    float focuslength;
public:
    void SetFilterSize(float val)
    {
        n_assert(!this->handled);
        this->filtersize = val;
    };
    float GetFilterSize() const
    {
        return this->filtersize;
    };
private:
    float filtersize;
};
//------------------------------------------------------------------------------
class BlendFog : public BlendBase
{
    __DeclareClass(BlendFog);
    __DeclareMsgId;
public:
    BlendFog() 
    { };
public:
    void SetNearDistance(float val)
    {
        n_assert(!this->handled);
        this->neardistance = val;
    };
    float GetNearDistance() const
    {
        return this->neardistance;
    };
private:
    float neardistance;
public:
    void SetFarDistance(float val)
    {
        n_assert(!this->handled);
        this->fardistance = val;
    };
    float GetFarDistance() const
    {
        return this->fardistance;
    };
private:
    float fardistance;
public:
    void SetHeight(float val)
    {
        n_assert(!this->handled);
        this->height = val;
    };
    float GetHeight() const
    {
        return this->height;
    };
private:
    float height;
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
};
//------------------------------------------------------------------------------
class BlendHdr : public BlendBase
{
    __DeclareClass(BlendHdr);
    __DeclareMsgId;
public:
    BlendHdr() 
    { };
public:
    void SetBloomIntensity(float val)
    {
        n_assert(!this->handled);
        this->bloomintensity = val;
    };
    float GetBloomIntensity() const
    {
        return this->bloomintensity;
    };
private:
    float bloomintensity;
public:
    void SetBloomThreshold(float val)
    {
        n_assert(!this->handled);
        this->bloomthreshold = val;
    };
    float GetBloomThreshold() const
    {
        return this->bloomthreshold;
    };
private:
    float bloomthreshold;
public:
    void SetBloomColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->bloomcolor = val;
    };
    const Math::float4& GetBloomColor() const
    {
        return this->bloomcolor;
    };
private:
    Math::float4 bloomcolor;
};
//------------------------------------------------------------------------------
class BlendLight : public BlendBase
{
    __DeclareClass(BlendLight);
    __DeclareMsgId;
public:
    BlendLight() 
    { };
public:
    void SetLightTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->lighttransform = val;
    };
    const Math::matrix44& GetLightTransform() const
    {
        return this->lighttransform;
    };
private:
    Math::matrix44 lighttransform;
public:
    void SetBackLightFactor(float val)
    {
        n_assert(!this->handled);
        this->backlightfactor = val;
    };
    float GetBackLightFactor() const
    {
        return this->backlightfactor;
    };
private:
    float backlightfactor;
public:
    void SetShadowIntensity(float val)
    {
        n_assert(!this->handled);
        this->shadowintensity = val;
    };
    float GetShadowIntensity() const
    {
        return this->shadowintensity;
    };
private:
    float shadowintensity;
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetLightOppositeColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightoppositecolor = val;
    };
    const Math::float4& GetLightOppositeColor() const
    {
        return this->lightoppositecolor;
    };
private:
    Math::float4 lightoppositecolor;
public:
    void SetLightAmbientColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightambientcolor = val;
    };
    const Math::float4& GetLightAmbientColor() const
    {
        return this->lightambientcolor;
    };
private:
    Math::float4 lightambientcolor;
};
//------------------------------------------------------------------------------
class BlendSky : public BlendBase
{
    __DeclareClass(BlendSky);
    __DeclareMsgId;
public:
    BlendSky() 
    { };
public:
    void SetContrast(float val)
    {
        n_assert(!this->handled);
        this->contrast = val;
    };
    float GetContrast() const
    {
        return this->contrast;
    };
private:
    float contrast;
public:
    void SetBrightness(float val)
    {
        n_assert(!this->handled);
        this->brightness = val;
    };
    float GetBrightness() const
    {
        return this->brightness;
    };
private:
    float brightness;
public:
    void SetTexture(const Util::String& val)
    {
        n_assert(!this->handled);
        this->texture = val;
    };
    const Util::String& GetTexture() const
    {
        return this->texture;
    };
private:
    Util::String texture;
};
} // namespace PostEffect
//------------------------------------------------------------------------------
