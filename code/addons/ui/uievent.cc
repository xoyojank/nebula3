//------------------------------------------------------------------------------
//  ui/uievent.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uievent.h"

namespace UI
{
__ImplementClass(UIEvent, 'UIEV', Messaging::Message);
__ImplementMsgId(UIEvent);

//------------------------------------------------------------------------------
/**
*/
UIEvent::UIEvent() :	
    eventType(Input::InputEvent::InvalidType),
    mouseButton(Input::MouseButton::InvalidMouseButton),
    key(Input::Key::InvalidKey),
    padBtn(Input::GamePad::InvalidButton),
    padAxis(Input::GamePad::InvalidAxis)
{
    // empty
}

} // namespace UI
