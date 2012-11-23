//------------------------------------------------------------------------------
//  posteffectserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "posteffect/rt/posteffectserver.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/internalmodelentity.h"
#include "coregraphics/shaderserver.h"
#include "lighting/internalgloballightentity.h"
#include "models/nodes/statenodeinstance.h"
#include "resources/managedtexture.h"
#include "resources/resourcemanager.h"
#include "coregraphics/shadersemantics.h"

namespace PostEffect
{
__ImplementClass(PostEffect::PostEffectServer, 'POSR', Core::RefCounted);
__ImplementSingleton(PostEffectServer);

using namespace Util;
using namespace CoreGraphics;
using namespace InternalGraphics;
using namespace Resources;
using namespace Math;
using namespace Models;

//------------------------------------------------------------------------------
/**
*/
PostEffectServer::PostEffectServer():
    lastTime(0.0),
    fadeTime(0.0),
    currentFadeValue(1.0f),
    curFadeMode(NoFade),
    postEffects(NumPostEffectTypes),
    skyLoaded(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
PostEffectServer::~PostEffectServer()
{
    this->Close();

    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::Open()
{
    const Ptr<ShaderServer> shdServer = ShaderServer::Instance();

    // lookup the shared post effect fade variable
    this->fadeShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_FADEVALUE));
    
    // color stuff
    this->saturationShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_SATURATION));
    this->balanceShaderVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_BALANCE));

    // fog stuff
    this->fogColorVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_FOGCOLOR));
    this->fogDistancesVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_FOGDISTANCES));

    // hdr stuff
    this->hdrColorVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_HDRBLOOMCOLOR));
    this->hdrScaleVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_HDRBLOOMSCALE));
    this->hdrThresholdVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_HDRBRIGHTPASSTHRESHOLD));

    // dof stuff
    this->dofVariable = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DOFDISTANCES));
}

//------------------------------------------------------------------------------
/**
*/
bool
PostEffectServer::FindCurrentSkyEntities()
{
    // find the right shader variable instance
    const Ptr<InternalGraphicsServer>& gfxServer = InternalGraphicsServer::Instance();
    const Ptr<InternalView>& view = gfxServer->GetDefaultView();
    if(view.isvalid())
    {
        const Ptr<InternalStage>& stage = view->GetStage();
        if(stage.isvalid())
        {
            const Util::Array<Ptr<InternalGraphicsEntity> >& entities = stage->GetEntitiesByType(InternalGraphicsEntityType::Model);

            IndexT index;
            for(index = 0; index < entities.Size(); index++)
            {
                const Ptr<InternalModelEntity> internalModel = entities[index].downcast<InternalModelEntity>();

                // check if this is our sky
                if (internalModel->GetResourceId() == "mdl:skies/sky.n3")
                {
                    const Ptr<ModelInstance>& modelInstance = internalModel->GetModelInstance();
                    if (modelInstance.isvalid())
                    {
                        const Array<Ptr<ModelNodeInstance> >& modelNodeInstances = modelInstance->GetNodeInstances();
                        IndexT modelNodeInstIndex;
                        for (modelNodeInstIndex = 0; modelNodeInstIndex < modelNodeInstances.Size(); modelNodeInstIndex++)
                        {
                            const Ptr<ModelNodeInstance>& nodeInst = modelNodeInstances[modelNodeInstIndex];
                            if (nodeInst->IsA(StateNodeInstance::RTTI))
                            {
                                const Ptr<StateNodeInstance>& stateNodeInst = nodeInst.downcast<StateNodeInstance>();
                                if (!stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP0)))
                                {
                                    stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP0));
                                }
                                this->skyBaseTexture = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP0));

                                if (!stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP1)))
                                {
                                    stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP1));
                                }
                                this->skyBlendTexture = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_DIFFMAP1));

                                if (!stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY2)))
                                {
                                    stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY2));
                                }
                                this->skyBlendFactor = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY2));

                                if (!stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY1)))
                                {
                                    stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY1));
                                }
                                this->skyBrightness = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY1));

                                if (!stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY0)))
                                {
                                    stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY0));
                                }
                                this->skyContrast = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(NEBULA3_SEMANTIC_INTENSITY0));

                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::Close()
{
    // delete all posteffect structs
    IndexT index;
    for (index = 0; index < this->postEffects.Size(); index++)
    {
        ParameterSet set;
        this->postEffects[index] = set;
    }

    this->curFadeMode = NoFade;
    this->currentFadeValue = 1.0f;
    this->fadeShaderVariable = 0;
    this->saturationShaderVariable = 0;
    this->balanceShaderVariable = 0;
    this->fogDistancesVariable = 0;
    this->fogColorVariable = 0;
    this->hdrScaleVariable = 0;
    this->hdrColorVariable = 0;
    this->hdrThresholdVariable = 0;
    this->dofVariable = 0;
    this->skyContrast = 0;
    this->skyBrightness = 0;
    this->skyBlendFactor = 0;
    this->skyBlendTexture = 0;
    this->skyBaseTexture = 0;
    this->globalLight = 0;
    this->skyLoaded = false;

    this->UnloadTextures();
}

