#pragma once
//------------------------------------------------------------------------------
/**
    @class PostEffect::PostEffectManager
  
    Server object of the post effect subsystem.
    
    (C) 2006 Radon Labs GmbH
*/
#include "game/manager.h"
#include "core/singleton.h"
#include "posteffect/posteffectentity.h"
#include "posteffect/rt/params/depthoffieldparams.h"
#include "timing/time.h"
#include "graphics/globallightentity.h"
#include "graphics/modelentity.h"
#include "posteffect/posteffectrendermodule.h"
#include "attr/attrid.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareFloat(Saturation, 'ASAT', ReadWrite);
    DeclareFloat4(Balance, 'BALA', ReadWrite);

    DeclareFloat(BloomScale, 'BLSC', ReadWrite);
    DeclareFloat4(BloomColor, 'BCOL', ReadWrite);
    DeclareFloat(BrightPassThreshold, 'BRPT', ReadWrite);

    DeclareFloat(FogNearDist, 'FNDI', ReadWrite);
    DeclareFloat(FogFarDist, 'FFDI', ReadWrite);
    DeclareFloat(FogHeight, 'FHEI', ReadWrite);
    DeclareFloat4(FogColor, 'FOGC', ReadWrite);
    DeclareFloat(FogIntensity, 'FOGI', ReadWrite);

    DeclareFloat(FocusDist, 'FOCD', ReadWrite);
    DeclareFloat(FocusLength, 'FOCL', ReadWrite);

    DeclareFloat(SkyContrast, 'SKYC', ReadWrite);
    DeclareFloat(SkyBrightness, 'SKYB', ReadWrite);
    DeclareString(SkyTexture, 'SKYT', ReadWrite);

    DeclareFloat(BackLightFactor, 'BLFA', ReadWrite);
    DeclareFloat(LightShadowIntensity, 'LISI', ReadWrite);   
};

//------------------------------------------------------------------------------
namespace PostEffect
{
class PostEffectManager : public Game::Manager
{
    __DeclareClass(PostEffectManager);
    __DeclareSingleton(PostEffectManager);
public:
    
    /// constructor
    PostEffectManager();
    /// destructor
    virtual ~PostEffectManager();

    /// open the posteffect server
    virtual void OnActivate();
    /// close the posteffect server
    virtual void OnDeactivate();
    /// called by Game::Server::Start() when the world is started
    virtual void OnStart();
    /// "render" the current post effect
    virtual void OnFrame();

    /// reset posteffect manager and trigger render thread to reset the server
    void ResetPostEffectSystem();

    /// set the main directional light which should be influenced by post effect entities
    /// (this is set from lightentity on creation of global light entity)
    void SetGlobalLightEntity(const Ptr<Graphics::GlobalLightEntity>& l);
    /// get the main directional light (may return 0)
    const Ptr<Graphics::GlobalLightEntity>& GetGlobalLightEntity() const;

    /// set the graphics entities which represent the sky (optional)
    void SetSkyEntities(const Util::Array<Ptr<Graphics::ModelEntity> >& e);
    /// get the graphics entities which represent the sky (optional)
    const Util::Array<Ptr<Graphics::ModelEntity> >& GetSkyEntities() const;

    /// attach a post effect entity to the server
    void AttachEntity(const Ptr<PostEffectEntity>& entity);
    /// remove a post effect entity from the server
    void RemoveEntity(const Ptr<PostEffectEntity>& entity);
    
    /// set the current point of interest (usually camera pos)
    /// (this is set from the cameraproperty on render)
    void SetPointOfInterest(const Math::point& p);
    /// get the current point of interest
    const Math::point& GetPointOfInterest() const;

    /// set depth-of-field for blending (time 0 causes instant set, no fade)
    void StartDepthOfFieldBlend(const Ptr<DepthOfFieldParams>& params, Timing::Time duration);
    /// set the fog parameters for blending (time 0 causes instant set, no fade)
    void StartFogBlend(const Ptr<FogParams>& params, Timing::Time blendTime);
    /// set the color parameters for blending (time 0 causes instant set, no fade)
    void StartColorBlend(const Ptr<ColorParams>& params, Timing::Time blendTime);
    /// set the hdr parameters for blending (time 0 causes instant set, no fade)
    void StartHdrBlend(const Ptr<HdrParams>& params, Timing::Time blendTime);
    /// set the light parameters for blending (time 0 causes instant set, no fade)
    void StartLightBlend(const Ptr<LightParams>& params, Timing::Time blendTime);
    /// set the sky parameters for blending (time 0 causes instant set, no fade)
    void StartSkyBlend(const Ptr<SkyParams>& params, Timing::Time blendTime);

    /// fade out
    void FadeOut(Timing::Time time);
    /// fade out and wait till renderthread has faded out
    void FadeOutAndWait(Timing::Time time);
    /// fade in
    void FadeIn(Timing::Time time);

    /// get CurFogParams	
    const Ptr<FogParams>& GetCurFogParams() const;

private:
    /// find current "point-of-interest" entity
    Ptr<PostEffectEntity> FindPointOfInterestEntity();

    /// set current blend params directly from the entity without blending
    void GetParamsFromEntity(const Ptr<PostEffectEntity>& entity);

    /// send the depth-of-field render variables update
    void SendDepthOfFieldParameters(Timing::Time blendTime);
    /// send the fog parameters update
    void SendFogParameters(Timing::Time blendTime);
    /// send the color parameters update
    void SendColorParameters(Timing::Time blendTime);
    /// send the hdr parameters update
    void SendHdrParameters(Timing::Time blendTime);
    /// send the light parameters update
    void SendLightParameters(Timing::Time blendTime);
    /// send the sky parameters update
    void SendSkyParameters(Timing::Time blendTime);
  
    bool firstFrame;

    Util::Array<Ptr<PostEffectEntity> > entities;
    Ptr<PostEffectEntity> currentEntity;
    Ptr<PostEffectEntity> defaultEntity;

    Math::point pointOfInterest;

    Ptr<Graphics::GlobalLightEntity> globalLightEntity;     // the global directional light source
    Util::Array<Ptr<Graphics::ModelEntity> > skyEntities;    // the global sky entity  
      
    Ptr<DepthOfFieldParams> curDepthOfFieldParams;
    Ptr<ColorParams> curColorParams;
    Ptr<FogParams> curFogParams;
    Ptr<HdrParams> curHdrParams;
    Ptr<LightParams> curLightParams;
    Ptr<SkyParams> curSkyParams;

    Ptr<PostEffectRenderModule> postEffectRenderModule;
};

//------------------------------------------------------------------------------
/**
*/
inline void
PostEffectManager::SetGlobalLightEntity(const Ptr<Graphics::GlobalLightEntity>& l)
{
    this->globalLightEntity = l;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::GlobalLightEntity>&
PostEffectManager::GetGlobalLightEntity() const
{
    return this->globalLightEntity;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PostEffectManager::SetSkyEntities(const Util::Array<Ptr<Graphics::ModelEntity> >& e)
{
    this->skyEntities = e;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<Graphics::ModelEntity> >&
PostEffectManager::GetSkyEntities() const
{
    return this->skyEntities;
}

//------------------------------------------------------------------------------
/**
*/
inline void
PostEffectManager::SetPointOfInterest(const Math::point& p)
{
    this->pointOfInterest = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
PostEffectManager::GetPointOfInterest() const
{
    return this->pointOfInterest;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FogParams>& 
PostEffectManager::GetCurFogParams() const
{
    return this->curFogParams;
}

} // namespace PostEffect
//------------------------------------------------------------------------------
