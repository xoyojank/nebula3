//------------------------------------------------------------------------------
//  graphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicshandler.h"
#include "graphics/graphicsinterface.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/base/shaperendererbase.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "debugrender/debugshaperenderer.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"
#include "internalgraphics/debug/graphicspagehandler.h"
#include "visibility/visibilityprotocol.h"
#include "characters/characterserver.h"
#if __WIN32__                                                
#include "resources/streaming/streamingtexturepagehandler.h"
#include "render/resources/streaming/poolresourcemapper.h"
#endif
#if __PS3__
#include "coregraphics/ps3/ps3dynamicgeometryserver.h"
#endif

namespace Graphics
{
__ImplementClass(Graphics::GraphicsHandler, 'GHDL', Messaging::Handler);

using namespace IO;
using namespace CoreGraphics;
using namespace CoreAnimation;
using namespace Resources;
using namespace Math;
using namespace Models;
using namespace InternalGraphics;
using namespace Lighting;
using namespace Frame;
using namespace Interface;
using namespace Util;
using namespace Debug;
using namespace Messaging;
using namespace Animation;
using namespace Timing;
using namespace Characters;
using namespace FrameSync;
using namespace Particles;
using namespace Visibility;


//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::GraphicsHandler() :
    isGraphicsRuntimeValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsHandler::~GraphicsHandler()
{
}

//------------------------------------------------------------------------------
/**
    Waits for all resources to be loaded.
    THIS METHOD IS CALLED IN THE MAIN THREAD CONTEXT!
*/
void
GraphicsHandler::WaitForPendingResources()
{
    // setting this flag causes the DoWork-loop to check for pending
    // resources and to set the pending resource event when all
    // pending resources have been loaded
    this->checkPendingResourcesFlag.Set();

    // now wait for the DoWork-loop to signal the event
    this->pendingResourcesEvent.Wait();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->isGraphicsRuntimeValid);
    InterfaceHandlerBase::Open();

    // setup frame-synced timer
    this->frameSyncTimer = FrameSyncTimer::Create();
    this->frameSyncTimer->Setup();

    // setup core runtime
    this->ioServer = IO::IoServer::Create();

#if __NEBULA3_HTTP_FILESYSTEM__
    // setup http client registry for shared HTTP connections
    this->httpClientRegistry = Http::HttpClientRegistry::Create();
    this->httpClientRegistry->Setup();
#endif

#if __NEBULA3_HTTP__
    // setup http page handlers
    this->httpServerProxy = Http::HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::DisplayPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::MeshPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::ShaderPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::TexturePageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::GraphicsPageHandler::Create());
#if __WIN32__
    this->httpServerProxy->AttachRequestHandler(Debug::StreamingTexturePageHandler::Create());