//------------------------------------------------------------------------------
/**
*/
void 
PostEffectServer::OnFrame(Timing::Time time)
{
    // if the shader variables are not yet connected to the sky shader, try
    if (!skyLoaded)
    {
        this->skyLoaded = this->FindCurrentSkyEntities();        
    }

    // if no global light entity set, find the new one
    //   (can be used for setting a new, in case of changing the view
    //    the old one should be reseted and here we will find the new one)
    if(!this->globalLight.isvalid())
    {
        this->globalLight = InternalGraphics::InternalGraphicsServer::Instance()->GetCurrentGlobalLightEntity();
        // return, the light still can be invalid
        return;
    }

    Timing::Time frameTime = time - this->lastTime;
    this->lastTime = time;

    // compute simple fading
    if (this->curFadeMode != NoFade)
    {
        this->currentFadeValue = this->ComputeFadeValue(frameTime);
        // apply fadeValue on shader variable
        this->fadeShaderVariable->SetFloat(this->currentFadeValue);
    }

    // now compute further post effect blendings
    IndexT index;
    for(index = 0; index < NumPostEffectTypes; index++)
    {
        if(this->ComputeBlending((PostEffectType)index))
        {
            this->ApplyParameters((PostEffectType)index);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
PostEffectServer::ComputeBlending(PostEffectType type)
{    
    const Ptr<ParamBase>& target = this->postEffects[(int)type].target;
    const Ptr<ParamBase>& current = this->postEffects[(int)type].current;
    
    //n_assert(current.isvalid());
    if(target.isvalid())
    {
        // if this is the first blend request
        if(!current.isvalid())
        {
            // set current, apply instantly and stop blending
            this->postEffects[type].current = target;
            this->ApplyParameters(type);
            this->StopBlending(type);
            return false;
        }
        else if(!current->Equals(target, TINY))
        {
            // how much have we progressed since last time?
            // and how much time left to the end?
            Timing::Time timeDiff = this->lastTime - this->postEffects[(int)type].lastTime;
            Timing::Time timeToEnd = n_max(this->postEffects[(int)type].blendEndTime - this->lastTime, 0.0001);

            // get normalized lerp value and perform blending
            float lerpVal = n_saturate(float(timeDiff / timeToEnd));
            current->BlendTo( target , lerpVal);

            // store current time for parameterblend
            this->postEffects[(int)type].lastTime = this->lastTime;

            return true;
        }
        else
        {
            this->StopBlending(type);
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyParameters(PostEffectType type)
{   
    switch(type)
    {
        case Color:
            this->ApplyColorParameters();
            break;
        case DoF:
            this->ApplyDepthOfFieldParameters();
            break;
        case Fog:
            this->ApplyFogParameters();
            break;
        case Hdr:
            this->ApplyHdrParameters();
            break;
        case Sky:
            this->ApplySkyParameters();
            break;
        case Light:
            this->ApplyLightParameters();
            break;
        default:
            n_error("Wrong PostEffectParameter Type set!");
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyColorParameters()
{   
    this->balanceShaderVariable->SetFloat4(this->postEffects[Color].current.cast<ColorParams>()->GetColorBalance());
    this->saturationShaderVariable->SetFloat(this->postEffects[Color].current.cast<ColorParams>()->GetColorSaturation());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyDepthOfFieldParameters()
{
    // TODO
    Math::vector dofDistances(  this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFocusDistance(),
                                this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFocusLength(),
                                this->postEffects[DoF].current.cast<DepthOfFieldParams>()->GetFilterSize());
    this->dofVariable->SetFloat4(dofDistances);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyFogParameters()
{
    this->fogColorVariable->SetFloat4(this->postEffects[Fog].current.cast<FogParams>()->GetFogColorAndIntensity());
    // build distances stuff
    Math::float4 fogDistances(  this->postEffects[Fog].current.cast<FogParams>()->GetFogNearDistance(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogFarDistance(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogHeight(),
                                this->postEffects[Fog].current.cast<FogParams>()->GetFogFarDistance() - this->postEffects[Fog].current.cast<FogParams>()->GetFogNearDistance());
    this->fogDistancesVariable->SetFloat4(fogDistances);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyHdrParameters()
{
    this->hdrColorVariable->SetFloat4(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomColor());
    this->hdrScaleVariable->SetFloat(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomIntensity());
    this->hdrThresholdVariable->SetFloat(this->postEffects[Hdr].current.cast<HdrParams>()->GetHdrBloomThreshold());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplyLightParameters()
{
    // do not apply any shader variables,
    // modify globallight instead
    const Ptr<LightParams>& paras = this->postEffects[Light].current.cast<LightParams>();
    this->globalLight->SetColor(paras->GetLightColor());
    this->globalLight->SetTransform(paras->GetLightTransform());
    this->globalLight->SetBackLightColor(paras->GetLightOppositeColor());
    // TODO: rename backlightfactor in application layer and corresponding xml tables into backlightoffset
    this->globalLight->SetBackLightOffset(paras->GetBackLightFactor());
    this->globalLight->SetAmbientLightColor(paras->GetLightAmbientColor());
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::ApplySkyParameters()
{
    // if sky isn't loaded yet, do nothing     
    if (!this->skyLoaded)
    {
        // RETURN !!!!
        return;
    }

    // get textures
    const Ptr<SkyParams>& targetPara = this->postEffects[Sky].target.cast<SkyParams>();
    const Ptr<SkyParams>& currentPara = this->postEffects[Sky].current.cast<SkyParams>();

    this->skyBrightness->SetFloat(currentPara->GetSkyBrightness());
    this->skyContrast->SetFloat(currentPara->GetSkyContrast());
    
    // if blending finished, apply blend texture as current
    if(1.0f - currentPara->GetTextureBlendFactor() <= N_TINY || targetPara == currentPara)
    {        
        // set current as target, reset blend factor and delete target
        currentPara->ResetTextureBlendFactor();
        currentPara->SetSkyTexturePath(targetPara->GetSkyTexturePath());
        this->StopBlending(Sky);

        // set base texture, other one is not needed
        this->skyBaseTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());        
        this->skyBlendTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());        
        this->skyBlendFactor->SetFloat(currentPara->GetTextureBlendFactor());
        this->skyBaseTexture->Apply();
        this->skyBlendTexture->Apply();
        this->skyBlendFactor->Apply();
    }
    else
    {
        // set base and blend texture
        this->skyBlendTexture->SetTexture(this->texturePool[targetPara->GetSkyTexturePath()]->GetTexture());
        this->skyBaseTexture->SetTexture(this->texturePool[currentPara->GetSkyTexturePath()]->GetTexture());
        this->skyBlendFactor->SetFloat(currentPara->GetTextureBlendFactor());
        this->skyBaseTexture->Apply();
        this->skyBlendTexture->Apply();
        this->skyBlendFactor->Apply();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::StartBlending(const Ptr<ParamBase>& target, Timing::Time fadeTime, PostEffectType postEffectType)
{
    //n_printf("PostEffectServer: Starting Blending (%i) at '%f' - fadeTime (%f)\n", (int)postEffectType, this->lastTime, fadeTime);

    // update times
    this->postEffects[postEffectType].blendTime = fadeTime;
    this->postEffects[postEffectType].blendEndTime = this->lastTime + fadeTime;
    this->postEffects[postEffectType].lastTime = this->lastTime;

    // set new target
    this->postEffects[postEffectType].target = target;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::StopBlending(PostEffectType postEffectType)
{
    //n_printf("PostEffectServer: Stopping Blending (%i) at '%f'\n", (int)postEffectType, this->lastTime);

    this->postEffects[(int)postEffectType].target = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::PreloadTexture(const Util::String &resource)
{
    // soft check
    if(!this->texturePool.Contains(resource))
    {
        // append textureextension dependend on platform
        ResourceId fullTexResId = String(resource + NEBULA3_TEXTURE_EXTENSION);
        Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, fullTexResId).downcast<ManagedTexture>();
        this->texturePool.Add(resource, managedTexture);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::UnloadTexture(const Util::String &resource)
{
    // append textureextension dependend on platform
    this->texturePool.Erase(resource);
}

//------------------------------------------------------------------------------
/**
*/
void
PostEffectServer::UnloadTextures()
{
    this->texturePool.Clear();
}

//------------------------------------------------------------------------------
/**
*/
float 
PostEffectServer::ComputeFadeValue(Timing::Time frameTime)
{
    float result = this->currentFadeValue;
    if (this->curFadeMode == FadeOut)
    {
        const float targetValue = 0.0f;
        if (this->fadeTime == 0.0f)
        {
            result = targetValue;
        }
        if (result <= targetValue)
        {
            this->curFadeMode = NoFade;
            return targetValue;
        }
        else
        {
            // always compute time in ratio of full black and full transparent
            float stepSize = (float)frameTime / (float)this->fadeTime; 
            result -= stepSize;
        }
    }
    else
    {
        const float targetValue = 1.0f;
        if (this->fadeTime == 0.0f)
        {
            result = targetValue;
        }
        if (result >= targetValue)
        {
            this->curFadeMode = NoFade;
            return targetValue;
        }
        else
        {
            // always compute time in ratio of full black and full transparent
            float stepSize = (float)frameTime / (float)this->fadeTime; 
            result += stepSize;
        }
    }

    return result;
}
} // namespace PostEffect
