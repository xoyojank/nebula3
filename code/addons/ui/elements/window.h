#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::Window

    Base class for windows. A window generally has exactly one canvas
    attached.

    Override this class to implement windows with new behaviour.
    The window class generally implements only functionality, not layout!
	The virtual position of the window is in the center of the screen.
	The alignment descripes the center position of the canvas.

    //If Activate() is called before Open() (the window has already been attached
    //to server for receiving messages) it will not be deactivated on Close(). So
    //the window is able to receive further messages after closing.


    Notes: @GUI_LEGACY_DSA3 Legacy Changes for DSA GUI
    ---------------------------------

    1) DSA expects events to be handled in

        Window::HandleEvent 

    2) addons/ui handles events here
     
       UIEventHandler::HandleEvent


    (C) 2008 Radon Labs GmbH
*/
#include "messaging/port.h"
#include "ui/elements/canvas.h"
#include "ui/event.h"
#include "input/inputevent.h"

//------------------------------------------------------------------------------
namespace UI
{
class Window : public Core::RefCounted
{
    __DeclareClass(Window);
public:
	
    enum ScreenAlignmentX
    {
        UnalignedX = 0,
        LeftSide,
        CenterX,
        RightSide
    };
    enum ScreenAlignmentY
    {
        UnalignedY = 0,
        UpSide,
        CenterY,
        DownSide
    };

    /// constructor
    Window();
    /// destructor
    virtual ~Window();

    /// open the window at position
    void Open();
    /// close the window (marks the window for remove)
    void Close();
    /// return true if window is open
    bool IsOpen() const;
    /// hide the window
    virtual void Hide();
    /// shows the window (will open if needed)
    virtual void Show();
    /// return true if window is hidden
    bool IsHidden() const;
    /// return true if window is open and not hidden
    bool IsShown() const;
    /// return true if window has been marked for closing (but hasn't cleaned up yet)
    bool IsMarkedForClose() const;

    /// get the canvas of the window
    const Ptr<Canvas>& GetCanvas() const;    

    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug (just the shapes of window and all elements)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area (just checks the canvas)
	virtual bool Inside(const Math::float2& mousePos);
	/// returns all elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element> >& elements_out);

	/// set screen alignment
    void SetScreenAlignmentX(ScreenAlignmentX align);
    /// set screen alignment
    ScreenAlignmentX GetScreenAlignmentX() const;
    /// set screen alignment
    void SetScreenAlignmentY(ScreenAlignmentY align);
    /// set screen alignment
    ScreenAlignmentY GetScreenAlignmentY() const;

    // @GUI_LEGACY_DSA3 BEGIN
    // DSA inherits from Window and overwrites this method
    virtual void HandleEvent(Event* e);
    /// handle an Input event
    virtual bool HandleInputEvent(const Input::InputEvent& inputEvent);
    // dsa code needs to create a default event handler etc.
    // look at the description at the declaration of Window::dsaLegacyMode
    void SetDsaLegacyMode(bool _dsaLegacyMode);
    /// set the xml resource for the window
    void SetResource(const Util::StringAtom& res);
    /// get the xml resource for the window
    const Util::StringAtom& GetResource() const;
    // @GUI_LEGACY_DSA3 END

	/// convert x align to string
	static Util::String AlignmentXToString(const ScreenAlignmentX& align);
	/// convert y align to string
	static Util::String AlignmentYToString(const ScreenAlignmentY& align);
	/// convert string to x align
	static ScreenAlignmentX StringToAlignmentX(const Util::String& align);
	/// convert string to y align
	static ScreenAlignmentY StringToAlignmentY(const Util::String& align);

	/// set the window title
	void SetWindowTitle(const Util::String& title);
	/// get the window title
	const Util::String& GetWindowTitle() const;

    /// set the windowtexture
    void SetWindowTexture(const Resources::ResourceId& texture);
    /// get the windowtexture
    const Resources::ResourceId& GetWindowTexture() const;

    /// set depth offset (for window ordering)
    void SetDepthOffset(int z);
    /// get the additional z offset
    int GetDepthOffset() const;

    /// set click-through flag
    void SetClickThrough(bool b);
    /// get click-through flag
    bool IsClickThrough() const;

    /// set fading flag
    void SetFading(bool b);
    /// get fading flag
    bool IsFading() const;

protected:
	friend class UIXmlParser;
	friend class UIManager;

    /// handle open actions
    virtual void OnOpen();
    /// handle close actions
    virtual void OnClose();
	/// set the canvas of the window
    void SetCanvas(const Ptr<Canvas>& canvas);
	/// fix screen position according to alignment
	void UpdateAlignedPosition();
	
    bool isOpen;    
    bool isHidden;
    bool isClickThrough;
    bool isFading;
    bool isMarkedForClose;

	Util::String windowTitle;
    Resources::ResourceId windowTexture;

    Ptr<Canvas> canvas;

    ScreenAlignmentX screenAlignmentX;
    ScreenAlignmentY screenAlignmentY;

    /// the additional offset position (for moving windows in background...)
    int depthOffset;               

    // @GUI_LEGACY_DSA3
    // dsaLegacyMode -> true:
    //    * window loads itsself in Open, SetResource must be called before
    //    * window create its own Event Handler, which redirects the events to Window::HandleEvent
    //      which is needed by DSA
    bool dsaLegacyMode;
    Util::StringAtom resourceName;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsMarkedForClose() const
{
    return this->isMarkedForClose;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetFading(bool b)
{
    this->isFading = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsFading() const
{
    return this->isFading;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetClickThrough(bool b)
{
    this->isClickThrough = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsClickThrough() const
{
    return this->isClickThrough;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetDepthOffset(int z)
{
    this->depthOffset = z;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Window::GetDepthOffset() const
{
    return this->depthOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetWindowTitle(const Util::String& title)
{
	this->windowTitle = title;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
Window::GetWindowTitle() const
{
    return this->windowTitle;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetCanvas(const Ptr<Canvas>& canvas)
{
	this->canvas = canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Canvas>&
Window::GetCanvas() const
{
    return this->canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsHidden() const
{
    if (this->isOpen && this->isHidden)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Window::IsShown() const
{
    if (this->isOpen && !this->isHidden)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
Window::SetScreenAlignmentX(Window::ScreenAlignmentX align)
{
    this->screenAlignmentX = align;
}

//------------------------------------------------------------------------------
/**
*/
inline Window::ScreenAlignmentX 
Window::GetScreenAlignmentX() const
{
    return this->screenAlignmentX;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Window::SetScreenAlignmentY(Window::ScreenAlignmentY align)
{
    this->screenAlignmentY = align;
}

//------------------------------------------------------------------------------
/**
*/
inline Window::ScreenAlignmentY 
Window::GetScreenAlignmentY() const
{
    return this->screenAlignmentY;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetWindowTexture(const Resources::ResourceId& texture)
{
	this->windowTexture = texture;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
Window::GetWindowTexture() const
{
    return this->windowTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Window::SetDsaLegacyMode(bool _dsaLegacyMode)
{
    this->dsaLegacyMode = _dsaLegacyMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Window::SetResource(const Util::StringAtom& res)
{
    this->resourceName = res;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom& 
Window::GetResource() const
{
    return this->resourceName;
}


}; // namespace UI
//------------------------------------------------------------------------------
