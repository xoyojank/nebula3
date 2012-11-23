
//------------------------------------------------------------------------------
//  texturestreamingapplication.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tests/texturestreamingtest_win32/texturestreamingapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/animresource.h"
#include "coreanimation/managedanimresource.h"
#include "resources/streaming/poolresourcemapper.h"
#include "resources/streaming/texturestreamer.h"
#include "resources/streaming/texturecreator.h"
#include "io/uri.h"
#include "io/stream.h"
#include "io/xmlreader.h"
#include "io/ioserver.h"
#include "render/resources/streaming/textureinfo.h"
#include "render/resources/streaming/resourcescheduler.h"
#include "render/resources/streaming/texturepoolmapperscheduler.h"
#include "framecapture/framecapturerendermodule.h"
#include "framecapture/framecaptureprotocol.h"
#include "resources/streaming/poolscheduler.h"
#include "io/ioserver.h"

namespace Tests
{
    using namespace CoreGraphics;
    using namespace Graphics;
    using namespace Math;
    using namespace Util;
    using namespace Resources;
    using namespace Timing;
    using namespace Debug;
    using namespace Input;
    using namespace FrameCapture;

//------------------------------------------------------------------------------
/**
*/
TextureStreamingApplication::TextureStreamingApplication() : 
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false),
    rotX(-225),
    direction(1.0),
    shadowConstants(100.0f, 20.0f, 0.003f, 512.0f),
    modelCounter(0),
    numModels(1),
    numModelsCreated(0),
    demoCase(0),
    capturing(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TextureStreamingApplication::~TextureStreamingApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureStreamingApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(0.02, 0.02, 0.02f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 0.15f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.52f, 0.52f, 0.52f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        // create a few point lights
        IndexT lightIndex;
        const SizeT numLights = 1;
        const float lightRange = 20;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {   
            float x = 0;//n_rand() * 60.0f - 20.0f;
            float y = 5;
            float z = 0;//n_rand() * 60.0f - 20.0f;
            float r = n_rand() + 1.8f;
            float g = n_rand() + 1.8f;
            float b = n_rand() + 1.8f;

            Ptr<PointLightEntity> pointLight = PointLightEntity::Create();            
            pointLight->SetTransformFromPosDirAndRange(point(x, y, z), -vector::upvec(), lightRange);
            pointLight->SetColor(float4(r, g, b, 1.0f));
            pointLight->SetCastShadows(false);
            this->pointLights.Append(pointLight);
            this->lightTransforms.Append(pointLight->GetTransform());
            this->stage->AttachEntity(pointLight.cast<GraphicsEntity>());
        }
        this->testPointLight = this->pointLights[0];

        const SizeT numSpotLights = 0;
        for (lightIndex = 0; lightIndex < numSpotLights; lightIndex++)
        {
            float x = n_rand() * 50.0f - 50.0f;
            float y = 10;
            float z = n_rand() * 50.0f - 50.0f;
            float r = n_rand() + 0.1f;
            float g = n_rand() + 0.1f;
            float b = n_rand() + 0.1f;
            matrix44 spotLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
            spotLightTransform.scale(point(7,7,7));
            spotLightTransform.set_position(point(x,y,z));
            Ptr<SpotLightEntity> spotLight = SpotLightEntity::Create();
            spotLight->SetTransform(spotLightTransform);
            spotLight->SetColor(float4(r, g, b, 1.0f));
            spotLight->SetCastShadows(false);
            this->spotLights.Append(spotLight);
            this->stage->AttachEntity(spotLight.cast<GraphicsEntity>());
        } 

        // create one shadow casting light 
        matrix44 transform = matrix44::scaling(point(30,30,60));
        transform = matrix44::multiply(transform, matrix44::rotationx(n_deg2rad(this->rotX)));        
        transform.set_position(point(0,7,10));
        this->testSpotLight = SpotLightEntity::Create();
        this->testSpotLight->SetTransform(transform);
        this->testSpotLight->SetCastShadows(false);
        this->testSpotLight->SetColor(float4(1,0,0,1));
        this->stage->AttachEntity(this->testSpotLight.cast<GraphicsEntity>());

        // setup models        
        this->ground = ModelEntity::Create();
        this->ground->SetResourceId(ResourceId("mdl:examples/emptyground.n3"));
        this->ground->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(ground.cast<GraphicsEntity>());

        // setup frame capture render module
        Ptr<FrameCaptureRenderModule> module = FrameCaptureRenderModule::Create();
        module->Setup();
        
                                      
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
    TextureStreamingApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
TextureStreamingApplication::Close()
{
    this->testPointLight = 0;
    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());          
    this->globalLight = 0;
    this->ground = 0;   
                         
    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->pointLights[i].cast<GraphicsEntity>());
    }
    this->pointLights.Clear();
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->spotLights[i].cast<GraphicsEntity>());
    }
    this->spotLights.Clear();

    this->stage->RemoveEntity(this->testSpotLight.cast<GraphicsEntity>());
    this->testSpotLight = 0;

    for (i = 0; i < this->models.Size(); i++)
    {
        this->stage->RemoveEntity(this->models[i].cast<GraphicsEntity>());    	
    }
    this->models.Clear();



    FrameCaptureRenderModule::Instance()->Discard();
    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TextureStreamingApplication::OnSetupResourceMappers()
{
    Array<Ptr<ResourceMapper> > resourceMappers;

    // setup resource mapper for textures
    Ptr<PoolResourceMapper> texMapper = PoolResourceMapper::Create();
    texMapper->SetAsyncEnabled(true);
    texMapper->SetResourceType(Texture::RTTI);
    texMapper->SetPlaceholderResourceId(ResourceId("tex:system/placeholder.dds"));
    texMapper->SetResourceLoaderClass(Resources::TextureStreamer::RTTI);
    //texMapper->SetResourceLoaderClass(CoreGraphics::StreamTextureLoader::RTTI);
    texMapper->SetResourceCreatorClass(Resources::TextureCreator::RTTI);
    texMapper->InitResourceDict(IO::URI("tex:resources.dic"));
    texMapper->SetScheduler(Resources::TexturePoolMapperScheduler::Create());
    texMapper->SetDefaultPoolScheduler(Resources::PoolScheduler::Create());
    texMapper->SetAutoMipMapping(true);
    // read xml containing pool info
    Util::Dictionary<Util::StringAtom, PoolSetupInfo> poolSetupData;
    
    IO::URI texPoolName;

     // ---- demo specific stuff ----
     if (this->demoCase != InvalidIndex)
     {
         switch (this->demoCase)
         {
             // tank demo
         case 0:
             texPoolName = "tex:tankdemo_pool.xml";
             break;
         case 1:
             texPoolName = "groundmipmapdemo_pool.xml";
             break;
         default:
             n_error("invalid demo case");
         }
         n_assert2(IO::IoServer::Instance()->FileExists(texPoolName), "demo pool XMLs not found! copy XMLs from /work/textures to export_win32/textures first!");
     }
     // ------------------------------
     else
     {
         if (IO::IoServer::Instance()->FileExists("tex:modified_pool.xml"))
         {
            texPoolName = "tex:modified_pool.xml";
         }
         else
         {
             texPoolName = "tex:default_pool.xml";
             n_assert(IO::IoServer::Instance()->FileExists(texPoolName));
         }
     }

    PoolResourceMapper::ReadTexturePoolFromXML(texPoolName, poolSetupData);
    texMapper->SetupPools(poolSetupData);
    resourceMappers.Append(texMapper.upcast<ResourceMapper>());
    poolSetupData.Clear();

    //     Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    //     texMapper->SetPlaceholderResourceId(ResourceId(NEBULA3_PLACEHOLDER_TEXTURENAME));
    //     texMapper->SetResourceClass(Texture::RTTI);
    //     texMapper->SetResourceLoaderClass(CoreGraphics::StreamTextureLoader::RTTI);
    //     texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    //     resourceMappers.Append(texMapper.upcast<ResourceMapper>());

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

//------------------------------------------------------------------------------
/**
*/
void
TextureStreamingApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    const Ptr<GamePad>& gamePad = InputServer::Instance()->GetDefaultGamePad(0);
    float moveDelta = 1.0f;
    
    if (kbd->KeyPressed(Key::A))
    {
        matrix44 lightPos = this->testPointLight->GetTransform();
        float4 trans(-moveDelta, 0.0, 0.0, 0.0);
        lightPos = matrix44::multiply(lightPos, matrix44::translation(trans));
        testPointLight->SetTransform(lightPos);
    }
    else if (kbd->KeyPressed(Key::D))
    {
        matrix44 lightPos = this->testPointLight->GetTransform();
        float4 trans(moveDelta, 0.0, 0.0, 0.0);
        lightPos = matrix44::multiply(lightPos, matrix44::translation(trans));
        testPointLight->SetTransform(lightPos);
    }
    if (kbd->KeyPressed(Key::W))
    {
        matrix44 lightPos = this->testPointLight->GetTransform();
        float4 trans(0.0, 0.0, -moveDelta, 0.0);
        lightPos = matrix44::multiply(lightPos, matrix44::translation(trans));
        testPointLight->SetTransform(lightPos);
    }
    else if (kbd->KeyPressed(Key::S))
    {
        matrix44 lightPos = this->testPointLight->GetTransform();
        float4 trans(0.0, 0.0, moveDelta, 0.0);
        lightPos = matrix44::multiply(lightPos, matrix44::translation(trans));
        testPointLight->SetTransform(lightPos);
    }
    if (kbd->KeyDown(Key::N))
    {
        this->AppendTestModel();
    }
    else if(kbd->KeyDown(Key::M) && this->models.Size() > 0)
    {
        n_printf("removing model: '%s'\n", this->models[0]->GetResourceId().AsString().AsCharPtr());
        this->stage->RemoveEntity(this->models[0].cast<GraphicsEntity>());
        this->models.EraseIndex(0);
    }
    float2 orbiting(0.0f, 0.0f);
    if (kbd->KeyPressed(Key::NumPad4))
    {
        orbiting.set(0.1f, 0.0f);
    }
    else if (kbd->KeyPressed(Key::NumPad6))
    {
        orbiting.set(-0.1f, 0.0f);
    }
    this->mayaCameraUtil.SetOrbiting(orbiting);
    //
    ////// enable/disable debug view
    //if (kbd->KeyDown(Key::F4))
    //{
    //    // turn on debug rendering        
    //    Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
    //    renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
    //    renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
    //    Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
    //    this->renderDebug = !this->renderDebug;
    //}
    ViewerApplication::OnProcessInput(); 
}

