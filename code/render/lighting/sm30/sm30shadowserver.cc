//------------------------------------------------------------------------------
//  sm30shadowserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/sm30/sm30shadowserver.h"
#include "frame/frameserver.h"
#include "coregraphics/transformdevice.h"
#include "models/visresolver.h"
#include "internalgraphics/internalmodelentity.h"
#include "coregraphics/shaderserver.h"     
#include "lighting/internalspotlightentity.h"
#include "math/polar.h"

namespace Lighting
{
__ImplementClass(Lighting::SM30ShadowServer, 'S3SS', Lighting::ShadowServerBase);

using namespace Math;
using namespace Util;
using namespace Frame;
using namespace Resources;
using namespace CoreGraphics;
using namespace InternalGraphics;
using namespace Models;

//------------------------------------------------------------------------------
/**
*/
SM30ShadowServer::SM30ShadowServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SM30ShadowServer::~SM30ShadowServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
SM30ShadowServer::Open()
{
    // call parent class
    ShadowServerBase::Open();

    // load the ShadowBuffer frame shader
    FrameServer* frameServer = FrameServer::Instance();
    this->frameShader = frameServer->LookupFrameShader(ResourceId("esmshadowmapping"));
    n_assert(this->frameShader.isvalid());
    this->finalPostEffect = this->frameShader->GetFramePassBaseByName(ResourceId("Final")).downcast<FramePostEffect>();

    // setup the shadow buffer render target, this is a single
    // render target which contains the shadow buffer data for
    // all shadow casting light sources
    const Ptr<RenderTarget>& origRt = this->frameShader->GetRenderTargetByName(ResourceId("ESMDownscale"));
    SizeT rtWidth = origRt->GetWidth();
    SizeT rtHeight = origRt->GetHeight();
    SizeT resolveWidth = rtWidth * MaxNumShadowLights;
    SizeT resolveHeight = rtHeight;
    PixelFormat::Code pixelFormat = origRt->GetColorBufferFormat();
    
    // create a shadow buffer for up to MaxNumShadowLights local light sources
    this->localLightShadowBuffer = RenderTarget::Create();
    this->localLightShadowBuffer->SetWidth(rtWidth);
    this->localLightShadowBuffer->SetHeight(rtHeight);
    this->localLightShadowBuffer->SetAntiAliasQuality(AntiAliasQuality::None);
    this->localLightShadowBuffer->SetColorBufferFormat(pixelFormat);
    this->localLightShadowBuffer->SetMipMapsEnabled(true);
    this->localLightShadowBuffer->SetResolveTextureResourceId(ResourceId("ShadowBuffer"));
    this->localLightShadowBuffer->SetResolveTextureWidth(resolveWidth);
    this->localLightShadowBuffer->SetResolveTextureHeight(resolveHeight);
    this->localLightShadowBuffer->Setup();
        
    // create a shadow buffer for Parallel-Split-Shadow-Maps (used for the global light source)
    // NOTE: PSSM-ShadowBuffer must be same size as LocalLight-ShadowBuffer
    /*this->pssmShadowBuffer = RenderTarget::Create();
    this->pssmShadowBuffer->SetWidth(rtWidth);
    this->pssmShadowBuffer->SetHeight(rtHeight);
    this->pssmShadowBuffer->SetAntiAliasQuality(AntiAliasQuality::None);
    this->pssmShadowBuffer->AddColorBuffer(pixelFormat);
    this->pssmShadowBuffer->SetMipMapsEnabled(true);
    this->pssmShadowBuffer->SetResolveTextureResourceId(ResourceId("PSSMShadowBuffer"));
    this->pssmShadowBuffer->SetResolveTextureWidth(rtWidth * PSSMUtil::NumSplits);
    this->pssmShadowBuffer->SetResolveTextureHeight(rtHeight);
    this->pssmShadowBuffer->Setup();*/
}

//------------------------------------------------------------------------------
/**
*/
void
SM30ShadowServer::Close()
{
    // release shadow buffers
    this->localLightShadowBuffer->Discard();
    this->localLightShadowBuffer = 0;
    /*this->pssmShadowBuffer->Discard();
    this->pssmShadowBuffer = 0;*/

    // release frame shader
    this->finalPostEffect = 0;
    this->frameShader->Discard();
    this->frameShader = 0;

    // call parent class
    ShadowServerBase::Close();
}

//------------------------------------------------------------------------------
/**
    This method updates the internal shadow buffer render taregets.
*/
void
SM30ShadowServer::UpdateShadowBuffers()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);

    // update local lights shadow buffer
    if (this->localLightEntities.Size() > 0)
    {
        this->UpdateLocalLightShadowBuffers();
    }

    // update global ligth parallel-split-shadow-map shadow buffers
    if (this->globalLightEntity.isvalid())
    {
        //this->UpdatePSSMShadowBuffers();
    }
}