#endif
#endif

    // setup the required objects, but do not open them, this will
    // happen at a later time when the SetupGrapics message is received
    this->renderDevice = RenderDevice::Create();
    this->displayDevice = DisplayDevice::Create();
    this->transformDevice = TransformDevice::Create();
    this->shaderServer = ShaderServer::Create();
    this->shapeRenderer = ShapeRenderer::Create();
    this->textRenderer = TextRenderer::Create();
    this->vertexLayoutServer = VertexLayoutServer::Create();
    this->resourceManager = ResourceManager::Create();
    this->modelServer = ModelServer::Create();
    this->graphicsServer = InternalGraphicsServer::Create();
    this->lightServer = LightServer::Create();
    this->shadowServer = ShadowServer::Create();
    this->frameServer = FrameServer::Create();
    this->attachmentServer = AttachmentServer::Create();
    this->animEventServer = Animation::AnimEventServer::Create();
    this->characterServer = Characters::CharacterServer::Create();
    this->mouseRenderDevice = MouseRenderDevice::Create();
    this->particleServer = ParticleServer::Create();

    // setup debug timers and counters
    _setup_timer(GraphicsFrameTime);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::Close()
{
    n_assert(this->IsOpen());

    _discard_timer(GraphicsFrameTime);
    
    if (this->isGraphicsRuntimeValid)
    {
        this->ShutdownGraphicsRuntime();
    }

    this->particleServer = 0;
    this->mouseRenderDevice = 0;
    this->characterServer = 0;
    this->frameServer = 0;
    this->shadowServer = 0;
    this->lightServer = 0;
    this->graphicsServer = 0;
    this->modelServer = 0;
    this->resourceManager = 0;
    this->vertexLayoutServer = 0;
    this->shapeRenderer = 0;
    this->shaderServer = 0;
    this->transformDevice = 0;
    this->displayDevice = 0;
    this->renderDevice = 0;
    this->textRenderer = 0;
    this->attachmentServer = 0;
    this->animEventServer = 0;

#if __NEBULA3_HTTP__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
#endif
#if __NEBULA3_HTTP_FILESYSTEM__
    this->httpClientRegistry->Discard();
    this->httpClientRegistry = 0;
#endif 

    this->ioServer = 0;

    this->frameSyncTimer->Discard();
    this->frameSyncTimer = 0;

    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
    Setup the graphics runtime, this method is called when the
    SetupGraphics message is received from the main thread. The method
    expects that the DisplayDevice has been configured with the 
    desired display settings.
*/
void
GraphicsHandler::SetupGraphicsRuntime(const Ptr<SetupGraphics>& msg)
{
    n_assert(!this->isGraphicsRuntimeValid);
    n_assert(!this->displayDevice->IsOpen());
    n_assert(!this->renderDevice->IsOpen());
    n_assert(!this->transformDevice->IsOpen());
    n_assert(!this->shaderServer->IsOpen());
    n_assert(!this->shapeRenderer->IsOpen());
    n_assert(!this->vertexLayoutServer->IsOpen());
    n_assert(!this->resourceManager->IsOpen());
    n_assert(!this->modelServer->IsOpen());
    n_assert(!this->graphicsServer->IsOpen());
    n_assert(!this->lightServer->IsOpen());
    n_assert(!this->shadowServer->IsOpen());
    n_assert(!this->frameServer->IsOpen());
    n_assert(!this->attachmentServer->IsOpen());
    n_assert(!this->characterServer->IsValid());
    
    if (!this->displayDevice->Open())
    {
        n_error("GraphicsHandler: failed to open display device!");
    }
    #if __WII__
    this->displayDevice->GetScreenShotHelper().EnableScreenShotMode();
    #endif
    
    if (!this->renderDevice->Open())
    {
        n_error("GraphicsHandler: failed to open render device!");
    }
    this->vertexLayoutServer->Open();
    this->shaderServer->Open();
    this->transformDevice->Open();
    this->shapeRenderer->Open();
    this->resourceManager->Open();
    this->frameServer->Open();
    this->textRenderer->Open();
    this->characterServer->Setup();
    this->mouseRenderDevice->Setup();

    // setup resource mappers
    IndexT i;
    for (i = 0; i < msg->GetResourceMappers().Size(); i++)
    {
        this->resourceManager->AttachMapper(msg->GetResourceMappers()[i]);
#if __WIN32__
        // HACK for debugging of resourceManagement (soon removed)
        if (msg->GetResourceMappers()[i]->IsA(PoolResourceMapper::RTTI))
        {
            this->resourceManager->textureMapper = msg->GetResourceMappers()[i].downcast<PoolResourceMapper>();
        }
#endif
    }

    this->modelServer->Open();
    this->graphicsServer->Open();
    this->lightServer->Open();
    this->shadowServer->Open();    
    this->attachmentServer->Open();
    this->particleServer->Open();  

    // HACK - pin placeholders and system stuff so they will not be automatically
    // removed or replaced on LOD-management
    this->resourceManager->AutoManageManagedResource("tex:system/black.dds", false);
    this->resourceManager->AutoManageManagedResource("tex:system/white.dds", false);
    this->resourceManager->AutoManageManagedResource("tex:system/nobump.dds", false);
    this->resourceManager->AutoManageManagedResource("tex:lighting/lightcones.dds", false);
    this->resourceManager->AutoManageManagedResource("tex:system/placeholder.dds", false);

    this->isGraphicsRuntimeValid = true;
}

//------------------------------------------------------------------------------
/**
    Shutdown the graphics runtime, this is called from the Close() method.
*/
void
GraphicsHandler::ShutdownGraphicsRuntime()
{
    n_assert(this->isGraphicsRuntimeValid);
    this->isGraphicsRuntimeValid = false;

    this->particleServer->Close();      
    this->mouseRenderDevice->Discard();
    this->characterServer->Discard();
    this->textRenderer->Close();
    this->shadowServer->Close();
    this->lightServer->Close();
    this->graphicsServer->Close();
    this->modelServer->Close();
    this->resourceManager->Close();
    this->frameServer->Close();
    this->shapeRenderer->Close();
    this->shaderServer->Close();
    this->vertexLayoutServer->Close();
    this->renderDevice->Close();
    this->displayDevice->Close();
    this->attachmentServer->Close();
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());

    if (msg->IsA(Graphics::GraphicsEntityMessage::RTTI))
    {
        // handle graphics entity message
        Ptr<InternalGraphicsEntity> entity = msg.cast<GraphicsEntityMessage>()->GetObjectRef()->Ref<InternalGraphicsEntity>();
        entity->HandleMessage(msg);
        return true;
    }
    else if (msg->CheckId(SetupGraphics::Id))
    {
        this->OnSetupGraphics(msg.cast<SetupGraphics>());
        return true;
    }

    __StaticHandle(CreateModelEntity);
    __StaticHandle(CreateCameraEntity);
    __StaticHandle(CreateGlobalLightEntity);
    __StaticHandle(CreatePointLightEntity);
    __StaticHandle(CreateSpotLightEntity);
    __StaticHandle(AdapterExists);
    __StaticHandle(GetAvailableDisplayModes);
    __StaticHandle(GetCurrentAdapterDisplayMode);
	__StaticHandle(AdjustDisplaySize);
    __StaticHandle(GetAdapterInfo);
    __StaticHandle(AttachDisplayEventHandler);
    __StaticHandle(RemoveDisplayEventHandler);
    __StaticHandle(AttachRenderEventHandler);
    __StaticHandle(RemoveRenderEventHandler);
    __StaticHandle(CreateGraphicsStage);
    __StaticHandle(DiscardGraphicsStage);
    __StaticHandle(CreateGraphicsView);
    __StaticHandle(DiscardGraphicsView);
    __StaticHandle(SetDefaultGraphicsView);
    __StaticHandle(UpdateSharedShaderVariable);
    __StaticHandle(RegisterAnimEventHandler);
    __StaticHandle(UnregisterAnimEventHandler);
    __StaticHandle(PreloadMousePointerTextures);
    __StaticHandle(UpdateMousePointers);
    __StaticHandle(RegisterRTPlugin);
    __StaticHandle(UnregisterRTPlugin);
    __StaticHandle(HoldSharedResources);
    __StaticHandle(ReleaseSharedResources);
    __StaticHandle(SetTimeFactor);
    __StaticHandle(SetShadowPointOfInterest);
    __StaticHandle(CreateVisibilityCluster);
    __StaticHandle(CreateVisibilityBoxes);

    // unhandled message
    return false;
}

