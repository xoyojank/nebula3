//------------------------------------------------------------------------------
//  posteffect/posteffectmanager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "posteffect/posteffectmanager.h"
#include "basegametiming/gametimesource.h"
#include "basegamefeature/managers/enventitymanager.h"
#include "basegametiming/systemtimesource.h"
#include "posteffect/posteffectprotocol.h"
#include "graphics/graphicsinterface.h"
#include "posteffect/rt/posteffectserver.h"

#include "input/key.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "input/inputserver.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DefineFloat(Saturation, 'ASAT', ReadWrite);
    DefineFloat4(Balance, 'BALA', ReadWrite);

    DefineFloat(BloomScale, 'BLSC', ReadWrite);
    DefineFloat4(BloomColor, 'BCOL', ReadWrite);
    DefineFloat(BrightPassThreshold, 'BRPT', ReadWrite);

    DefineFloat(FogNearDist, 'FNDI', ReadWrite);
    DefineFloat(FogFarDist, 'FFDI', ReadWrite);
    DefineFloat(FogHeight, 'FHEI', ReadWrite);
    DefineFloat4(FogColor, 'FOGC', ReadWrite);
    DefineFloat(FogIntensity, 'FOGI', ReadWrite);

    DefineFloat(FocusDist, 'FOCD', ReadWrite);
    DefineFloat(FocusLength, 'FOCL', ReadWrite);

    DefineFloat(SkyContrast, 'SKYC', ReadWrite);
    DefineFloat(SkyBrightness, 'SKYB', ReadWrite);
    DefineString(SkyTexture, 'SKYT', ReadWrite);

    DefineFloat(BackLightFactor, 'BLFA', ReadWrite);
    DefineFloat(LightShadowIntensity, 'LISI', ReadWrite);   
};