//------------------------------------------------------------------------------
/**
    Update the local lights shadow buffer.
*/
void
SM30ShadowServer::UpdateLocalLightShadowBuffers()
{
    VisResolver* visResolver = VisResolver::Instance();
    TransformDevice* transDev = TransformDevice::Instance();

    // store original view and projection transform
    matrix44 origView = transDev->GetViewTransform();
    matrix44 origProj = transDev->GetProjTransform();

    // for each shadow casting light...
    SizeT numLights = this->localLightEntities.Size();
    if (numLights > MaxNumShadowLights)
    {
        numLights = MaxNumShadowLights;
    }
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < numLights; lightIndex++)
    {
        // render shadow casters in current light volume to shadow buffer
        const Ptr<InternalAbstractLightEntity>& lightEntity = this->localLightEntities[lightIndex];
        transDev->SetViewTransform(lightEntity->GetShadowInvTransform());
        transDev->SetProjTransform(lightEntity->GetShadowProjTransform());   

        // perform visibility resolve for current light
        visResolver->BeginResolve(lightEntity->GetTransform());
        const Array<Ptr<InternalGraphicsEntity> >& visLinks = lightEntity->GetLinks(InternalGraphicsEntity::LightLink);
        IndexT linkIndex;
        for (linkIndex = 0; linkIndex < visLinks.Size(); linkIndex++)
        {
            const Ptr<InternalGraphicsEntity>& curEntity = visLinks[linkIndex];
            n_assert(InternalGraphicsEntityType::Model == curEntity->GetType());
            const Ptr<InternalModelEntity>& modelEntity = curEntity.downcast<InternalModelEntity>();
            visResolver->AttachVisibleModelInstance(modelEntity->GetModelInstance());
        }
        visResolver->EndResolve();

        // prepare shadow buffer render target for rendering and 
        // patch current shadow buffer render target into the frame shader
        Math::rectangle<int> resolveRect;
        resolveRect.left   = lightIndex * this->localLightShadowBuffer->GetWidth();
        resolveRect.right  = resolveRect.left + this->localLightShadowBuffer->GetWidth();
        resolveRect.top    = 0;
        resolveRect.bottom = this->localLightShadowBuffer->GetHeight();
        this->localLightShadowBuffer->SetResolveRect(resolveRect);
        this->finalPostEffect->SetRenderTarget(this->localLightShadowBuffer);

        // render the resolved model node instances into the main shadow buffer
        this->frameShader->Render();

        // patch shadow buffer and shadow buffer uv offset into the light source  
        // uvOffset.xy is offset
        // uvOffset.zw is modulate
        // also moves projection space coords into uv space
        float shadowBufferHoriPixelSize = 1.0f / this->localLightShadowBuffer->GetResolveTextureWidth();
        Math::float4 uvOffset;
        uvOffset.x() = float(lightIndex) / float(MaxNumShadowLights);
        uvOffset.y() = 0.0f;
        uvOffset.z() = (1.0f - shadowBufferHoriPixelSize) / float(MaxNumShadowLights);
        uvOffset.w() = 1.0f;
        lightEntity->SetShadowBufferUvOffsetAndScale(uvOffset);
    }

    // generate mip levels for the shadow buffer
    this->localLightShadowBuffer->GenerateMipLevels();

    // restore original view and projection transforms
    transDev->SetViewTransform(origView);
    transDev->SetProjTransform(origProj);
}

