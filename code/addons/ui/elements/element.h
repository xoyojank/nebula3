#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::Element

    The base class for all 2D user interface elements describes a rectangle
    and implements a hierarchy. The position describes the center of the window.
	Sizes are set around this center. All screen values are in the (0|1) space.

						|------------------------------------|--- top
						|				  .					 |
						|				  .					 |
						|.................x..................|...... centerY
						|				  .					 |
						|				  .					 |					
						|------------------------------------|--- bottom
						|				  .					 |
					   left				  .					right				
										centerX

    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/rectangle.h"
#include "input/key.h"
#include "input/gamepad.h"
#include "math/float2.h"
#include "math/rectangle.h"
#include "resources/resourceid.h"

//------------------------------------------------------------------------------
namespace UI
{

class Window;

class Element : public Core::RefCounted
{
    __DeclareClass(Element);
public:
    enum NeighbourDirection
    {
        Top = 0,
        Bottom,
        Left,
        Right,

        NumDirections
    };
    /// constructor
    Element();
    /// destructor
    virtual ~Element();
    
	/// called when gui hierarchy is created
	virtual void OnCreate();
    /// called when gui hierarchy is destroyed
    virtual void OnDestroy();
	/// return true if element is valid (between OnCreate() and OnDestroy())
    bool IsValid() const;

    /// set id
    void SetId(const Util::StringAtom& i);
    /// get id
    const Util::StringAtom& GetId() const;
    /// set optional event name
    void SetEventName(const Util::StringAtom& n);
    /// get optional event name
    const Util::StringAtom& GetEventName() const;

    /// get element id
    IndexT GetElementId() const;

    /// set visible flag of the element
    virtual void SetVisible(bool b);
    /// get visible flag of the element
    bool IsVisible() const;   	

    /// set screen space rectangle
    void SetScreenSpaceRect(const Math::rectangle<float>& r);
    /// get screen space rectangle
    const Math::rectangle<float>& GetScreenSpaceRect() const;

	/// set the default color
	void SetColor(const Math::float4& color);
	/// get the default color
	const Math::float4& GetColor() const;

	/// set the default uv coord
	void SetUV(const Math::float4& uv);
	/// get the default uv coord
	const Math::float4& GetUV() const;
    
    /// set optional texture
    void SetTexture(const Resources::ResourceId& resId);
    /// get optional texture
    const Resources::ResourceId& GetTexture() const;

    /// attach child element
    void AttachElement(const Ptr<Element>& elm);
    /// remove child element
    void RemoveElement(const Ptr<Element>& elm);
    /// erase the childelements array
    void EraseElement(const Ptr<Element>& elm);

    /// get the number of childelements
    int GetNumChildElements() const;
	/// access to child elements
	const Util::Array<Ptr<Element> >& GetChildElements() const;
    /// acccess to a single child element
    const Ptr<Element>& GetChildElementAt(int index) const;
    /// find a direct child element by name
    Ptr<Element> FindChildElement(const Util::StringAtom& name) const;

    /// get pointer to parent element (can be 0)
    const Ptr<Element>& GetParentElement() const;    
    /// get window that contains this element
    const Ptr<Window>& GetParentWindow() const;
    
    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug yourself (just the shape)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area
    virtual bool Inside(const Math::float2& mousePos);
	/// returns all child elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element> >& elements_out);

    /// enable/disable the element (won't call OnX() methods if disabled)
    void SetEnabled(bool b);
    /// get enabled state of element
    bool IsEnabled() const;

    /// set the mouseover state
    void SetMouseOver(bool b);
    /// get mouse over
    bool IsMouseOver() const;

    /// set click-through flag
    void SetClickThrough(bool b);
    /// get click-through behaviour
    bool IsClickThrough() const;

    /// called when mouse is moved
    virtual void OnMouseMove(const Math::float2& mousePos);
    /// called when mousewheel is moved (false = down, true = up)
    virtual void OnMouseWheelMoved(const bool& up);
    
	/// called when left mouse button is pressed over element
    virtual void OnLeftButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual void OnLeftButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual void OnLeftButtonUp(const Math::float2& mousePos);
    /// called when left mouse button is double clicked
    virtual void OnLeftButtonDoubleClicked(const Math::float2& mousePos);
	
	/// called when left mouse button is pressed over element
    virtual void OnMiddleButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual void OnMiddleButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual void OnMiddleButtonUp(const Math::float2& mousePos);
    /// called when left mouse button is double clicked
    virtual void OnMiddleButtonDoubleClicked(const Math::float2& mousePos);