namespace PostEffect
{
using namespace Math;
using namespace Input;

__ImplementClass(PostEffect::PostEffectManager, 'PEMA', Game::Manager);
__ImplementSingleton(PostEffect::PostEffectManager);

//------------------------------------------------------------------------------
/**
*/
PostEffectManager::PostEffectManager() :
    firstFrame(false)
{   
    __ConstructSingleton;
}   

//------------------------------------------------------------------------------
/**
*/
PostEffectManager::~PostEffectManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::OnActivate()
{
    n_assert(this->entities.IsEmpty());
    n_assert(!this->defaultEntity.isvalid());
    n_assert(!this->currentEntity.isvalid());
    n_assert(!this->globalLightEntity.isvalid());
    n_assert(this->skyEntities.IsEmpty());

    // setup posteffect rendermodule
    this->postEffectRenderModule = PostEffect::PostEffectRenderModule::Create();
    this->postEffectRenderModule->Setup();
    
    // init
    this->firstFrame = true;

    this->curDepthOfFieldParams = DepthOfFieldParams::Create();
    this->curColorParams = ColorParams::Create();
    this->curFogParams = FogParams::Create();
    this->curHdrParams = HdrParams::Create();
    this->curLightParams = LightParams::Create();
    this->curSkyParams = SkyParams::Create();
    
    // call parent
    Game::Manager::OnActivate();
}

//------------------------------------------------------------------------------
/**
    Find the sky entity and set it
*/
void
PostEffectManager::OnStart()
{
    // find global sky object
    Util::Array<Ptr<Graphics::ModelEntity> > skyEntities = BaseGameFeature::EnvEntityManager::Instance()->GetGraphicsEntities("|Level|Environment|sky_1");
    this->SetSkyEntities(skyEntities);

    // call parent
    Game::Manager::OnStart();
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::OnFrame()
{
    // find the current "point of interest entity", which is the entity where the
    // point of interest is currently in (or the default entity), if there is
    // no POI-Entity found in the level this means there is no
    // PostEffects-Stuff at all in this level
    Ptr<PostEffectEntity> poiEntity = this->FindPointOfInterestEntity();
    if (poiEntity.isvalid())
    {
        // check if a new blend must be started, if currently a blend
        // is in progress this blend must not be disturbed to prevent
        // popping!
        if (this->currentEntity != poiEntity)
        {
            // start blending to values from the poi entity
            Timing::Time blendTime = poiEntity->GetBlendTime();
            this->GetParamsFromEntity(poiEntity);
            this->SendColorParameters(blendTime);
            this->SendDepthOfFieldParameters(blendTime);
            this->SendFogParameters(blendTime);
            this->SendHdrParameters(blendTime);
            this->SendLightParameters(blendTime);
            this->SendSkyParameters(blendTime);

            // now set current to poi, to avoid multiple para sending
            this->currentEntity = poiEntity;
        }
    }

    // call parent
    Game::Manager::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::OnDeactivate()
{
    this->ResetPostEffectSystem();

    // Discard posteffect render module
    this->postEffectRenderModule->Discard();
    this->postEffectRenderModule = 0;

    // call parent
    Game::Manager::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::ResetPostEffectSystem()
{
    while (!this->entities.IsEmpty())
    {
        this->RemoveEntity(this->entities[0]);
    }
    n_assert(!this->defaultEntity.isvalid());
    n_assert(!this->currentEntity.isvalid());

    // release special graphics entities
    this->globalLightEntity = 0;
    this->skyEntities.Clear();

    // send reset to render thread
    Ptr<PostEffect::ResetPostEffectSystem> resetMsg = PostEffect::ResetPostEffectSystem::Create();
    Graphics::GraphicsInterface::Instance()->SendBatched(resetMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::AttachEntity(const Ptr<PostEffectEntity>& entity)
{
    n_assert(0 != entity);
    this->entities.Append(entity);
    entity->OnAttach();

    // check if this post effect entity has a texture to preload
    Util::String texPath = entity->Params().sky->GetSkyTexturePath();
    if(texPath.IsValid())
    {
        const Ptr<PreloadTexture> loadTex = PreloadTexture::Create();
        loadTex->SetResource(texPath);
        Graphics::GraphicsInterface::Instance()->SendBatched(loadTex.cast<Messaging::Message>());
    }

    if (entity->IsDefaultEntity())
    {
        n_assert2(this->defaultEntity == 0, "PostEffectManager::AttachEntity: Allready a default posteffect entity set!\n");
        this->defaultEntity = entity;
    }   
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::RemoveEntity(const Ptr<PostEffectEntity>& entity)
{
    n_assert(0 != entity);
    int index = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != index);

    // check if this post effect entity has a texture to preload
    Util::String texPath = entity->Params().sky->GetSkyTexturePath();
    if(texPath.IsValid())
    {
        const Ptr<UnloadTexture> loadTex = UnloadTexture::Create();
        loadTex->SetResource(texPath);
        Graphics::GraphicsInterface::Instance()->SendBatched(loadTex.cast<Messaging::Message>());
    }
    entity->OnRemove();
    if (this->defaultEntity == entity)
    {
        this->defaultEntity = 0;
    }
    if (this->currentEntity == entity)
    {
        this->currentEntity = 0;
    }
    this->entities.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    Set current post effect parameters to the values from the
    current blend target entity without blending.
*/
void
PostEffectManager::GetParamsFromEntity(const Ptr<PostEffectEntity>& entity)
{
    n_assert(0 != entity);
    this->curColorParams = entity->Params().color;
    this->curDepthOfFieldParams = entity->Params().dof;
    this->curFogParams = entity->Params().fog;
    this->curHdrParams = entity->Params().hdr;
    this->curLightParams = entity->Params().light;
    this->curSkyParams = entity->Params().sky;

    // set the light transform for the global directional light, if the 
    // blend target entity is a sphere we just use its transformation,
    // if it is a box we use the transform of the default entity since
    // the orientation of the box cannot be used for the directional light
    if (entity->GetShapeType() == PostEffectEntity::Box)
    {
        if (this->defaultEntity.isvalid())
        {
            this->curLightParams->SetLightTransform(this->defaultEntity->GetTransform());
        }
    }
}

//------------------------------------------------------------------------------
/**
This method finds the current "point of interest entity" which is the
highest priority entity where the current point of interest is inside, 
or the default entity if not inside a specific entity. 

NOTE: The method may return NULL if there is no default entity in the
level! 
*/
Ptr<PostEffectEntity>
PostEffectManager::FindPointOfInterestEntity()
{
    Ptr<PostEffectEntity> poiEntity = 0;

    // because of entity priorities we need to check all entities
    // every time...
    int entityIndex;    
    for (entityIndex = 0; entityIndex < this->entities.Size(); entityIndex++)
    {
        const Ptr<PostEffectEntity>& curEntity = this->entities[entityIndex];
        if (!curEntity->IsDefaultEntity() && 
             curEntity->IsInside(this->GetPointOfInterest()) &&
             curEntity->IsEnabled())
        {
            // check priorities...
            if (poiEntity.isvalid())
            {
                if (curEntity->GetPriority() > poiEntity->GetPriority())
                {
                    // new entity is higher priority
                    poiEntity = curEntity;
                }
            }
            else
            {
                // no previous poiEntity, assign without priority check
                poiEntity = curEntity;
            }
        }
    }

    // if the point of interest is outside all non-default entities, 
    // use the default entity
    if (!poiEntity.isvalid())
    {
        poiEntity = this->defaultEntity.get_unsafe();
    }

    // hmm, if there's still no point of interest entity at this point
    // there's nothing we can do... so we may return a NULL pointer
    return poiEntity;
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartDepthOfFieldBlend(const Ptr<DepthOfFieldParams>& params, Timing::Time blendTime)
{
    this->curDepthOfFieldParams = params;
    this->SendDepthOfFieldParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartFogBlend(const Ptr<FogParams>& params, Timing::Time blendTime)
{
    this->curFogParams = params;
    this->SendFogParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartColorBlend(const Ptr<ColorParams>& params, Timing::Time blendTime)
{
    this->curColorParams = params;
    this->SendColorParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartHdrBlend(const Ptr<HdrParams>& params, Timing::Time blendTime)
{
    this->curHdrParams = params;
    this->SendHdrParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartLightBlend(const Ptr<LightParams>& params, Timing::Time blendTime)
{
    this->curLightParams = params;
    this->SendLightParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::StartSkyBlend(const Ptr<SkyParams>& params, Timing::Time blendTime)
{
    this->curSkyParams = params;
    this->SendSkyParameters(blendTime);
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::SendColorParameters(Timing::Time blendTime)
{
    Ptr<PostEffect::BlendColor> setColorMsg = PostEffect::BlendColor::Create();
    setColorMsg->SetBalance(this->curColorParams->GetColorBalance());
    setColorMsg->SetSaturation(this->curColorParams->GetColorSaturation());
    setColorMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setColorMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::SendHdrParameters(Timing::Time blendTime)
{
    Ptr<PostEffect::BlendHdr> setHdrMsg = PostEffect::BlendHdr::Create();
    setHdrMsg->SetBloomColor(this->curHdrParams->GetHdrBloomColor());
    setHdrMsg->SetBloomIntensity(this->curHdrParams->GetHdrBloomIntensity());
    setHdrMsg->SetBloomThreshold(this->curHdrParams->GetHdrBloomThreshold());
    setHdrMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setHdrMsg.cast<Messaging::Message>());    
}


//------------------------------------------------------------------------------
/**
*/
void
PostEffectManager::SendDepthOfFieldParameters(Timing::Time blendTime)
{    
    Ptr<PostEffect::BlendDepthOfField> setDofMsg = PostEffect::BlendDepthOfField::Create();
    setDofMsg->SetFilterSize(this->curDepthOfFieldParams->GetFilterSize());
    setDofMsg->SetFocusDistance(this->curDepthOfFieldParams->GetFocusDistance());
    setDofMsg->SetFocusLength(this->curDepthOfFieldParams->GetFocusLength());
    setDofMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setDofMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::SendFogParameters(Timing::Time blendTime)
{
    Ptr<PostEffect::BlendFog> setFogMsg = PostEffect::BlendFog::Create();
    setFogMsg->SetColor(this->curFogParams->GetFogColorAndIntensity());
    setFogMsg->SetNearDistance(this->curFogParams->GetFogNearDistance());
    setFogMsg->SetFarDistance(this->curFogParams->GetFogFarDistance());
    setFogMsg->SetHeight(this->curFogParams->GetFogHeight());
    setFogMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setFogMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::SendLightParameters(Timing::Time blendTime)
{
    Ptr<PostEffect::BlendLight> setLightMsg = PostEffect::BlendLight::Create();
    setLightMsg->SetLightTransform(this->curLightParams->GetLightTransform());
    setLightMsg->SetBackLightFactor(this->curLightParams->GetBackLightFactor());
    setLightMsg->SetShadowIntensity(this->curLightParams->GetLightShadowIntensity());
    setLightMsg->SetLightColor(this->curLightParams->GetLightColor());
    setLightMsg->SetLightOppositeColor(this->curLightParams->GetLightOppositeColor());
    setLightMsg->SetLightAmbientColor(this->curLightParams->GetLightAmbientColor());
    setLightMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setLightMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::SendSkyParameters(Timing::Time blendTime)
{
    Ptr<PostEffect::BlendSky> setSkyMsg = PostEffect::BlendSky::Create();
    setSkyMsg->SetContrast(this->curSkyParams->GetSkyContrast());
    setSkyMsg->SetBrightness(this->curSkyParams->GetSkyBrightness());
    setSkyMsg->SetTexture(this->curSkyParams->GetSkyTexturePath());
    setSkyMsg->SetBlendTime(blendTime);
    Graphics::GraphicsInterface::Instance()->SendBatched(setSkyMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::FadeOut(Timing::Time time)
{
    Ptr<PostEffect::FadeOut> fadeOutMsg = PostEffect::FadeOut::Create();
    fadeOutMsg->SetBlendTime(time);
    Graphics::GraphicsInterface::Instance()->SendBatched(fadeOutMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::FadeOutAndWait(Timing::Time time)
{
    Ptr<PostEffect::FadeOut> fadeOutMsg = PostEffect::FadeOut::Create();
    fadeOutMsg->SetBlendTime(time);
    Graphics::GraphicsInterface::Instance()->SendWait(fadeOutMsg.cast<Messaging::Message>());    
}

//------------------------------------------------------------------------------
/**    
*/
void
PostEffectManager::FadeIn(Timing::Time time)
{
    Ptr<PostEffect::FadeIn> fadeInMsg = PostEffect::FadeIn::Create();
    fadeInMsg->SetBlendTime(time);
    Graphics::GraphicsInterface::Instance()->SendBatched(fadeInMsg.cast<Messaging::Message>());    
}

} // namespace Server
