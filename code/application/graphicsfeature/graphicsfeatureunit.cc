//------------------------------------------------------------------------------
//  game/graphicsfeature.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "graphicsfeature/graphicsfeatureproperties.h"
#include "basegamefeature/loader/loaderserver.h"
#include "visibility/visibilitysystems/visibilityquadtree.h"
#include "internalgraphics/internalview.h"  // FIXME! should have to use InternalGraphics!
#include "game/gameserver.h"
#include "debugrender/debugrenderprotocol.h"
#include "threading/thread.h"
#include "input/keyboard.h"
#include "basegametiming/gametimesource.h"   
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/animresource.h"
#include "coreanimation/managedanimresource.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeatureUnit, 'FGFX' , Game::FeatureUnit);
__ImplementSingleton(GraphicsFeatureUnit);

using namespace CoreGraphics;
using namespace Graphics;
using namespace Input;
using namespace Resources;
using namespace Threading;
using namespace Animation;
using namespace Util;
using namespace Visibility;

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::GraphicsFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GraphicsFeatureUnit::~GraphicsFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnActivate()
{
    n_assert(!this->IsActive());
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    
    // call parent class
    FeatureUnit::OnActivate();

    // setup the graphics subsystem
    this->graphicsInterface = GraphicsInterface::Create();
    this->graphicsInterface->Open();
    
    this->display = Display::Create();  
    // setup default resource mappers
    this->OnSetupResourceMappers();
    this->OnConfigureDisplay();
    this->display->Open();                                   

    // setup the frame-sync timer
    this->frameSyncTimer = FrameSync::FrameSyncTimer::Create();
    this->frameSyncTimer->Setup();

    this->graphicsServer = GraphicsServer::Create();
    this->graphicsServer->Open();
    this->postEffectManager = PostEffect::PostEffectManager::Create();
    this->AttachManager(this->postEffectManager.cast<Game::Manager>());

    this->SetupDefaultGraphicsWorld();

	this->debugShapeRenderer = Debug::DebugShapeRenderer::Create();
    this->debugTextRenderer = Debug::DebugTextRenderer::Create();

    // setup input subsystem
    this->inputServer = InputServer::Create();
    this->inputServer->Open();

    this->resManager = ResourceManager::Create();
    ResourceManager::Instance()->Open();

    // append standard managers
    this->attachmentManager = GraphicsFeature::AttachmentManager::Create();
    this->AttachManager(this->attachmentManager.cast<Game::Manager>());

    _setup_timer(WaitForGraphicsThreadTime)
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnDeactivate()
{
    n_assert(this->IsActive());

    if (this->defaultStage.isvalid())
    {
        this->DiscardDefaultGraphicsWorld();    
    }
    
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    n_assert(!this->defaultCamera.isvalid());

    // remove standard managers
    this->RemoveManager(this->postEffectManager.upcast<Game::Manager>());
    this->postEffectManager = 0;

    this->RemoveManager(this->attachmentManager.upcast<Game::Manager>());
    this->attachmentManager = 0;

    // shutdown frame-sync timer singleton
    this->frameSyncTimer->Discard();
    this->frameSyncTimer = 0;

    // shutdown Nebula3
    this->inputServer->Close();
    this->inputServer = 0;
    this->graphicsServer->Close();
    this->graphicsServer = 0;
    this->display->Close();
    this->display = 0;
    this->graphicsInterface->Close();
    this->graphicsInterface = 0;
	this->debugShapeRenderer = 0;
    this->debugTextRenderer = 0;
    this->resManager->Close();
    this->resManager = 0;
        
    _discard_timer(WaitForGraphicsThreadTime)

    FeatureUnit::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnStart()
{
    // reset input events, e.g. needed after new gamestate is set
    this->inputServer->Reset();

    Game::FeatureUnit::OnStart();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::SetupDefaultGraphicsWorld()
{
    n_assert(this->IsActive());
    n_assert(!this->defaultStage.isvalid());
    n_assert(!this->defaultView.isvalid());
    n_assert(!this->defaultCamera.isvalid());

    ResourceId defaultStageName("DefaultStage");
    ResourceId defaultViewName("DefaultView");
    
    // setup a default stage and view
    
    // attach visibility systems to checker
    Ptr<Visibility::VisibilityQuadtree> visSystem = Visibility::VisibilityQuadtree::Create();
    visSystem->SetQuadTreeSettings(4, Math::bbox(Math::point(0,0,0), Math::vector(500.0f, 100.0f, 500.0f)));
    Util::Array<Ptr<VisibilitySystemBase> > visSystems;
    visSystems.Append(visSystem.cast<VisibilitySystemBase>());
    this->defaultStage = this->graphicsServer->CreateStage(defaultStageName, visSystems);
        
    this->defaultView = this->graphicsServer->CreateView(InternalGraphics::InternalView::RTTI,
                                                         defaultViewName,
                                                         defaultStageName,
                                                         ResourceId(NEBULA3_DEFAULT_FRAMESHADER_NAME),
                                                         true);

    // setup a default camera
    this->defaultCamera = CameraEntity::Create();
    this->defaultStage->AttachEntity(this->defaultCamera.cast<GraphicsEntity>());
    this->defaultView->SetCameraEntity(this->defaultCamera);

    // reset posteffect stuff
    this->postEffectManager->ResetPostEffectSystem();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::DiscardDefaultGraphicsWorld()
{
    n_assert(this->defaultStage.isvalid());
    n_assert(this->defaultView.isvalid());
    n_assert(this->defaultCamera.isvalid());

    // cleanup attachmentmanager
    this->attachmentManager->ClearAttachments();

    this->defaultStage->RemoveEntity(this->defaultCamera.cast<GraphicsEntity>());
    this->defaultCamera = 0;
    this->graphicsServer->DiscardAllStages();
    this->graphicsServer->DiscardAllViews();
    this->defaultView = 0;
    this->defaultStage = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnBeginFrame()
{  
    this->inputServer->BeginFrame();
    this->inputServer->OnFrame();
    
    _start_timer(WaitForGraphicsThreadTime)
    // wait till graphics thread has finished processing messages
    GraphicsInterface::Instance()->GameThreadWaitForFrameSync();
    _stop_timer(WaitForGraphicsThreadTime)

    Game::FeatureUnit::OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnFrame()
{    
	this->debugShapeRenderer->OnFrame();
    this->debugTextRenderer->OnFrame();
    GraphicsServer::Instance()->OnFrame();
    
    Game::FeatureUnit::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnEndFrame()
{
    // send batched messages for the graphic subsystem
    GraphicsInterface::Instance()->FlushBatchedMessages();

    // handle quit requested
    if (this->inputServer->IsQuitRequested())
    {
        Game::GameServer::Instance()->SetQuitRequested();
    }
    this->inputServer->EndFrame();

    // in windowed mode, give other apps time slice
    if (!this->display->Settings().IsFullscreen())
    {
        Core::SysFunc::Sleep(0.0);
    }

    Game::FeatureUnit::OnEndFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::StartRenderDebug()
{   
    Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
    renderDebugMsg->SetThreadId(Thread::GetMyThreadId());
    renderDebugMsg->SetEnableDebugRendering(true);
    Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::StopRenderDebug()
{   
    Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
    renderDebugMsg->SetThreadId(Thread::GetMyThreadId());
    renderDebugMsg->SetEnableDebugRendering(false);    
    Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsFeatureUnit::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->display->AdapterExists(adapter))
        {
            this->display->Settings().SetAdapter(adapter);
        }
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    if (this->args.HasArg("-ratio"))
    {
        displayMode.SetAspectRatio(this->args.GetFloat("-ratio"));
    }
    this->display->Settings().DisplayMode() = displayMode;
    this->display->Settings().SetFullscreen(this->args.GetBoolFlag("-fullscreen"));
    this->display->Settings().SetAlwaysOnTop(this->args.GetBoolFlag("-alwaysontop"));
    this->display->Settings().SetVerticalSyncEnabled(this->args.GetBoolFlag("-vsync"));
    
    if (this->args.HasArg("-aa"))
    {
        this->display->Settings().SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
}

//------------------------------------------------------------------------------
/**
    This method is called after all entities are loaded.
*/
void 
GraphicsFeatureUnit::OnEntitiesLoaded()
{
}


//------------------------------------------------------------------------------
/**
*/
void 
GraphicsFeatureUnit::OnSetupResourceMappers()
{
    Array<Ptr<ResourceMapper> > resourceMappers;

    // setup resource mapper for textures
    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId(NEBULA3_PLACEHOLDER_TEXTURENAME));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(CoreGraphics::StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    resourceMappers.Append(texMapper.upcast<ResourceMapper>());

    // setup resource mapper for meshes
    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId(NEBULA3_PLACEHOLDER_MESHNAME));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(CoreGraphics::StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    resourceMappers.Append(meshMapper.upcast<ResourceMapper>());

    // setup resource mapper for animations
    // FIXME: should be configurable!
    Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
    animMapper->SetResourceClass(CoreAnimation::AnimResource::RTTI);
    animMapper->SetResourceLoaderClass(CoreAnimation::StreamAnimationLoader::RTTI);
    animMapper->SetManagedResourceClass(CoreAnimation::ManagedAnimResource::RTTI);
    resourceMappers.Append(animMapper.upcast<ResourceMapper>());

    this->display->SetResourceMappers(resourceMappers);
}
} // namespace Game
