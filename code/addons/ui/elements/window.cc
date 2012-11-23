//------------------------------------------------------------------------------
//  ui/elements/window.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/elements/window.h"
#include "ui/uimanager.h"
#include "ui/uievent.h"
#include "ui/uirenderer.h"

namespace UI
{
__ImplementClass(Window, 'UIWN', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Window::Window() :
    isOpen(false),
    isHidden(false),
    isClickThrough(false),
    isFading(false),
    isMarkedForClose(false),
    screenAlignmentX(UnalignedX),
    screenAlignmentY(UnalignedY),
    depthOffset(0),
    // @GUI_LEGACY_DSA3
    // default is true, so DSA code will generate its own eventhandler to pass 
    // events to this window
    dsaLegacyMode(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Window::~Window()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
    This will simply call OnOpen(), unless the window has been marked
    for close but has not been removed yet, in this case, no
    initialization needs to happen.
*/
void
Window::Open()
{
    n_assert(!this->isOpen);
    if (!this->isMarkedForClose)
    {
        this->OnOpen();
    }
}

//------------------------------------------------------------------------------
/**
    Call to close the window. This will not actually remove the window,
    just mark it for closing. The UI manager will take care about windows
    which are marked for closing.
*/
void
Window::Close()
{
    this->isMarkedForClose = true;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Opens the window. This will register the window with the
    UI::Server, make the window visible and start to process UI
    messages.
*/
void
Window::OnOpen()
{
    n_assert(!this->isOpen);
    n_assert(!this->isMarkedForClose);

    // @GUI_LEGACY_DSA3
    if(this->dsaLegacyMode)
    {
        n_assert(this->resourceName.IsValid());
        String extRes;
        extRes.Format("%s.xml", this->resourceName.Value());
        UIManager::Instance()->LoadWindow(Ptr<Window>(this), extRes);        
    }

    // register our UI texture
    if (this->windowTexture.IsValid())
    {
        UIRenderer::Instance()->RegisterTexture(this->windowTexture);
    }

    // attach the window to the UI::Server
    UIManager::Instance()->AttachWindow(this);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
    Perform cleanup on closing the window. This will be called by the
    UIManager on windows marked for closing.
*/
void
Window::OnClose()
{
    n_assert(this->isMarkedForClose);
    n_assert(!this->isOpen);

    // unregister our UI texture
    if (this->windowTexture.IsValid())
    {
        UIRenderer::Instance()->UnregisterTexture(this->windowTexture);
    }

    this->isHidden = false;
    this->canvas->OnDestroy();
    this->canvas = 0;      
    this->isMarkedForClose = false;

    // detach the window from the UI::Server
    UIManager::Instance()->RemoveWindow(Ptr<Window>(this));   
}

//------------------------------------------------------------------------------
/**
*/
void
Window::Show()
{
	// open if not already done
    if (!this->IsOpen())
    {
		this->Open();
    }

	// and unhide
    if (this->isHidden)
    {        
        this->canvas->SetVisible(true);
        this->isHidden = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Window::Hide()
{
	// hide
    if (!this->isHidden && this->IsOpen())
    {
        this->canvas->SetVisible(false);
        this->isHidden = true;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called per-frame by the UI::Server as long as the
    window is open. You may override the method to implement
    your own per-frame stuff there.
*/
void
Window::OnFrame()
{
    // fade might close the window (!)
    if (this->IsOpen())
    {
        this->canvas->OnFrame();        
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the UI::Server when the window should
    render itself.
*/
void
Window::OnRender()
{
    if (this->IsOpen() && !this->IsHidden())
    {
        this->canvas->OnRender();
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the UI::Server when the window should
    render in debug mode. Therefore just the shapes will be rendered.
*/
void
Window::OnRenderDebug()
{
    if (this->IsOpen())
    {
        this->canvas->OnRenderDebug();
    }
}

//------------------------------------------------------------------------------
/**
    Return true if the mouse is within the window.
*/
bool
Window::Inside(const Math::float2& mousePos)
{
    if (this->IsShown())
    {
        if (this->canvas->Inside(mousePos))
        {
            if (this->isClickThrough)
            {
                const Array<Ptr<Element> >& canvasChildren = this->canvas->GetChildElements();
                int i;
                for (i = 0; i < canvasChildren.Size(); i++)
                {
                    if (canvasChildren[i]->Inside(mousePos))
                    {
                        return true;
                    }
                }
            }
            else
            {
			    return true;
            }
        }
    }        
    return false;
}

//------------------------------------------------------------------------------
/**
    Calls canvas, to return all elements under the mouse
*/
void
Window::GetElementsAt(const Math::float2& mousePos, Array<Ptr<Element> >& elements_out)
{
	if(this->canvas->Inside(mousePos))
	{
		elements_out.Append(Ptr<Element>(this->canvas));
	}
	return this->canvas->GetElementsAt(mousePos, elements_out);
}

//------------------------------------------------------------------------------
/**
    Calculates the Window center position and updates the canvas position.

    FIXME: this method is broken!
*/
void
Window::UpdateAlignedPosition()
{
/*
	// the pos
	float x = 0;
    float y = 0;

	switch(this->screenAlignmentX)
	{
		case UnalignedX:	x = 0.5f;	break; // TODO.. something here with unaligned windows
		case LeftSide:		x = 0.0f;	break;
		case CenterX:		x = 0.5f;	break;
		case RightSide:		x = 1.0f;	break;
		default:			n_error("Invalid X windowalignment!");
	}
	switch(this->screenAlignmentY)
	{
		case UnalignedY:	y = 0.5f;	break;	// TODO.. something here with unaligned windows
		case UpSide:		y = 0.0f;	break;
		case CenterY:		y = 0.5f;	break;
		case DownSide:		y = 1.0f;	break;
		default:			n_error("Invalid Y windowalignment!");
	}
	
	// Do not change order of following calls
	// init all absolute positions, and move window to final position
	this->canvas->Move(Math::float2(x, y));	
*/
}

//------------------------------------------------------------------------------
/**
*/
String
Window::AlignmentXToString(const UI::Window::ScreenAlignmentX &align)
{
	switch(align)
	{
	case CenterX:		return "CenterX";		break;
	case LeftSide:		return "LeftSide";		break;
	case RightSide:		return "RightSide";		break;
	case UnalignedX:	return "UnalignedX";	break;		
	}
	
	n_error("Wrong X alignment set!");
	// dummy return
	return "";
}

//------------------------------------------------------------------------------
/**
*/
String
Window::AlignmentYToString(const UI::Window::ScreenAlignmentY &align)
{
	switch(align)
	{
	case CenterY:		return "CenterY";		break;
	case UpSide:		return "UpSide";		break;
	case DownSide:		return "DownSide";		break;
	case UnalignedY:	return "UnalignedY";	break;		
	}
	
	n_error("Wrong Y alignment set!");
	// dummy return
	return "";
}


//------------------------------------------------------------------------------
/**
*/
Window::ScreenAlignmentX 
Window::StringToAlignmentX(const String& align)
{
	if("CenterX" == align)			return CenterX;
	else if("LeftSide" == align)	return LeftSide;
	else if("RightSide" == align)	return RightSide;
	else if("UnalignedX" == align)	return UnalignedX;
	
	n_error("Wrong X alignment set!");
	// dummy return
	return UnalignedX;
}

//------------------------------------------------------------------------------
/**
*/
Window::ScreenAlignmentY
Window::StringToAlignmentY(const String& align)
{
	if("CenterY" == align)			return CenterY;
	else if("UpSide" == align)		return UpSide;
	else if("DownSide" == align)	return DownSide;
	else if("UnalignedY" == align)	return UnalignedY;
	
	n_error("Wrong Y alignment set!");
	// dummy return
	return UnalignedY;
}
    
//------------------------------------------------------------------------------
/**
    @GUI_LEGACY_DSA3
*/
void 
Window::HandleEvent(Event* e)
{
    // FIXME
    n_printf("Window::HandleEvent: %s\n", e->GetEventName().Value());
}

//------------------------------------------------------------------------------
/**
    Handle an input event.
*/
bool
Window::HandleInputEvent(const Input::InputEvent& inputEvent)
{
    // FIXME
    n_printf("Window::HandleInputEvent: %s\n", Input::InputEvent::TypeToString(inputEvent.GetType()));
    return true;
}

} // namespace UI
