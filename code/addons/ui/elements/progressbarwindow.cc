//------------------------------------------------------------------------------
//  progressbarwindow.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/progressbarwindow.h"

namespace UI
{
__ImplementClass(UI::ProgressBarWindow, 'UPBW', UI::Window);

//------------------------------------------------------------------------------
/**
*/
ProgressBarWindow::ProgressBarWindow() /*:
    rangeMaxValue(100),
    rangeCurValue(0),
    debugTextEnabled(true),
    inputEnabled(false),
    okEventReceived(false)*/
{
/*
    this->SetLayerId("Front");
    this->SetModal(true);
*/
}

//------------------------------------------------------------------------------
/**
*/
ProgressBarWindow::~ProgressBarWindow()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ProgressBarWindow::Open()
{
/*
    // reset values
    this->rangeMaxValue = 100;
    this->rangeCurValue = 0;
    this->text.Clear();

    // call parent class
    Window::Open();
    this->HideOkButton();

    // clear time source of canvas, so that the graphics server's time is used
    this->canvas->GetGraphicsEntity()->SetTimeSource(0);
*/
}

//------------------------------------------------------------------------------
/**
    This does a full autonomous render loop for this window.
*/
void
ProgressBarWindow::Present()
{
/*
    // set correct animation time for the gfx entity
    MGraphics::Entity* gfxEntity = this->canvas->GetGraphicsEntity();
    if (gfxEntity)
    {
        // we need to convert the progress into a time between 0.0 and 1.0,
        // so the "progress" must be visualized in the graphics resource
        // with an animation between 0 and 1
        nTime activationTime = gfxEntity->GetActivationTime();
        nTime progressTime = nTime(this->rangeCurValue) / nTime(this->rangeMaxValue);
    }
    
    UI::Server::Instance()->Trigger();
    if (this->inputEnabled)
    {
        Input::InputServer::Instance()->BeginFrame();
        Input::InputServer::Instance()->OnFrame();
        UI::Server::Instance()->Trigger();
    }

    // render the scene
    nSceneServer* sceneServer = nSceneServer::Instance();
    if (sceneServer->BeginScene(nGfxServer2::Instance()->GetCamera().GetProjection()))
    {        
        // trigger on frame stuff (usually done by the gfx server)        
        UI::Server::Instance()->Render();

    #if !(PUBLIC_BUILD)
        if (this->debugTextEnabled)
        {
            // draw the text
            if (this->text.IsValid())
            {
                nGfxServer2* gfxServer = nGfxServer2::Instance();
                rectangle rect(vector2(0.0f, 0.0f), vector2(1.0f, 0.1f));
                vector4 color(1.0f, 0.5f, 0.0f, 1.0f);
                gfxServer->DrawText(this->text, color, rect, nFont2::Left|nFont2::VCenter|nFont2::NoClip, false);
            }
        }
    #endif
        // finish rendering
        sceneServer->EndScene();
        sceneServer->RenderScene();
        sceneServer->PresentScene();
    }
    if (this->inputEnabled)
    {
        Input::InputServer::Instance()->EndFrame();
    }
*/
}

//------------------------------------------------------------------------------
/**
    Make an gui element visible/invisible.
*/
/*
void
ProgressBarWindow::SetElementVisibility(const nString& elementName, bool visible)
{
    Element* elm = this->canvas->FindChildElement(elementName);
    if (0 != elm)
    {
        elm->SetVisible(visible);
    }
}
*/

//------------------------------------------------------------------------------
/**
    Show the optional ok button.
*/
/*
void
ProgressBarWindow::ShowOkButton()
{
    this->SetElementVisibility("OkButton", true);
    this->SetElementVisibility("Loading", false);
    this->SetElementVisibility("ProgressBar", false);
    this->SetElementVisibility("ProgressBackground", false);
}
*/

//------------------------------------------------------------------------------
/**
    Hide the optional ok button.
*/
/*
void
ProgressBarWindow::HideOkButton()
{   
    this->SetElementVisibility("OkButton", false);
    this->SetElementVisibility("Loading", true);
    this->SetElementVisibility("ProgressBar", true);
    this->SetElementVisibility("ProgressBackground", true);
}
*/

//------------------------------------------------------------------------------
/**
    This runs the render loop until the Ok button is pressed.
*/
void
ProgressBarWindow::WaitForOkButton()
{
/*
    this->ShowOkButton();
    this->inputEnabled = true;
    this->okEventReceived = false;
    while (!this->okEventReceived)
    {
        this->Present();
        n_sleep(0.01);
    }
    // Audio Hack
    Audio::AudioServer::Instance()->Play2DSoundEffect("ui_click", 1.f);
    this->inputEnabled = false;
*/
}

//------------------------------------------------------------------------------
/**    
*/
void
ProgressBarWindow::HandleEvent(Event* e)
{
/*
    n_assert(0 != e);
    if (e->GetEventName() == "Ok")
    {
        this->okEventReceived = true;
    }
    Window::HandleEvent(e);
*/
}

} // namespace UI
    