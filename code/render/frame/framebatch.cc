//------------------------------------------------------------------------------
//  framebatch.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framebatch.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/textrenderer.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/mouserenderdevice.h"
#include "models/visresolver.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"
#include "internalgraphics/internalmodelentity.h"
#include "framesync/framesynctimer.h"
#include "lighting/lightserver.h"
#include "rendermodules/rt/rtpluginregistry.h"
#include "coregraphics/shadersemantics.h"

namespace Frame
{
__ImplementClass(Frame::FrameBatch, 'FBTH', Core::RefCounted);

using namespace InternalGraphics;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace Lighting;
using namespace FrameSync;

//------------------------------------------------------------------------------
/**
*/
FrameBatch::FrameBatch() :
    batchType(BatchType::InvalidBatchType),
    nodeFilter(ModelNodeType::InvalidModelNodeType),
    lightingMode(LightingMode::None),
    sortingMode(SortingMode::None),
    shaderFeatures(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FrameBatch::~FrameBatch()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(this->shaderVariables.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    this->shaderVariables.Clear();

    _discard_timer(this->debugTimer);
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::Render()
{
    RenderDevice* renderDevice = RenderDevice::Instance();

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render the batch
    renderDevice->BeginBatch(this->batchType, this->shader);
    this->RenderBatch();
    renderDevice->EndBatch();
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::RenderBatch()
{
    // at first call plugins 
    const Ptr<FrameBatch> batchPtr(this);
    RenderModules::RTPluginRegistry* rtPluginRegistry = RenderModules::RTPluginRegistry::Instance();
    rtPluginRegistry->OnRenderFrameBatch(batchPtr);

    // now do usual render stuff
    ShaderServer* shaderServer = ShaderServer::Instance();
    VisResolver* visResolver = VisResolver::Instance();      
    LightServer* lightServer = LightServer::Instance(); 

    _start_timer(this->debugTimer);
    // handle special cases
    if (BatchType::UI == this->batchType)
    {
        // ui batch will be handled with a RTplugin
        // thats, why this is empty
    }
    else if (BatchType::WiiHBM == this->batchType || BatchType::WiiPanel == this->batchType)
    {
        // wii home menu batch will be handled with a RTplugin
        // and is only available for the wii platform
        #ifndef __WII__
        n_error("Batchtype %s only available on Wii!", BatchType::ToString(this->batchType).AsCharPtr());
        #endif
    }
    else if (BatchType::Shapes == this->batchType)
    {
        ShapeRenderer::Instance()->DrawShapes();		
    }
    else if (BatchType::Text == this->batchType)
    {
        TextRenderer::Instance()->DrawTextElements();
    }
    else if (BatchType::ResolveDepthBuffer == this->batchType)
    {
        // resolve depth buffer to texture
        RenderDevice* renderDevice = RenderDevice::Instance();
        n_assert(renderDevice->HasPassRenderTarget());
        renderDevice->GetPassRenderTarget()->ResolveDepthBuffer();
    }
    else if (BatchType::MousePointers == this->batchType)
    {
        MouseRenderDevice::Instance()->RenderPointers();
    }
    else if (BatchType::Lights == this->batchType)
    {
        LightServer::Instance()->RenderLights();
    }
    else
    {
		#ifdef __WII__
        // FIXME: this is dirty!
    	GXSetCullMode(GX_CULL_FRONT);
		#endif

        // get current frame index from graphics server
        IndexT frameIndex = FrameSyncTimer::Instance()->GetFrameCount();

        // default case: render models...
        const Array<Ptr<Model> >& models = visResolver->GetVisibleModels(this->nodeFilter);
        IndexT modelIndex;
        for (modelIndex = 0; modelIndex < models.Size(); modelIndex++)
        {
            FRAME_LOG("      FrameBatch::RenderBatch() model: %s", models[modelIndex]->GetResourceId().Value());

            // for each visible model node of the model...
            const Array<Ptr<ModelNode> >& modelNodes = visResolver->GetVisibleModelNodes(this->nodeFilter, models[modelIndex]);
            IndexT modelNodeIndex;  
            for (modelNodeIndex = 0; modelNodeIndex < modelNodes.Size(); modelNodeIndex++)
            {
                // apply render state which is shared by all instances
                shaderServer->ResetFeatureBits();
                shaderServer->SetFeatureBits(this->shaderFeatures);
                const Ptr<ModelNode>& modelNode = modelNodes[modelNodeIndex];            
                modelNode->ApplySharedState(frameIndex);

                FRAME_LOG("        FrameBatch::RenderBatch() node: %s", modelNode->GetName().Value());

                // if lighting mode is Off, we can render all node instances with the same shader
                const Ptr<ShaderInstance>& shaderInst = shaderServer->GetActiveShaderInstance();
                if (LightingMode::None == this->lightingMode)
                {
                    shaderInst->SelectActiveVariation(shaderServer->GetFeatureBits());
                    SizeT numPasses = shaderInst->Begin();
                    n_assert(1 == numPasses);
                    shaderInst->BeginPass(0);
                }

                // render instances
                const Array<Ptr<ModelNodeInstance> >& nodeInstances = visResolver->GetVisibleModelNodeInstances(this->nodeFilter, modelNode);
                IndexT nodeInstIndex;
                for (nodeInstIndex = 0; nodeInstIndex < nodeInstances.Size(); nodeInstIndex++)
                {
                    const Ptr<ModelNodeInstance>& nodeInstance = nodeInstances[nodeInstIndex];

                    // if single-pass lighting is enabled, we need to setup the lighting 
                    // shader states
                    // FIXME: This may set a new shader variation for every node instance
                    // which is expensive! Would be better to sort node instances by number
                    // of active lights!!!
                    if (LightingMode::SinglePass == this->lightingMode)
                    {
                        // setup lighting render states
                        // NOTE: this may change the shader feature bit mask which may select
                        // a different shader variation per entity
                        const Ptr<InternalGraphics::InternalModelEntity>& modelEntity = nodeInstance->GetModelInstance()->GetModelEntity();
                        lightServer->ApplyModelEntityLights(modelEntity);
                        shaderInst->SelectActiveVariation(shaderServer->GetFeatureBits());
                        SizeT numPasses = shaderInst->Begin();
                        n_assert(1 == numPasses);
                        shaderInst->BeginPass(0);
                    }

                    // render the node instance
                    nodeInstance->ApplyState();
                    // get object id and apply object id 
                #if !__WII__
                    shaderServer->ApplyObjectId(nodeInstance->GetModelNodeInstanceIndex());
                #endif
                    shaderInst->Commit();

                #if NEBULA3_ENABLE_PROFILING
                    nodeInstance->StartDebugTimer();
                #endif  
                    nodeInstance->Render();
                #if NEBULA3_ENABLE_PROFILING
                    nodeInstance->StopDebugTimer();
                #endif  
                    if (LightingMode::SinglePass == this->lightingMode)
                    {
                        shaderInst->EndPass();
                        shaderInst->End();
                    }
                }

                if (LightingMode::None == this->lightingMode)
                {
                    shaderInst->EndPass();
                    shaderInst->End();
                }                
            }
        }
    }
    _stop_timer(this->debugTimer);
}

#if NEBULA3_ENABLE_PROFILING
//------------------------------------------------------------------------------
/**
*/
void 
FrameBatch::SetBatchDebugTimer(const Util::String& name)
{
    this->debugTimer = Debug::DebugTimer::Create();
    this->debugTimer->Setup(name);
}
#endif
} // namespace Frame
