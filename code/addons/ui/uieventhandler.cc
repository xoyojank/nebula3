//------------------------------------------------------------------------------
//  ui/uieventhandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uieventhandler.h"
#include "ui/uievent.h"
#include "ui/uimanager.h"

namespace UI
{
__ImplementClass(UIEventHandler, 'UIEH', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::UIEventHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::~UIEventHandler ()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    The HandleEvent() method is the front end for your UI logic implementation.
    All user interface events emitted by the current UI will be routed to
    this method.
*/
void
UIEventHandler::HandleEvent(const Ptr<UIEvent>& e)
{
    // empty
}

}; // namespace UI