    /// called when right mouse button is pressed over element
    virtual void OnRightButtonDown(const Math::float2& mousePos);
    /// called when right mouse button is held pressed
    virtual void OnRightButtonPressed(const Math::float2& mousePos);
    /// called when right mouse button is released over element
    virtual void OnRightButtonUp(const Math::float2& mousePos);
    /// called when right mouse button is double clicked
    virtual void OnRightButtonDoubleClicked(const Math::float2& mousePos);

    /// called when a character is input
    virtual void OnChar(const uchar& code);
    /// called when a key is pressed
    virtual void OnKeyDown(Input::Key::Code key);
    /// called when a key is released
    virtual void OnKeyUp(Input::Key::Code key);

    /// called when a gamepad button was pressed
    virtual void OnGamePadButtonDown(Input::GamePad::Button btn);
    /// called when a gamepad button was released
    virtual void OnGamePadButtonUp(Input::GamePad::Button btn);

    /// called when input focus is gained
    virtual void OnFocusGain();
    /// called when input focus is lost
    virtual void OnFocusLost();    

    /// register control as input focus element
    void RegisterAsFocusElement();
    /// unregister control as input focus element
    void UnregisterAsFocusElement();
    /// return true if this is currently the focus element
    bool IsFocusElement() const;

	/// moves this element and all childs
    void Move(const Math::float2& vec);

    /// link neighbours
    void LinkNeighbours();
    /// get neighbor by direction (can return invalid ptr)
    const Ptr<Element>& GetNeighbour(NeighbourDirection dir) const;

protected:
	friend class Window;
	friend class UIXmlParser;

    /// appends an exiting child element to the childs array
    void AppendChildElement(const Ptr<Element>& elem);

	/// set parent element
	void SetParentElement(const Ptr<Element>& parentElement);
	/// set parent window
	void SetParentWindow(const Ptr<Window>& parentWindow);

	/// calculate how deep this element is in the hierarchy
	int CalculateHierarchyDepth() const;

    Util::StringAtom id;    
    Util::StringAtom eventName;
    bool visible;
    bool enabled;       
    bool isValid;     
    IndexT elementId;
    Ptr<Element> parentElement;  
    Ptr<Window> parentWindow;         
    Ptr<Element> neighbours[NumDirections];
	Util::Array<Ptr<Element> > childElements;
    Math::float4 color;
    Math::float4 uv;
	Math::rectangle<float> screenSpaceRect;
    Resources::ResourceId texture;

	Math::float4 renderColor;
	Math::float4 renderUV;
    Resources::ResourceId renderTexture;

    bool mouseOver;
    bool clickThrough;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetClickThrough(bool b)
{
    this->clickThrough = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Element::IsClickThrough() const
{
    return this->clickThrough;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetMouseOver(bool b)
{
    this->mouseOver = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Element::IsMouseOver() const
{
    return this->mouseOver;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetColor(const Math::float4& c)
{
	this->color = c;
    this->renderColor = color;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
Element::GetColor() const
{
	return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetUV(const Math::float4& u)
{
	this->uv = u;
    this->renderUV = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
Element::GetUV() const
{
	return this->uv;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetTexture(const Resources::ResourceId& resId)
{
    this->texture = resId;
    this->renderTexture = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
Element::GetTexture() const
{
    return this->texture;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Element::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetId(const Util::StringAtom& i)
{
    this->id = i;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
Element::GetId() const
{
    return this->id;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetEventName(const Util::StringAtom& n)
{
    this->eventName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
Element::GetEventName() const
{
    return this->eventName;
}

//------------------------------------------------------------------------------
/**
    Get pointer to parent element.
*/
inline const Ptr<Element>&
Element::GetParentElement() const
{
    return this->parentElement;
}


//------------------------------------------------------------------------------
/**
    Set the pointer to the parent element.
*/
inline void
Element::SetParentElement(const Ptr<Element>& parent)
{
	n_assert(parent.isvalid());
	n_assert(!this->parentElement.isvalid());	

	this->parentElement = parent;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetEnabled(bool b)
{
    this->enabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::AppendChildElement(const Ptr<Element>& elem)
{
	n_assert(elem.isvalid());
    this->childElements.Append(elem);
}

//------------------------------------------------------------------------------
/**
    Provides access to the child element array.
*/
inline 
const Util::Array<Ptr<Element> >&
Element::GetChildElements() const
{
    return this->childElements;
}

//------------------------------------------------------------------------------
/**
    Returns the number of child elements
*/
inline int
Element::GetNumChildElements() const
{
    return this->childElements.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
Element::GetElementId() const
{   
    return this->elementId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetScreenSpaceRect(const Math::rectangle<float>& r)
{
    this->screenSpaceRect = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::rectangle<float>& 
Element::GetScreenSpaceRect() const
{
    return this->screenSpaceRect;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Element>& 
Element::GetNeighbour(NeighbourDirection dir) const
{
    return this->neighbours[dir];
}
}; // namespace UI
//------------------------------------------------------------------------------