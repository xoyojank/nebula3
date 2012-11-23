#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIEventHandler
    
    An UI event handler implements the logic behind an user interface. It is
    implemented as a Message::Port which accepts messages of the type 
    UI::Event. Derive a subclass to implement your UI logic inside
    the HandleEvent() method.
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIEvent;

class UIEventHandler : public Core::RefCounted
{
	__DeclareClass(UIEventHandler);
public:
    /// constructor
    UIEventHandler();
    /// destructor 
    virtual ~UIEventHandler();
    /// override this method to handle your user interface events
    virtual void HandleEvent(const Ptr<UIEvent>& e);
};

}; // namespace UI
//------------------------------------------------------------------------------