//------------------------------------------------------------------------------
/**
*/
void
TextureStreamingApplication::OnUpdateFrame()
{
    // ----test text rendering
    Timing::Time frameTime = (float)this->GetFrameTime();
    Util::String fpsTxt;
    fpsTxt.Format("Game FPS: %.2f", 1/frameTime);
    _debug_text(fpsTxt, Math::float2(0.0,0.0), Math::float4(1,1,1,1));
    _debug_text("Toggle Benchmark with F3!", Math::float2(0.0,0.025), Math::float4(1,1,1,1));      
    // --------

    float4 spotLightPos = this->testPointLight->GetTransform().get_position();

    ViewerApplication::OnUpdateFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
TextureStreamingApplication::AppendTestModel()
{
    if (demoCase == InvalidIndex)
    {
        return;
    }
    float x, y, startX, startY;
    Util::String resId;
    if (demoCase == 0)
    {
        int maxVariantModels = 9;
        if (this->numModelsCreated <= maxVariantModels)
        {
            x = (float)(this->numModelsCreated % 3) * 20.0f;
            y = (float)(this->numModelsCreated / 3) * 15.0f;
            startX = -20.0f;
            startY = -25.0f;
            int curVariant = this->numModelsCreated % maxVariantModels;
            resId.Format("mdl:examples/streamingdemo_tiger%i.n3", curVariant);
        }
    }
    else if (demoCase == 1)
    {
        return;
        /*int maxVariantModels = 9;
        x = (float)(this->numModelsCreated % 3) * 40.0f;
        y = (float)(this->numModelsCreated / 3) * 35.0f;
        startX = -40.0f;
        startY = -45.0f;
        int curVariant = this->numModelsCreated % maxVariantModels;
        resId.Format("mdl:locr01/locr01_bauwerk_haus_g_0%i.n3", curVariant);*/
    }
    else
    {
        n_error("unknown democase: %i\n", demoCase);
    }

    Ptr<ModelEntity> model = ModelEntity::Create();
    float4 trans(startX + x, 0.0f, startY + y, 0.0);
    matrix44 pos = matrix44::translation(trans);
    model->SetTransform(pos);
    model->SetResourceId(ResourceId(resId));
    this->stage->AttachEntity(model.cast<GraphicsEntity>());
    this->models.Append(model);
    if (++this->numModelsCreated == 4)
    {
        this->numModelsCreated++;
    }
    n_printf("appending model: '%s'\n", resId.AsCharPtr());
}
} // namespace Tests
//------------------------------------------------------------------------------