//------------------------------------------------------------------------------
/**
    Update the parallel-split-shadow-map shadow buffers for the
    global light source.
*/
void
SM30ShadowServer::UpdatePSSMShadowBuffers()
{
    n_assert(this->globalLightEntity.isvalid());
    VisResolver* visResolver = VisResolver::Instance();
    TransformDevice* transDev = TransformDevice::Instance();

    // store original view and projection transform
    matrix44 origView = transDev->GetViewTransform();
    matrix44 origProj = transDev->GetProjTransform();

    // compute split view volumes
    this->pssmUtil.SetCameraEntity(this->cameraEntity);
    this->pssmUtil.SetLightDir(this->globalLightEntity->GetLightDirection());
    this->pssmUtil.Compute();

    // render shadow casters for each view volume split
    IndexT splitIndex;
    for (splitIndex = 0; splitIndex < PSSMUtil::NumSplits; splitIndex++)
    {
        // get the view and projection matrices of the current view volume split
        const matrix44& splitLightProjMatrix = this->pssmUtil.GetSplitLightProjTransform(splitIndex);
        const matrix44& splitLightMatrix = this->pssmUtil.GetSplitLightTransform(splitIndex);
        const matrix44& splitProjMatrix = this->pssmUtil.GetSplitProjTransform(splitIndex);
        
        // update transform device with view and projection matrix of current split volume
        transDev->SetViewTransform(splitLightMatrix);
        transDev->SetProjTransform(splitProjMatrix);

        // perform visibility resolve, directional lights don't really have a position
        // thus we're feeding an identity matrix as camera transform
        visResolver->BeginResolve(matrix44::identity());
        const Array<Ptr<InternalGraphicsEntity> >& visLinks = this->globalLightEntity->GetLinks(InternalGraphicsEntity::LightLink);
        IndexT linkIndex;
        for (linkIndex = 0; linkIndex < visLinks.Size(); linkIndex++)
        {
            const Ptr<InternalGraphicsEntity>& curEntity = visLinks[linkIndex];
            n_assert(InternalGraphicsEntityType::Model == curEntity->GetType());

            // test if the current graphics entity falls into the current split volume
            // (FIXME: need to check against extruded shadow bounding box!)
            if (curEntity->GetGlobalBoundingBox().clipstatus(splitLightProjMatrix) != ClipStatus::Outside)
            {
                const Ptr<InternalModelEntity>& modelEntity = curEntity.downcast<InternalModelEntity>();
                visResolver->AttachVisibleModelInstance(modelEntity->GetModelInstance());
            }
        }
        visResolver->EndResolve();

        // prepare shadow buffer render target for rendering and 
        // patch shadow buffer render target into the frame shader
        Math::rectangle<int> resolveRect;
        resolveRect.left   = splitIndex * this->pssmShadowBuffer->GetWidth();
        resolveRect.right  = resolveRect.left + this->pssmShadowBuffer->GetWidth();
        resolveRect.top    = 0;
        resolveRect.bottom = this->pssmShadowBuffer->GetHeight();
        this->pssmShadowBuffer->SetResolveRect(resolveRect);
        this->finalPostEffect->SetRenderTarget(this->pssmShadowBuffer);

        // render the resolved model node instances into the main shadow buffer
        this->frameShader->Render();
    }

    // generate mip levels for PSSM shadow buffer
    this->pssmShadowBuffer->GenerateMipLevels();

    // restore original view and projection transforms
    transDev->SetViewTransform(origView);
    transDev->SetProjTransform(origProj);
    this->pssmUtil.SetCameraEntity(0);
}

//------------------------------------------------------------------------------
/**
    Get raw pointer to array of PSSM split distances.
*/
const float*
SM30ShadowServer::GetPSSMSplitDistances() const
{
    return this->pssmUtil.GetSplitDistances();
}

//------------------------------------------------------------------------------
/**
    Get raw pointer to array of PSSM split LightProjTransform matrices.
*/
const Math::matrix44*
SM30ShadowServer::GetPSSMSplitLightProjTransforms() const
{
    return this->pssmUtil.GetSplitLightProjTransforms();
}

