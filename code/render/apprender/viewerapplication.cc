//------------------------------------------------------------------------------
//  viewerapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/viewerapplication.h"
#include "visibility/visibilitysystems/visibilityquadtree.h"
#include "visibility/visibilitysystems/visibilityclustersystem.h"
#include "visibility/visibilitysystems/visibilityboxsystem.h"
#include "graphics/view.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "internalgraphics/internalview.h"

namespace App
{
using namespace Math;
using namespace Graphics;
using namespace Util;
using namespace Resources;
using namespace Input;
using namespace Debug;
using namespace Visibility;


//------------------------------------------------------------------------------
/**
*/
ViewerApplication::ViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ViewerApplication::~ViewerApplication()
{
    n_assert(!this->stage.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
bool
ViewerApplication::Open()
{
    if (RenderApplication::Open())
    {
        StringAtom defaultStageName("DefaultStage");
        StringAtom defaultViewName("DefaultView");
                                                         
        // create a GraphicServer, Stage and View
        this->graphicsServer = GraphicsServer::Create();
        this->graphicsServer->Open();
        this->debugShapeRenderer = DebugShapeRenderer::Create();
        this->debugTextRenderer = DebugTextRenderer::Create();

        // create a default stage
        // attach visibility systems to checker
        Ptr<VisibilityQuadtree> visQuadtreeSystem = VisibilityQuadtree::Create();
        visQuadtreeSystem->SetQuadTreeSettings(4, Math::bbox(Math::point(0,0,0), Math::vector(100.0f, 10.0f, 100.0f)));
        Ptr<VisibilityClusterSystem> visClusterSystem = VisibilityClusterSystem::Create();
        Ptr<VisibilityBoxSystem> visBoxSystem = VisibilityBoxSystem::Create();
        
        Util::Array<Ptr<VisibilitySystemBase> > visSystems;
        visSystems.Append(visQuadtreeSystem.cast<VisibilitySystemBase>());
        //visSystems.Append(visClusterSystem.cast<VisibilitySystemBase>());
        //visSystems.Append(visBoxSystem.cast<VisibilitySystemBase>());
        this->stage = this->graphicsServer->CreateStage(defaultStageName, visSystems);

        // create a default view
        // FIXME: sucks that I have to use InternalGraphics here!
        this->view = this->graphicsServer->CreateView(InternalGraphics::InternalView::RTTI,
                                                      defaultViewName,
                                                      defaultStageName,
                                                      ResourceId(NEBULA3_DEFAULT_FRAMESHADER_NAME),
                                                      true);

        // create a camera entity
        this->camera = CameraEntity::Create();
        this->stage->AttachEntity(this->camera.cast<GraphicsEntity>());
        this->view->SetCameraEntity(this->camera);

        // setup the camera util object
        this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 3.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));
        this->mayaCameraUtil.Update();
        this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::Close()
{
    this->stage->RemoveEntity(this->camera.cast<GraphicsEntity>());
    this->camera = 0;

    this->graphicsServer->DiscardView(this->view);
    this->view = 0;
    
    this->graphicsServer->DiscardStage(this->stage);
    this->stage = 0;

    this->debugTextRenderer = 0;
    this->debugShapeRenderer = 0;
    this->graphicsServer->Close();
    this->graphicsServer = 0;

    // call parent class
    RenderApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnProcessInput()
{
    #if !__WII__
    
    // update the camera from input
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
    const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);

    // standard input handling: manipulate camera
    this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
    this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
    this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
    this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
    this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
    this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());
    
    // process gamepad input
    float zoomIn = 0.0f;
    float zoomOut = 0.0f;
    float2 panning(0.0f, 0.0f);
    float2 orbiting(0.0f, 0.0f);
    if (gamePad->IsConnected())
    {
        const float gamePadZoomSpeed = 5.0f;
        const float gamePadOrbitSpeed = 1.0f;
        const float gamePadPanSpeed = 10.0f;
        if (gamePad->ButtonDown(GamePad::AButton))
        {
            this->mayaCameraUtil.Reset();
        }
        if (gamePad->ButtonDown(GamePad::StartButton) ||
            gamePad->ButtonDown(GamePad::BackButton))
        {
            this->SetQuitRequested(true);
        }
        float frameTime = (float) this->GetFrameTime();
        zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime * gamePadZoomSpeed;
        zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime * gamePadZoomSpeed;
        panning.x()  -= gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime * gamePadPanSpeed;
        panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime * gamePadPanSpeed;
        orbiting.x() -= gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime * gamePadOrbitSpeed;
        orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime * gamePadOrbitSpeed;
    }

    // process keyboard input
    if (keyboard->KeyDown(Key::Escape))
    {
        this->SetQuitRequested(true);
    }
    if (keyboard->KeyDown(Key::Space))
    {
        this->mayaCameraUtil.Reset();
    }
    if (keyboard->KeyPressed(Key::Left))
    {
        panning.x() -= 0.1f;
    }
    if (keyboard->KeyPressed(Key::Right))
    {
        panning.x() += 0.1f;
    }
    if (keyboard->KeyPressed(Key::Up))
    {
        panning.y() -= 0.1f;
    }
    if (keyboard->KeyPressed(Key::Down))
    {
        panning.y() += 0.1f;
    }

    this->mayaCameraUtil.SetPanning(panning);
    this->mayaCameraUtil.SetOrbiting(orbiting);
    this->mayaCameraUtil.SetZoomIn(zoomIn);
    this->mayaCameraUtil.SetZoomOut(zoomOut);
    this->mayaCameraUtil.Update();
    this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
    
    #endif // __WII__
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnUpdateFrame()
{
    this->graphicsServer->OnFrame();
    this->debugShapeRenderer->OnFrame();
    this->debugTextRenderer->OnFrame();
    Threading::Thread::YieldThread();
}

} // namespace App

    
