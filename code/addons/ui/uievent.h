#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIEvent
    
    User interface events are emitted by user interface and handled by the
    currently registered UI event handler. UIs usually consist of 2 parts,
    the user interface itself (a hierarchy of UI::Element objects, constructed
    from an XUI file) which represents the visual part, and an event handler 
    which implements the logic behind the UI.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "messaging/message.h"
#include "ui/elements/element.h"
#include "input/inputevent.h"
#include "input/mousebutton.h"
#include "input/key.h"
#include "input/gamepad.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIEvent : public Messaging::Message
{
	__DeclareClass(UIEvent);
    __DeclareMsgId;
public:

    /// constructor
    UIEvent();

	/// returns the elements id (uses the element pointer, set this one)
	const Util::StringAtom& GetElementId() const;
    // get event name
    const Util::StringAtom& GetEventName() const;

	/// set pointer to element which emitted the event
    void SetElement(const Ptr<Element>& e);
    /// get pointer to element which emitted the event
    const Ptr<Element>& GetElement() const;

	/// sets the event type
	void SetEventType(Input::InputEvent::Type type);
	/// gets the event type
	const Input::InputEvent::Type& GetEventType() const;

	/// if the event type was a mouse event, this sets the mouse button
	void SetMouseButton(Input::MouseButton::Code code);
	/// get mouse button if this is a mouse event
	const Input::MouseButton::Code& GetMouseButton() const;

	/// if the event type was a key event, this sets the key
	void SetKey(Input::Key::Code code);
	/// get key if this is a keyboard event
	const Input::Key::Code& GetKey() const;

    /// if the event type was a gamepad button event, this sets the button
	void SetGamePadButton(Input::GamePad::Button code);
	/// get button if this is a gamepad event
	const Input::GamePad::Button& GetGamePadButton() const;

    /// if the event type was a gamepad axis event, this sets the axis
	void SetGamePadAxis(Input::GamePad::Axis code);
	/// get axis if this is a gamepad event
	const Input::GamePad::Axis& GetGamePadAxis() const;

private:
    Ptr<Element> element;
	Input::InputEvent::Type eventType;
	Input::MouseButton::Code mouseButton;
	Input::Key::Code key;
    Input::GamePad::Button padBtn;
    Input::GamePad::Axis padAxis;
};

//------------------------------------------------------------------------------
/**
*/
inline void
UIEvent::SetMouseButton(Input::MouseButton::Code code)
{
	this->mouseButton = code;
}

//------------------------------------------------------------------------------
/**
*/
inline const Input::MouseButton::Code&
UIEvent::GetMouseButton() const
{
	return this->mouseButton;
}

//------------------------------------------------------------------------------
/**
*/
inline void
UIEvent::SetKey(Input::Key::Code code)
{
	this->key = code;
}

//------------------------------------------------------------------------------
/**
*/
inline const Input::Key::Code&
UIEvent::GetKey() const
{
	return this->key;
}

//------------------------------------------------------------------------------
/**
*/
inline void
UIEvent::SetEventType(Input::InputEvent::Type type)
{
	this->eventType = type;		
}

//------------------------------------------------------------------------------
/**
*/
inline const Input::InputEvent::Type&
UIEvent::GetEventType() const
{
	return this->eventType;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
UIEvent::GetElementId() const
{
	n_assert(this->element.isvalid());
	return this->element->GetId();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom& 
UIEvent::GetEventName() const
{
	n_assert(this->element.isvalid());
    return this->element->GetEventName();
}

//------------------------------------------------------------------------------
/**
*/
inline void
UIEvent::SetElement(const Ptr<Element>& e)
{
    this->element = e;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Element>&
UIEvent::GetElement() const
{
    return this->element;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIEvent::SetGamePadButton(Input::GamePad::Button code)
{
    this->padBtn = code;
}

//------------------------------------------------------------------------------
/**
*/
inline const Input::GamePad::Button& 
UIEvent::GetGamePadButton() const
{
    return this->padBtn;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
UIEvent::SetGamePadAxis(Input::GamePad::Axis code)
{
    this->padAxis = code;
}

//------------------------------------------------------------------------------
/**
*/
inline const Input::GamePad::Axis& 
UIEvent::GetGamePadAxis() const
{
    return this->padAxis;
}

}; // namespace UI
//------------------------------------------------------------------------------