//------------------------------------------------------------------------------
/**
*/
void 
SM30ShadowServer::SortLights()
{
    // @todo: sort without dictionary 
    Util::Dictionary<float, Ptr<InternalAbstractLightEntity> > sortedArray;
    sortedArray.BeginBulkAdd();
    IndexT i;
    for (i = 0; i < this->localLightEntities.Size(); ++i)
    {
        const matrix44& lightTrans = this->localLightEntities[i]->GetTransform();                                                                                                            
        vector vec2Poi = (float4)this->pointOfInterest - lightTrans.get_position();
        float distance = vec2Poi.length();
        float range = lightTrans.get_zaxis().length();
        float attenuation = n_saturate(1.0f - (distance / range));
        if (this->localLightEntities[i]->IsA(InternalSpotLightEntity::RTTI))
        {
            // consider spotlight frustum            
            if (!matrix44::ispointinside(this->pointOfInterest, this->localLightEntities[i]->GetInvLightProjTransform()))
            { 
                attenuation = 0.0f;
            }
        }
        attenuation = 1.0f - attenuation;
        sortedArray.Add(attenuation, this->localLightEntities[i]);
    }
    sortedArray.EndBulkAdd();     

    // patch positions if maxnumshadowlights  = 1
    if (sortedArray.Size() > MaxNumShadowLights)
    {   
        if (MaxNumShadowLights == 1)
        {   
            IndexT lastShadowCastingLight = MaxNumShadowLights - 1;
            const Ptr<InternalAbstractLightEntity>& lightEntity = sortedArray.ValueAtIndex(lastShadowCastingLight);
            const matrix44& shadowLightTransform = lightEntity->GetTransform();
            float range0 = shadowLightTransform.get_zaxis().length();
            vector vecPoiLight = shadowLightTransform.get_position() - this->pointOfInterest;
            vector normedPoiLight = float4::normalize(vecPoiLight);

            point interpolatedPos = shadowLightTransform.get_position();
            polar lightDir(shadowLightTransform.get_zaxis());
            float interpolatedXRot = lightDir.theta;
            float interpolatedYRot = lightDir.rho;
            SizeT numLightsInRange = 0;
            IndexT i;
            for (i = MaxNumShadowLights; i < sortedArray.Size(); ++i)
            {
                const Ptr<InternalAbstractLightEntity>& curLightEntity = sortedArray.ValueAtIndex(i);
                const matrix44& curTransform = curLightEntity->GetTransform();            
                vector distVec = shadowLightTransform.get_position() - curTransform.get_position();
                float range1 = curTransform.get_zaxis().length();
                bool lerpLightTransform = (distVec.length() - range0 - range1 < 0);
                if (curLightEntity->IsA(InternalSpotLightEntity::RTTI))
                {
                    // consider spotlight frustum            
                    if (!matrix44::ispointinside(shadowLightTransform.get_position(), curLightEntity->GetInvLightProjTransform()))
                    {
                        lerpLightTransform = false;
                    }
                }
                if (lerpLightTransform)
                {                      
                    float dotPoi = float4::dot3(normedPoiLight, float4::normalize(distVec));
                    if (dotPoi > 0)
                    {                
                        // project poi - shadowlight to shadowlight next light vector
                        float len0 = distVec.length();
                        float projLen = float4::dot3(vecPoiLight, distVec);
                        projLen /= (len0 * len0);
                        // do a smoothstep
                        float lerpFactor = n_smoothstep(0, 1, projLen);
                        interpolatedPos = float4::lerp(interpolatedPos, curTransform.get_position(), lerpFactor);                                                      

                        polar curDir(curTransform.get_zaxis());
                        interpolatedXRot = n_lerp(interpolatedXRot, curDir.theta, lerpFactor);
                        interpolatedYRot = n_lerp(interpolatedYRot, curDir.rho, lerpFactor);
                    }                
                }            
            }       
            interpolatedPos.w() = 1.0f;
            matrix44 rotM = matrix44::rotationyawpitchroll(interpolatedYRot, interpolatedXRot - (N_PI * 0.5f), 0.0);
            matrix44 interpolatedTransform = matrix44::scaling(shadowLightTransform.get_xaxis().length(),
                shadowLightTransform.get_yaxis().length(),
                shadowLightTransform.get_zaxis().length());        
            interpolatedTransform = matrix44::multiply(interpolatedTransform, rotM);
            interpolatedTransform.set_position(interpolatedPos);
            lightEntity->SetShadowTransform(interpolatedTransform);
        }
        // more than one casting lights allowed, use shadow fading
        else
        {
            IndexT lastShadowCastingLight = MaxNumShadowLights - 1;
            IndexT firstNotAllowedShadowCastingLight = MaxNumShadowLights;
            // fade out shadow of casting light depended on next shadowcasting light
            float att0 = 1 - sortedArray.KeyAtIndex(lastShadowCastingLight);
            float att1 = 1 - sortedArray.KeyAtIndex(firstNotAllowedShadowCastingLight);

            float shadowIntensity = n_saturate(4.0f * att0);
            sortedArray.ValueAtIndex(lastShadowCastingLight)->SetShadowIntensity(shadowIntensity);                                                                                                                
        }
    }
    // assign sorted array
    this->localLightEntities = sortedArray.ValuesAsArray();
}
} // namespace Lighting
