//------------------------------------------------------------------------------
//  drasa3testuiapplication.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tests/drasa3testui/drasa3testuiapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "GfxSysAlloc.h"
#include "GfXRender.h"

GfxSysAlloc gfxAlloc;
GFxSystem	gfxInit(&gfxAlloc);

namespace Test
{
using namespace CoreGraphics;
using namespace Graphics;
using namespace Resources;
using namespace Util;
using namespace Timing;
using namespace Debug;
using namespace Input;
using namespace ScaleForms;

//------------------------------------------------------------------------------
/**
*/
Drasa3TestUiApplication::Drasa3TestUiApplication() : 
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false),
    rotX(-225),
    direction(1.0),
    shadowConstants(100.0f, 20.0f, 0.003f, 512.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Drasa3TestUiApplication::~Drasa3TestUiApplication()
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
Drasa3TestUiApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        Math::matrix44 lightTransform = Math::matrix44::rotationx(Math::n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(Math::float4(0.02, 0.02, 0.02f, 1.0f));
        this->globalLight->SetBackLightColor(Math::float4(0.0f, 0.0f, 0.15f, 1.0f));
        this->globalLight->SetAmbientLightColor(Math::float4(0.02f, 0.02f, 0.02f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        // create a few point lights
        IndexT lightIndex;
        const SizeT numLights = 1;
        const float scaling = 20;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {   
            float x = 0;//n_rand() * 60.0f - 20.0f;
            float y = 5;
            float z = 0;//n_rand() * 60.0f - 20.0f;
            float r = Math::n_rand() + 0.8f;
            float g = Math::n_rand() + 0.8f;
            float b = Math::n_rand() + 0.8f;
            Math::matrix44 pointLightTransform = Math::matrix44::rotationx(Math::n_deg2rad(-90.0f));
            pointLightTransform =  Math::matrix44::multiply(pointLightTransform , 
                            Math::matrix44::multiply(
                            Math::matrix44::scaling(scaling, scaling, scaling), 
                            Math::matrix44::translation(x, y, z)));

            Ptr<PointLightEntity> pointLight = PointLightEntity::Create();            
            pointLight->SetTransform(pointLightTransform);
            pointLight->SetColor(Math::float4(r, g, b, 1.0f));
            pointLight->SetCastShadows(true);
            this->pointLights.Append(pointLight);
            this->lightTransforms.Append(pointLightTransform);
            this->stage->AttachEntity(pointLight.cast<GraphicsEntity>());
        }

        const SizeT numSpotLights = 0;
        for (lightIndex = 0; lightIndex < numSpotLights; lightIndex++)
        {
            float x = Math::n_rand() * 50.0f - 50.0f;
            float y = 10;
            float z = Math::n_rand() * 50.0f - 50.0f;
            float r = Math::n_rand() + 0.1f;
            float g = Math::n_rand() + 0.1f;
            float b = Math::n_rand() + 0.1f;
            Math::matrix44 spotLightTransform = Math::matrix44::rotationx(Math::n_deg2rad(-45.0f));
            spotLightTransform.scale(Math::point(7,7,7));
            spotLightTransform.set_position(Math::point(x,y,z));
            Ptr<SpotLightEntity> spotLight = SpotLightEntity::Create();
            spotLight->SetTransform(spotLightTransform);
            spotLight->SetColor(Math::float4(r, g, b, 1.0f));
            spotLight->SetCastShadows(true);
            this->spotLights.Append(spotLight);
            this->stage->AttachEntity(spotLight.cast<GraphicsEntity>());
        } 

        // create one shadow casting light 
        Math::matrix44 transform = Math::matrix44::scaling(Math::point(30,30,60));
        transform = Math::matrix44::multiply(transform, Math::matrix44::rotationx(Math::n_deg2rad(this->rotX)));        
        transform.set_position(Math::point(0,7,10));
        this->testSpotLight = SpotLightEntity::Create();
        this->testSpotLight->SetTransform(transform);
        this->testSpotLight->SetCastShadows(true);
        this->testSpotLight->SetColor(Math::float4(2,2,1,1));
        this->stage->AttachEntity(this->testSpotLight.cast<GraphicsEntity>());

        // setup models        
        this->ground = ModelEntity::Create();
        this->ground->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
        this->ground->SetTransform(Math::matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(ground.cast<GraphicsEntity>());
                       
        IndexT j;
        IndexT i;
        bool createStatic = true;
        float spacing = 2.0f;
        for (j = 0; j < 1; ++j)
        {
            for (i = 0; i < 1; ++i)
            {
                Ptr<ModelEntity> model = ModelEntity::Create();
				model->SetTransform(Math::matrix44::translation(0.0f, 0.0f, 0.0f));
				model->SetResourceId(ResourceId("mdl:examples/tiger.n3"));  
				this->stage->AttachEntity(model.cast<GraphicsEntity>());
                this->models.Append(model);  
            } 
        }                                           

	    //this->uiManager = UI::UIManager::Create();
	    //this->uiRenderer = UI::UIRenderer::Create();

	    //this->uiRenderModule = UI::UIRenderModule::Create();
	    //this->uiRenderModule->Setup();
     //   //this->uiManager->Open();	

     //   this->mainWindow = UI::MainScreen::Create();
     //   this->mainWindow->SetResource("mainscreen");
     //   this->mainWindow->Open();

		this->gfxRenderModule = ScaleForms::GfxRenderModule::Create();
		this->gfxRenderModule->Setup();		

        // wait for resources to be loaded
        GraphicsInterface::Instance()->WaitForPendingResources();

		
                                      
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Drasa3TestUiApplication::Close()
{
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

    //this->uiManager->Close();
    //this->uiManager = 0;
    //this->uiRenderModule->Discard();
    //this->uiRenderModule = 0;

	this->gfxRenderModule->Discard();
	this->gfxRenderModule = 0;

    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
Drasa3TestUiApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
Drasa3TestUiApplication::OnProcessInput()
{
	this->gfxRenderModule->OnFrame();

    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    const Ptr<GamePad>& gamePad = InputServer::Instance()->GetDefaultGamePad(0);
    
    // enable/disable debug view
    if (kbd->KeyDown(Key::F4) || gamePad.isvalid() && gamePad->ButtonDown(GamePad::XButton))
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
    else if (kbd->KeyDown(Key::F6) || gamePad->ButtonDown(GamePad::LeftThumbButton))
    {
        this->direction *= -1;
    }
    
    bool updateSharedVar = false;
    if (kbd->KeyDown(Key::Q) || gamePad->ButtonDown(GamePad::RightThumbButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            if (this->shadowConstants.x() >= 1.0f)
            {
                this->shadowConstants.x() -= 1.0f;    
            }
            else
            {
                this->shadowConstants.x() -= 0.1f;    
            }            
        }
        else
        {
            if (this->shadowConstants.x() >= 1.0f)
            {
                this->shadowConstants.x() += 1.0f;    
            }
            else
            {
                this->shadowConstants.x() += 0.1f;    
            }
        }           
        this->shadowConstants.x() = Math::n_clamp(this->shadowConstants.x(), 0.1f, 100.0f);
        updateSharedVar = true;
    }
    else if (kbd->KeyDown(Key::W) || gamePad->ButtonDown(GamePad::YButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            this->shadowConstants.y() -= 1.0f;    
        }
        else
        {
            this->shadowConstants.y() += 1.0f;    
        }   
        this->shadowConstants.y() = Math::n_clamp(this->shadowConstants.y(), 1.0f, 100.0f);
        updateSharedVar = true;
    } 
    else if (kbd->KeyDown(Key::E) || gamePad->ButtonDown(GamePad::BButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            this->shadowConstants.z() -= 0.0001f;    
        }
        else
        {
            this->shadowConstants.z() += 0.0001f;    
        }           
        updateSharedVar = true;
    }

    Util::String shadowConstantStr(Util::String::FromFloat4(this->shadowConstants));
    _debug_text(shadowConstantStr, Math::float2(0.2,0.0), Math::float4(1,1,1,1));

    if (updateSharedVar)
    {
        Ptr<Graphics::UpdateSharedShaderVariable> msg = Graphics::UpdateSharedShaderVariable::Create();
        msg->SetSemantic("ShadowConstants");
        msg->SetValue(this->shadowConstants);
        GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
    }
        
    if (this->benchmarkmode)
    {
        // auto rotate
        this->mayaCameraUtil.SetOrbiting(Math::float2(0.02,0));
        this->mayaCameraUtil.SetOrbitButton(true);
        this->mayaCameraUtil.Update();
        const Math::matrix44 &m = this->mayaCameraUtil.GetCameraTransform();
        this->camera->SetTransform(m);
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
Drasa3TestUiApplication::UpdateGUI()
{
    // update time in ui system
 //   Timing::Time time = FrameSync::FrameSyncTimer::Instance()->GetTime();
	//this->uiManager->SetTime(time);

 //   // trigger ui
 //   this->uiManager->Trigger();
	//this->uiManager->Render();
 //   if (this->renderDebug)
 //   {
 //       this->uiManager->RenderDebug();
 //   }       

	// now render the stuff	
	//this->uiRenderer->OnFrame();

	// update time in ui system
	//Timing::Time time = FrameSync::FrameSyncTimer::Instance()->GetTime();
	//UI::UIManager* uiManager = UI::UIManager::Instance();

	//// trigger ui
	//uiManager->SetTime(time);
	//uiManager->Trigger();
	//uiManager->Render();

	//// now render the stuff	
	//UI::UIRenderer::Instance()->OnFrame();	
}

//------------------------------------------------------------------------------
/**
*/
void
Drasa3TestUiApplication::OnUpdateFrame()
{
    this->UpdateGUI();

    //DebugShapeRenderer::Instance()->DrawBox(matrix44::identity(), float4(1.0f, 0.0f, 0.0f, 1.0f));
    //DebugShapeRenderer:://Instance()->DrawSphere(matrix44::identity(), float4(1.0f, 0.5f, 0.0f, 0.5f));

    //DebugShapeRenderer::Instance()->DrawBox(matrix44::identity(), float4(1.0f, 0.5f, 0.0f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawSphere(matrix44::translation(-2.0f, 1.0f, 0.0f), float4(0.0f, 0.5f, 0.0f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawCylinder(matrix44::translation(-1.0f, 3.0f, 0.0f), float4(1.0f, 0.5f, 0.2f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawTorus(matrix44::translation(2.0f, 1.0f, 0.0f), float4(0.3f, 0.5f, 0.2f, 0.5f));

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
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        Math::matrix44 m = this->lightTransforms[i];
        Math::float4 t(5 * Math::n_sin(0.1f * i * time), 0.0f, 5 * Math::n_sin(0.05f * i * time), 0.0f);
        m.translate(t);
        this->pointLights[i]->SetTransform(m);        
    }
    //const Math::matrix44& firstOne = this->models[0]->GetTransform();
    //if (firstOne.get_position().z() > 30.0f
    //    || firstOne.get_position().z() < -30.0f)
    //{
    //    direction *= -1;
    //}
    const Math::matrix44& lastOne = this->models.Back()->GetTransform();
    // update shadow point of interest
    //Ptr<Graphics::SetShadowPointOfInterest> poiMsg = Graphics::SetShadowPointOfInterest::Create();
    //poiMsg->SetPoi((firstOne.get_position() + lastOne.get_position()) * 0.5f);
    //GraphicsInterface::Instance()->SendBatched(poiMsg.cast<Messaging::Message>());

    //for (i = 0; i < this->models.Size(); i++)
    //{
    //    const Math::matrix44& m = this->models[i]->GetTransform();           
    //    Math::float4 t(0.0f, 0.0f, 1.2f * (float)frameTime * direction, 0.0f);
    //    Math::float4 newPos = m.get_position() + t;
    //               
    //    // rotate always in move direction
    //    Math::matrix44 newTrans = Math::matrix44::lookatrh(newPos, newPos + t, Math::vector::upvec());        
    //    this->models[i]->SetTransform(newTrans);        
    //}

    // rotate spotlights
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        Math::matrix44 spotLightTransform = Math::matrix44::rotationyawpitchroll(time * 2 * i / this->spotLights.Size(), Math::n_deg2rad(-55), 0);        
        spotLightTransform.scale(Math::point(i * 0.2f + 15, 30, i * 0.2f + 15));
        spotLightTransform.set_position(this->spotLights[i]->GetTransform().get_position());
        this->spotLights[i]->SetTransform(spotLightTransform);
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
    
    // move light
    Ptr<Input::GamePad> pad = Input::InputServer::Instance()->GetDefaultGamePad(0);
    n_assert(pad.isvalid());
    Math::matrix44 tr = this->testSpotLight->GetTransform();
    if (pad->ButtonPressed(Input::GamePad::DPadLeftButton))
    {
        tr.translate(Math::vector(0, 0, -0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadRightButton))
    {
        tr.translate(Math::vector(0, 0, 0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadUpButton))
    {
        tr.translate(Math::vector(0, 0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadDownButton))
    {
        tr.translate(Math::vector(0, -0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::L1Button))
    {
        this->rotX += 0.5f;
        Math::matrix44 rot = Math::matrix44::rotationx(Math::n_deg2rad(this->rotX));
        rot.scale(Math::point(10,10,10));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    if (pad->ButtonPressed(Input::GamePad::R1Button))
    {
        this->rotX -= 0.5f;
        Math::matrix44 rot = Math::matrix44::rotationx(Math::n_deg2rad(this->rotX));
        rot.scale(Math::point(10,10,10));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    this->testSpotLight->SetTransform(tr);

    ViewerApplication::OnUpdateFrame();
}
} // namespace Test
