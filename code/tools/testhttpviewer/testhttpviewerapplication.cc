//------------------------------------------------------------------------------
//  testhttpviewerapplication.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testhttpviewer/testhttpviewerapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"

namespace Tools
{
using namespace CoreGraphics;
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;
using namespace Debug;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
TestHttpViewerApplication::TestHttpViewerApplication() : 
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestHttpViewerApplication::~TestHttpViewerApplication()
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
TestHttpViewerApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(0.3, 0.3f, 0.3f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 0.5f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.1f, 0.1f, 0.1f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        // create a few point lights
        /*
        IndexT lightIndex;
        const SizeT numLights = 4;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {
            float x = n_rand() * 40.0f - 20.0f;
            float y = 3;
            float z = n_rand() * 40.0f - 20.0f;
            float r = n_rand() * 2.0f;
            float g = n_rand() * 2.0f;
            float b = n_rand() * 2.0f;
            matrix44 pointLightTransform = matrix44::multiply(matrix44::scaling(10.0f, 10.0f, 10.0f), 
                                                              matrix44::translation(x, y, z));

            Ptr<PointLightEntity> pointLight = PointLightEntity::Create();
            pointLight->SetTransform(pointLightTransform);
            pointLight->SetColor(float4(r, g, b, 1.0f));
            pointLight->SetCastShadows(false);
            this->localLights.Append(pointLight);
            this->lightTransforms.Append(pointLightTransform);
            this->stage->AttachEntity(pointLight.cast<GraphicsEntity>());
        }
        */

        // setup models        
        this->ground = ModelEntity::Create();
        this->ground->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
        this->ground->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(ground.cast<GraphicsEntity>());
                       
        IndexT j;
        bool createStatic = true;
        for (j = -1; j < 2; ++j)
        {
            IndexT i;
            for (i = -1; i < 2; ++i)
            {
                Ptr<ModelEntity> model = ModelEntity::Create();
                model->SetTransform(matrix44::translation(7.0f * i, 0.0, 7.0f * j - 5));
                if (createStatic)
                {                                                                   
                    model->SetResourceId(ResourceId("mdl:examples/tiger.n3"));  
                    this->stage->AttachEntity(model.cast<GraphicsEntity>());
                }
                else
                {                                                             
                    model->SetResourceId(ResourceId("mdl:characters/mensch_m.n3"));
                    this->stage->AttachEntity(model.cast<GraphicsEntity>());
                
                    // apply skin
                    Ptr<Graphics::ApplySkinList> skinList = Graphics::ApplySkinList::Create();
                    skinList->SetSkinList(StringAtom("mann_nackt"));
                    model->SendMsg(skinList.cast<GraphicsEntityMessage>());

                    // play animation
                    Ptr<Graphics::AnimPlayClip> playClip = Graphics::AnimPlayClip::Create();
                    playClip->SetClipName("gehen_01");
                    playClip->SetTrackIndex(0);
                    playClip->SetLoopCount(0.0f);
                    model->SendMsg(playClip.cast<GraphicsEntityMessage>());
                } 
                this->models.Append(model);  

                createStatic  = !createStatic;
            } 
        }          
        
        // wait for resources to be loaded
        // GraphicsInterface::Instance()->WaitForPendingResources();
                                      
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestHttpViewerApplication::Close()
{
    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());    
    this->globalLight = 0;
    this->ground = 0;

    IndexT i;
    for (i = 0; i < this->localLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->localLights[i].cast<GraphicsEntity>());
    }
    this->localLights.Clear();

    for (i = 0; i < this->models.Size(); i++)
    {
        this->stage->RemoveEntity(this->models[i].cast<GraphicsEntity>());    	
    }
    this->models.Clear();
    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TestHttpViewerApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().DisplayMode() = CoreGraphics::DisplayMode(640, 480, CoreGraphics::PixelFormat::X8R8G8B8);
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
TestHttpViewerApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    
    // enable/disable debug view
    if (kbd->KeyDown(Key::F4))
    {
        // turn on debug rendering        
        Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
        renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
        renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
        Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
        this->renderDebug = !this->renderDebug;
    }
    else if (kbd->KeyDown(Key::F3))
    {
        this->benchmarkmode = !this->benchmarkmode;
        // reset to start pos
        this->mayaCameraUtil.Reset();
    }
        
    if (this->benchmarkmode)
    {
        // auto rotate
        this->mayaCameraUtil.SetOrbiting(Math::float2(0.02,0));
        this->mayaCameraUtil.SetOrbitButton(true);
        this->mayaCameraUtil.Update();
        this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
    }
    else
    {                                        
        ViewerApplication::OnProcessInput(); 
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TestHttpViewerApplication::OnUpdateFrame()
{
    // test text rendering
    Timing::Time frameTime = (float)this->GetFrameTime();
    Util::String fpsTxt;
    fpsTxt.Format("Game FPS: %.2f", 1/frameTime);
    _debug_text(fpsTxt, Math::float2(0.0,0.0), Math::float4(1,1,1,1));
    _debug_text("Toggle Benchmark with F3!", Math::float2(0.0,0.025), Math::float4(1,1,1,1));

    if (this->benchmarkmode)
    {
        // benchmark with avg frames for every 100 frames
        if (this->frameTimes.Size() > 0 || frameTime < 0.08)
        {
            this->frameTimes.Append(frameTime);
        }       
        if (this->frameTimes.Size() > 0 && this->frameTimes.Size() % 50 == 0)
        {
            this->avgFPS = 0;
            IndexT i;
            for (i = 0; i < this->frameTimes.Size(); ++i)
            {
        	    this->avgFPS += this->frameTimes[i];
            }
            this->avgFPS /=  this->frameTimes.Size();
        }   
        if (this->avgFPS > 0)
        {
            fpsTxt.Format("Benchmarking: Avg Game FPS: %.2f", 1/this->avgFPS);
        }
        else
        {
            fpsTxt = "Benchmarking: Wait for measuring average framerate";
        }
        _debug_text(fpsTxt, Math::float2(0.0,0.05f), Math::float4(1,1,1,1));
    }               
          
    float time = (float) this->GetTime();
    IndexT i;
    for (i = 0; i < this->localLights.Size(); i++)
    {
        matrix44 m = this->lightTransforms[i];
        float4 t(5 * n_sin(0.1f * i * time), 0.0f, 5 * n_sin(0.05f * i * time), 0.0f);
        m.translate(t);
        this->localLights[i]->SetTransform(m);        
    }

    // render a few debug shapes 
    /*IndexT x;
    for (x = 0; x < 10; x++)
    {
        IndexT y;
        for (y = 0; y < 10; y++)
        {
            quaternion rot = quaternion::rotationyawpitchroll(0.0f, curTime, 0.0f);
            matrix44 m = matrix44::affinetransformation(1.0f, vector::nullvec(), rot, point(x * 2.0f, 1.0f, y * 2.0f));
            DebugShapeRenderer::Instance()->DrawBox(m, float4(1.0f, 0.0f, 0.0f, 0.5f));
        }
    }*/
    
    ViewerApplication::OnUpdateFrame();
}
} // namespace Tools