//------------------------------------------------------------------------------
/**
    This is the per-frame method which implements the asynchronous render-loop.
*/
void
GraphicsHandler::DoWork()
{
    if (this->isGraphicsRuntimeValid)
    {
        _start_timer(GraphicsFrameTime);
        n_assert(this->IsOpen());

#if __PS3__
        PS3::PS3DynamicGeometryServer::Instance()->Flip();
#endif

        // debug render
        // n_printf("--> graphics time: %f\n", this->frameSyncTimer->GetTime());

        // need to check for asynchronous pending resources?
        if (this->checkPendingResourcesFlag.Test())
        {            
            if (ResourceManager::Instance()->CheckPendingResources())
            {
                this->checkPendingResourcesFlag.Clear();
                this->pendingResourcesEvent.Signal();
            }
        }

        // FIXME: handle anim events
        this->animEventServer->OnFrame(this->frameSyncTimer->GetScaledTime());
      
        // finally, render the frame
        this->graphicsServer->OnFrame(this->frameSyncTimer->GetScaledTime(), this->frameSyncTimer->GetTimeFactor());
       
        _stop_timer(GraphicsFrameTime);
    }

#if __NEBULA3_HTTP__
    // allow render-thread HttpRequests to be processed
    this->httpServerProxy->HandlePendingRequests();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsHandler::OnSetupGraphics(const Ptr<SetupGraphics>& msg)
{
    n_assert(!this->isGraphicsRuntimeValid);

    // configure the display device and setup the graphics runtime
    DisplayDevice* disp = this->displayDevice;
    disp->SetAdapter(msg->GetAdapter());
    disp->SetDisplayMode(msg->GetDisplayMode());
    disp->SetAntiAliasQuality(msg->GetAntiAliasQuality());
    disp->SetFullscreen(msg->GetFullscreen());
	disp->SetAutoAdjustSize(msg->GetAutoAdjustSize());
    disp->SetDisplayModeSwitchEnabled(msg->GetDisplayModeSwitchEnabled());
    disp->SetTripleBufferingEnabled(msg->GetTripleBufferingEnabled());
    disp->SetAlwaysOnTop(msg->GetAlwaysOnTop());
    disp->SetVerticalSyncEnabled(msg->GetVerticalSyncEnabled());
    disp->SetIconName(msg->GetIconName());
    disp->SetWindowTitle(msg->GetWindowTitle());
    disp->SetParentWindow(msg->GetParentWindow());
    this->SetupGraphicsRuntime(msg);

    msg->SetActualDisplayMode(disp->GetDisplayMode());
    msg->SetActualAdapter(disp->GetAdapter());
    msg->SetActualFullscreen(disp->IsFullscreen());
}

} // namespace Graphics

