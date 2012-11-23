#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::PostEffectServer
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"
#include "timing/time.h"
#include "posteffect/rt/params/colorparams.h"
#include "posteffect/rt/params/depthoffieldparams.h"
#include "posteffect/rt/params/fogparams.h"
#include "posteffect/rt/params/hdrparams.h"
#include "posteffect/rt/params/lightparams.h"
#include "posteffect/rt/params/skyparams.h"

// forward declaration
namespace Lighting
{
    class InternalGlobalLightEntity;
}
namespace Resources
{
    class ManagedTexture;
}

//------------------------------------------------------------------------------
namespace PostEffect
{
class PostEffectServer : public Core::RefCounted
{
    __DeclareClass(PostEffectServer);
    __DeclareSingleton(PostEffectServer);

public:

    enum PostEffectType
    {
        Color = 0,
        DoF = 1,
        Fog = 2,
        Hdr = 3,
        Light = 4,
        Sky = 5,
        Fadeout = 6,
        FadeoutAndWait = 7,
        Fadein = 8,

        NumPostEffectTypes,
        InvalidPostEffectType
    };

    enum FadeMode
    {
        FadeOut = 0,
        FadeIn,
        NoFade
    };

    /// constructor
    PostEffectServer();
    /// destructor
    virtual ~PostEffectServer();  
    
    /// open the posteffect server
    void Open();
    /// close the posteffect server
    void Close();
    /// start fadeout
    void StartFadeOut(Timing::Time time);
    /// start fadeout
    void StartFadeIn(Timing::Time time);

    /// on frame
    void OnFrame(Timing::Time time);

    /// get current fademode    
    FadeMode GetFadeMode() const;

    /// starts blending for specified effect
    void StartBlending(const Ptr<ParamBase>& target, Timing::Time fadeTime, PostEffectType postEffectType);
    /// stops blending for specified effect
    void StopBlending(PostEffectType postEffectType);

    /// preload a texture
    void PreloadTexture(const Util::String& resource);
    /// unload one texture
    void UnloadTexture(const Util::String &resource);
    /// unload all textures
    void UnloadTextures();

private:
    /// apply ´special parameter
    void ApplyParameters(PostEffectType type);

    /// set the depth of field parameters
    void ApplyDepthOfFieldParameters();    
    /// set the color parameters
    void ApplyColorParameters(); 
    /// set the fog parameters
    void ApplyFogParameters();
    /// set the hdr parameters
    void ApplyHdrParameters();
    /// set the light parameters
    void ApplyLightParameters();
    /// set the sky parameters
    void ApplySkyParameters();

    Timing::Time lastTime;

    ////////////////////////////////////////////////////////////////////////// REWORK FADE STUFF
    /// compute fadevalue
    float ComputeFadeValue(Timing::Time frameTime);
    /// simple fade parameters
    Timing::Time fadeTime;
    float currentFadeValue;
    FadeMode curFadeMode;    
    ////////////////////////////////////////////////////////////////////////// REWORK FADE STUFF

    /// finds the current sky entities
    bool FindCurrentSkyEntities();

    /// struct for actuall parameters and assigned targets
    struct ParameterSet
    {
        Ptr<ParamBase> current;
        Ptr<ParamBase> target;
        Timing::Time lastTime ;
        Timing::Time blendEndTime;
        Timing::Time blendTime;
    };

    /// compute fade values, returns true if something changed
    bool ComputeBlending(PostEffectType type);

    /// holds all of the parameter sets
    Util::FixedArray<ParameterSet> postEffects;

    /// shader variable handles
    Ptr<CoreGraphics::ShaderVariable> fadeShaderVariable;
    Ptr<CoreGraphics::ShaderVariable> saturationShaderVariable;
    Ptr<CoreGraphics::ShaderVariable> balanceShaderVariable;
    Ptr<CoreGraphics::ShaderVariable> fogDistancesVariable;
    Ptr<CoreGraphics::ShaderVariable> fogColorVariable;
    Ptr<CoreGraphics::ShaderVariable> hdrScaleVariable;
    Ptr<CoreGraphics::ShaderVariable> hdrColorVariable;
    Ptr<CoreGraphics::ShaderVariable> hdrThresholdVariable;
    Ptr<CoreGraphics::ShaderVariable> dofVariable;
    Ptr<CoreGraphics::ShaderVariableInstance> skyContrast;
    Ptr<CoreGraphics::ShaderVariableInstance> skyBrightness;
    Ptr<CoreGraphics::ShaderVariableInstance> skyBlendFactor;
    Ptr<CoreGraphics::ShaderVariableInstance> skyBlendTexture;
    Ptr<CoreGraphics::ShaderVariableInstance> skyBaseTexture;
    bool skyLoaded; // flag is true if sky model was loaded and all shader params could be initialized

    /// handle to global light
    Ptr<Lighting::InternalGlobalLightEntity> globalLight;

    /// texture pool
    Util::Dictionary<Util::String, Ptr<Resources::ManagedTexture> > texturePool;
};


//------------------------------------------------------------------------------
/**
*/
inline void 
PostEffectServer::StartFadeOut(Timing::Time time)
{
    this->fadeTime = time;
    this->curFadeMode = FadeOut;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
PostEffectServer::StartFadeIn(Timing::Time time)
{
    this->fadeTime = time;
    this->curFadeMode = FadeIn;
}

//------------------------------------------------------------------------------
/**
*/
inline PostEffectServer::FadeMode 
PostEffectServer::GetFadeMode() const
{
    return this->curFadeMode;
}

} // namespace Debug
//------------------------------------------------------------------------------
