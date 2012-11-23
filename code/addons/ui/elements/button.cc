//------------------------------------------------------------------------------
//  ui/elements/button.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/button.h"
#include "ui/uimanager.h"
#include "ui/uievent.h"
#include "ui/uirenderer.h"

namespace UI
{
__ImplementClass(Button, 'UIBT', Element);

//------------------------------------------------------------------------------
/**
*/
Button::Button() :   
    activeState(Normal),
    pressed(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Button::~Button()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Button::OnCreate()
{
    // register textures
    IndexT i;
    for (i = 0; i < NumVisualStates; i++)
    {
        if (this->stateTextures[i].IsValid())
        {
            UIRenderer::Instance()->RegisterTexture(this->stateTextures[i]);
        }
    }

    // call parent
    Element::OnCreate();
}

//------------------------------------------------------------------------------
/**
*/
void
Button::OnDestroy()
{
    // unregister textures
    IndexT i;
    for (i = 0; i < NumVisualStates; i++)
    {
        if (this->stateTextures[i].IsValid())
        {
            UIRenderer::Instance()->UnregisterTexture(this->stateTextures[i]);
        }
    }

    // call parent
    Element::OnDestroy();
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnRender()
{
    // set correct UV for rendering
    if (!this->enabled)
    {
        this->SetActiveVisualState(Disabled);
    }
    else if (this->IsMouseOver())
    {
        if(this->pressed)
        {
            this->SetActiveVisualState(Pressed);
        }
        else
        {
            this->SetActiveVisualState(MouseOver);
        }
        // if mouse is over element, switch focus element
        this->RegisterAsFocusElement();
    }
    else
    {        
        // reset pressed state if mouse outside
        this->pressed = false;
        if (this->IsFocusElement())
        {
            this->UnregisterAsFocusElement();
        }
    }

    // select the right UV and texture for rendering
    n_assert((this->activeState >= 0) && (this->activeState < NumVisualStates));
    this->renderUV = this->stateUV[this->activeState];
    if (this->stateTextures[this->activeState].IsValid())
    {
        this->renderTexture = this->stateTextures[this->activeState];
    }
    else
    {
        this->renderTexture = this->stateTextures[Normal];
    }

    // now call parent to render
    Element::OnRender();    
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button down. This sets the pressed state of the
    button. Also set the pressed state.
*/
void
Button::OnLeftButtonDown(const Math::float2& mousePos)
{
    // DSA expects one event, either up or down, we only send up-event
    if(!UIManager::Instance()->GetDsaLegacyModeFlag())
    {
	    // build event
	    Ptr<Button> ptr(this);
	    Ptr<UIEvent> event = UIEvent::Create();
	    event->SetElement(ptr.cast<Element>());
	    event->SetEventType(Input::InputEvent::MouseButtonDown);
	    event->SetMouseButton(Input::MouseButton::LeftButton);
	    UIManager::Instance()->PutWindowEvent(this->parentWindow, event);
    }

    // set state
    this->pressed = true;
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button up. This sends the button event.
    Also reset the pressed state.
*/
void
Button::OnLeftButtonUp(const Math::float2& mousePos)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
	event->SetEventType(Input::InputEvent::MouseButtonUp);
	event->SetMouseButton(Input::MouseButton::LeftButton);
	UIManager::Instance()->PutWindowEvent(this->parentWindow, event);    

    // reset state
    this->pressed = false;
}

//------------------------------------------------------------------------------
/**
    Called on gamepad button down. This sets the pressed state of the
    button.
*/
void
Button::OnGamePadButtonDown(Input::GamePad::Button btn)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
    event->SetEventType(Input::InputEvent::GamePadButtonDown);
    event->SetGamePadButton(btn);
	UIManager::Instance()->PutWindowEvent(this->parentWindow, event);
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button up. This sends the button event.
*/
void
Button::OnGamePadButtonUp(Input::GamePad::Button btn)
{
	// build event
	Ptr<Button> ptr(this);
	Ptr<UIEvent> event = UIEvent::Create();
	event->SetElement(ptr.cast<Element>());
    event->SetEventType(Input::InputEvent::GamePadButtonUp);
    event->SetGamePadButton(btn);
	UIManager::Instance()->PutWindowEvent(this->parentWindow, event);
}

//------------------------------------------------------------------------------
/**
*/    
Util::String
Button::VisualStateToString(VisualState visualState)
{
    n_assert(visualState < NumVisualStates);
    switch (visualState)
    {
        case Normal:        return "Normal";
        case Pressed:       return "Pressed";
        case MouseOver:     return "MouseOver";
        case Disabled:      return "Disabled";
        default:            return "Normal";
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Button::SetActiveVisualState(VisualState state)
{
    n_assert((state >= 0) && (state < NumVisualStates));
    this->activeState = state;
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnFocusGain()
{
    this->SetActiveVisualState(MouseOver);
}

//------------------------------------------------------------------------------
/**
*/
void 
Button::OnFocusLost()
{
    this->SetActiveVisualState(Normal);
}

} // namespace UI

