//------------------------------------------------------------------------------
//  ui/uimanager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uimanager.h"
#include "ui/uieventhandler.h"
#include "ui/uievent.h"
#include "ui/uifactory.h"
#include "ui/elements/window.h"
#include "ui/elements/element.h"
#include "ui/uixmlparser.h"
#include "ui/uirenderer.h"
#include "ui/base/coreuirendererbase.h"
#include "ui/uirenderprotocol.h"
#include "input/inputserver.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "input/keyboard.h"
#include "graphics/graphicsinterface.h"
#include "threading/threadid.h"

namespace UI
{
__ImplementClass(UIManager, 'UISV', Core::RefCounted);
__ImplementSingleton(UIManager);

using namespace Input;
using namespace Graphics;
using namespace Messaging;
using namespace Threading;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
UIManager::UIManager() :
    isOpen(false),      
    globalHide(false),
    inputEnabled(true),
    mousePosition(0.f, 0.f),     
    pixelMousePosition(0.f, 0.f),
    nextElementId(0),
    // @GUI_LEGACY_DSA3
    dsaLegacyMode(true)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIManager::~UIManager()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIManager::Open()
{
    n_assert(!this->isOpen);
	this->isOpen = true;
    this->uiFactory = UIFactory::Create(); 
	this->parser = UIXmlParser::Create();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
UIManager::LoadWindow(const Ptr<Window>& window, const Util::String &fileName)
{
	n_assert(fileName.IsValid());
	this->parser->SetFileName(fileName);
	this->parser->ParseFile(window);	
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Ptr<Window> >
UIManager::LoadWindows(const Util::String& fileName, bool attachAndOpen)
{    
	n_assert(fileName.IsValid());

    // at first load windows
	this->parser->SetFileName(fileName);
	Util::Array<Ptr<Window> > newWins = this->parser->ParseFile();	

    // finally open if wanted
	if (attachAndOpen)
	{
		for(IndexT i = 0; i < newWins.Size(); i++)
		{
			newWins[i]->Open();
		}
	}

	return newWins;
}

//------------------------------------------------------------------------------
/**
*/
void
UIManager::Close()
{
    n_assert(this->isOpen);

    // let focus element unregister itself
    if (this->focusElement.isvalid())
    {
        this->focusElement->OnFocusLost();
        this->focusElement = 0;
    }

    // destroy windows    
    IndexT i;
    for (i = 0; i < this->windows.Size(); i++)
    {
        if (this->windows[i]->IsOpen())
        {
            this->windows[i]->Close();
        }
    }
    this->GarbageCollectClosedWindows();
    n_assert(this->windows.IsEmpty());
	this->uiFactory = 0;
    
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Attaches a new window to the UIServer. This method is exclusively called
    by Window::Open()!
*/
void
UIManager::AttachWindow(const Ptr<Window>& window)
{    
	n_assert(window.isvalid());
    n_assert(InvalidIndex == this->windows.FindIndex(window));
    this->windows.Append(window);
}

//------------------------------------------------------------------------------
/**
    Removes a window from the UIServer. This method is exclusively called
    by Window::Close()!
*/
void
UIManager::RemoveWindow(const Ptr<Window>& window)
{
	n_assert(window.isvalid());
    if (window == this->activeWindow)
    {
        this->activeWindow = 0;
    }
    int index = this->windows.FindIndex(window);
    if (index != InvalidIndex)
    {        
        this->windows.EraseIndex(index);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
UIManager::Render()
{  
	// trigger all windows    
    IndexT i;
    for (i=0; i < this->windows.Size(); i++)
    {
        // first set priority offset
        this->windows[i]->SetDepthOffset(i * WINDOW_Z_OFFSET);
		this->windows[i]->OnRender();
    }    	
}

//------------------------------------------------------------------------------
/**
*/
void
UIManager::RenderDebug()
{  
	// trigger all windows    
    IndexT i;
    for (i=0; i < this->windows.Size(); i++)
    {
    	this->windows[i]->OnRenderDebug();
    }  	
}

//------------------------------------------------------------------------------
/**
    Trigger the ui server.
*/
void
UIManager::Trigger()
{
	// at first handle input
	this->HandleInput();

	// now trigger all windows
    IndexT i;
    for (i=0; i < this->windows.Size(); i++)
    {
        this->windows[i]->OnFrame();
    }  

    // garbage-collect closed windows
    this->GarbageCollectClosedWindows();
}

//-----------------------------------------------------------------------------
/**
*/
void
UIManager::GarbageCollectClosedWindows()
{
    // start at end of array, because array may shrink during loop
    IndexT i;
    for (i = (this->windows.Size() - 1); i >= 0; i--)
    {
        Ptr<Window> curWindow = this->windows[i];
        if (curWindow->IsMarkedForClose())
        {
            curWindow->OnClose();
        }
    }
}   

//-----------------------------------------------------------------------------
/**
	Handle the mouse and keyboard input each frame
*/
void
UIManager::HandleInput()
{    
    if (!this->IsInputEnabled()) return;

	Ptr<Input::Mouse> mouse = Input::InputServer::Instance()->GetDefaultMouse();
	Ptr<Input::Keyboard> keyboard = Input::InputServer::Instance()->GetDefaultKeyboard();

    if (mouse.isvalid())
    {
        // record current mouse position
	    this->mousePosition = mouse->GetScreenPosition();
        this->pixelMousePosition = mouse->GetPixelPosition();

	    // MOUSE: check input and route
        const Ptr<Window>& windowUnderMouse = this->GetWindowUnderMouse();
	    if(windowUnderMouse.isvalid())
	    {
		    Util::Array<Ptr<Element> > elements;

		    // collect all hitted elements
		    windowUnderMouse->GetElementsAt(this->mousePosition, elements);

		    // now distribute the mouse events to the elements
		    SizeT size = elements.Size();
			
		    // check if not over this window
		    if(size <= 0)
		    {
			    return;
		    }

            IndexT index;
            for(index = 0; index < size; index++)
            {
                // ---> mouse is over ;)
                elements[index]->SetMouseOver(true);
  
                // ---> BUTTON DOWN
		        if(mouse->ButtonDown(Input::MouseButton::LeftButton))			elements[index]->OnLeftButtonDown(this->mousePosition);
		        if(mouse->ButtonDown(Input::MouseButton::RightButton))			elements[index]->OnRightButtonDown(this->mousePosition);
		        if(mouse->ButtonDown(Input::MouseButton::MiddleButton))			elements[index]->OnMiddleButtonDown(this->mousePosition);
    			
		        // ---> BUTTON PRESSED
		        if(mouse->ButtonPressed(Input::MouseButton::LeftButton))		elements[index]->OnLeftButtonPressed(this->mousePosition);
		        if(mouse->ButtonPressed(Input::MouseButton::RightButton))		elements[index]->OnRightButtonPressed(this->mousePosition);
		        if(mouse->ButtonPressed(Input::MouseButton::MiddleButton))		elements[index]->OnMiddleButtonPressed(this->mousePosition);
    			
		        // ---> BUTTON UP
		        if(mouse->ButtonUp(Input::MouseButton::LeftButton))				elements[index]->OnLeftButtonUp(this->mousePosition);
		        if(mouse->ButtonUp(Input::MouseButton::RightButton))			elements[index]->OnRightButtonUp(this->mousePosition);
		        if(mouse->ButtonUp(Input::MouseButton::MiddleButton))			elements[index]->OnMiddleButtonUp(this->mousePosition);
    			
		        // ---> BUTTON DOUBLECLICKED
		        if(mouse->ButtonDoubleClicked(Input::MouseButton::LeftButton))	elements[index]->OnLeftButtonDoubleClicked(this->mousePosition);
		        if(mouse->ButtonDoubleClicked(Input::MouseButton::RightButton))	elements[index]->OnRightButtonDoubleClicked(this->mousePosition);
		        if(mouse->ButtonDoubleClicked(Input::MouseButton::MiddleButton))elements[index]->OnMiddleButtonDoubleClicked(this->mousePosition);

		        // ---> WHEEL 
		        if(mouse->WheelForward())										elements[index]->OnMouseWheelMoved(true);
		        if(mouse->WheelBackward())										elements[index]->OnMouseWheelMoved(false);

		        // ---> MOUSE MOVEMENT
		        if(mouse->GetMovement().lengthsq() > N_TINY)					elements[index]->OnMouseMove(this->mousePosition);			
            }
	    }
    }

    if (this->GetFocusElement().isvalid())
    {
        // GAMEPAD: check input and route 
        // TODO: link buttons with neighbor buttons (left, right, bottom, above)
        // TODO: move selection of buttons with gamepad axis
        // TODO: send button a only to focus element    
        IndexT gamepadIdx;
        for (gamepadIdx = 0; gamepadIdx < InputServer::Instance()->GetMaxNumLocalPlayers(); gamepadIdx++)
        {
            if (this->GetActiveWindow().isvalid() && this->GetActiveWindow()->GetCanvas().isvalid())
            {
                // check for axis movement 
                // ATTENTION: INVERT Y VALUE, CAUSE UI COORD SYSTEM IS LIKE SCREENSPACE
                //            0,0 IS UP LEFT and 1,1 IS DOWN!!!!! RIGHT
                const Ptr<GamePad>& gamepad = InputServer::Instance()->GetDefaultGamePad(gamepadIdx);
                float2 inputDir(0,0);
                inputDir.x() += gamepad->GetAxisValue(GamePad::LeftThumbXAxis);
                inputDir.y() -= gamepad->GetAxisValue(GamePad::LeftThumbYAxis);
                inputDir.x() += gamepad->GetAxisValue(GamePad::RightThumbXAxis);
                inputDir.y() -= gamepad->GetAxisValue(GamePad::RightThumbYAxis);
                inputDir.x() += gamepad->ButtonDown(GamePad::DPadLeftButton) ? +1 : 0;
                inputDir.x() += gamepad->ButtonDown(GamePad::DPadRightButton) ? -1 : 0;
                inputDir.y() += gamepad->ButtonDown(GamePad::DPadUpButton) ? -1 : 0;
                inputDir.y() += gamepad->ButtonDown(GamePad::DPadDownButton) ? +1 : 0;
                this->SelectFocusElement(inputDir);

                Util::Array<Ptr<Element> > elements = this->GetActiveWindow()->GetCanvas()->GetChildElements();                      
                if (gamepad.isvalid())
                {
                    IndexT btnIdx;
                    for (btnIdx = 0; btnIdx < GamePad::NumButtons; ++btnIdx)
                    {
                        if (gamepad->ButtonDown((GamePad::Button)btnIdx))
                        {                
                            //for(i = 0; i < size; i++) elements[i]->OnGamePadButtonDown((GamePad::Button)btnIdx);
                            this->focusElement->OnGamePadButtonDown((GamePad::Button)btnIdx);
                        }
                        else if (gamepad->ButtonUp((GamePad::Button)btnIdx))
                        {
                            //for(i = 0; i < size; i++) elements[i]->OnGamePadButtonUp((GamePad::Button)btnIdx);
                            this->focusElement->OnGamePadButtonUp((GamePad::Button)btnIdx);
                        }                	
                    }
                }
            }
        }

	    // KEYBOARD: check input and route 	
		const Util::String& input = keyboard->GetCharInput();
		if(input.IsValid())
		{
			int i = 0;
			i++;
			//this->GetFocusElement()->OnChar(input);
		}
	}
}

//-----------------------------------------------------------------------------
/**
    Check if the mouse is currently over an GUI element.    
*/
bool
UIManager::IsMouseOverGui() const
{
    if(this->GetWindowUnderMouse().isvalid())
    {
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
/**
    Return the window under the mouse
*/
Ptr<Window>
UIManager::GetWindowUnderMouse() const
{
    int i;
    int num = this->windows.Size();
    for (i = 0; i < num; i++)
    {
        if (this->windows[i]->Inside(this->GetMousePosition()))
        {
            return this->windows[i];
        }
    }
    return Ptr<Window>(0);
}

//-----------------------------------------------------------------------------
/**
    This sets the input focus to a given control. All Events will first be sent
    to this object, then to all other control elements. This Method will be called
    by the control's register method. Don't call this method directly
*/
void
UIManager::SetFocusElement(const Ptr<Element>& focus, bool reg)
{
	n_assert(focus.isvalid());
    if (this->focusElement.isvalid())
    {
        this->focusElement->OnFocusLost();
        this->focusElement = 0;
    }
    if (reg)
    {
        this->focusElement = focus;
        this->focusElement->OnFocusGain();
    }
}

//-----------------------------------------------------------------------------
/**
*/
void
UIManager::PutWindowEvent(const Ptr<Window>& window, const Ptr<UIEvent> &event)
{
    window->HandleEvent(event);
    if (this->curEventHandler.isvalid())
    {
        this->curEventHandler->HandleEvent(event);
    }
}

//-----------------------------------------------------------------------------
/**
*/
void
UIManager::CloseAllWindows()
{
    while(this->windows.Size() > 0)
    {
        this->windows[0]->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
UIManager::SelectFocusElement(const float2& cursorDir)
{
    const float deadZone = 0.2f;
    bool switchFocusElement = (n_saturate(n_abs(cursorDir.x()) - deadZone) > 0) ||
                              (n_saturate(n_abs(cursorDir.y()) - deadZone) > 0);        
    if (switchFocusElement)
    {
        Element::NeighbourDirection inputDir;
        Ptr<Element> neighbor;
        if (cursorDir.x() != 0)
        {
            inputDir = cursorDir.x() > 0 ? Element::Right : Element::Left;
            neighbor = this->GetFocusElement()->GetNeighbour(inputDir);
        }
        if (!neighbor.isvalid() && cursorDir.y() != 0)
        {
            inputDir = cursorDir.y() > 0 ? Element::Bottom : Element::Top;
            neighbor = this->GetFocusElement()->GetNeighbour(inputDir);
        }

        if (neighbor.isvalid())
        {
            this->SetFocusElement(neighbor, true);
        }        
    }
}

} // namespace UI
