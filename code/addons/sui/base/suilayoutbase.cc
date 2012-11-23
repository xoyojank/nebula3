//------------------------------------------------------------------------------
//  suilayoutbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/base/suilayoutbase.h"
#include "sui/rt/suiserver.h"

namespace Base
{
__ImplementClass(Base::SUILayoutBase, 'SULB', Core::RefCounted);

using namespace Util;
using namespace SUI;
using namespace Input;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
SUILayoutBase::SUILayoutBase() :
    isValid(false),
    resState(Resource::Initial),
    showTimeStamp(0.0),
    hideTimeStamp(0.0),
    shown(false)
    
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SUILayoutBase::~SUILayoutBase()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
SUILayoutBase::Setup(const StringAtom& lid, const StringAtom& resId)
{
    n_assert(!this->IsValid());
    this->layoutId = lid;
    this->resourceId = resId;
    this->shown = false;
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
SUILayoutBase::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
    Call this method to make the layout visible.
*/
void
SUILayoutBase::Show()
{
    n_assert(this->IsValid());
    if (!this->shown)
    {
        this->shown = true;
        this->showTimeStamp = SUIServer::Instance()->GetTime();        
    }
}

//------------------------------------------------------------------------------
/**
    Call this method to make the layout invisible.
*/
void
SUILayoutBase::Hide()
{
    n_assert(this->IsValid());
    if (this->shown)
    {
        this->shown = false;
        this->hideTimeStamp = SUIServer::Instance()->GetTime();
    }
}

//------------------------------------------------------------------------------
/**
    This method returns true if the layout is currently in the shown state.
*/
bool
SUILayoutBase::IsShown() const
{
    return this->shown;
}

//------------------------------------------------------------------------------
/**
    Start or continue loading the layout resource. Loading should happen
    asynchronously. As long as the IsPending() method returns true, the
    app should continue to call Load() until IsPending() returns false.
    If loading has failed, the LoadFailed() method will return true.
*/
void
SUILayoutBase::Load()
{
    // override in subclass!
}

//------------------------------------------------------------------------------
/**
*/
bool
SUILayoutBase::IsPending() const
{
    return Resource::Pending == this->resState;
}

//------------------------------------------------------------------------------
/**
*/
bool
SUILayoutBase::LoadFailed() const
{
    return Resource::Failed == this->resState;
}

//------------------------------------------------------------------------------
/**
    This method must be overwritten in a subclass and return true as soon
    as the layout is ready to be displayed.
*/
bool
SUILayoutBase::IsShowable() const
{
    return Resource::Loaded == this->resState;
}

//------------------------------------------------------------------------------
/**
    This method takes an array of input events, and produce a resulting 
    SUIEvents which must be appended(!) to the outEvents array. The method
    must return the number of produced SUIEvents.
*/
SizeT
SUILayoutBase::ProcessInputEvents(const Array<InputEvent>& inputEvents, Array<SUIEvent>& outEvents)
{
    return 0;
}

//------------------------------------------------------------------------------
/**
    This method is called once per frame on ALL layouts (visible or not), and
    is guaranteed to be only called once per frame. This is where layout
    animation and other dynamic effects should happen.
*/
void
SUILayoutBase::Update()
{
    // override in subclass!
}

//------------------------------------------------------------------------------
/**
    This method is called when the layout should render itself. The method
    may be called several times per frame.
    NOTE that the method will even be called, when the layout is not currently
    set to shown state. The method must decide on its own whether it wants
    to render itself.
*/
void
SUILayoutBase::Render(const Ptr<Frame::FrameBatch>& frameBatch)
{
    // override in subclass!
}

//------------------------------------------------------------------------------
/**
    Invoke an action on the layout. This is the only way to manipulate the
    contents of the from the outside.
*/
void
SUILayoutBase::InvokeAction(const Ptr<SUIAction>& action)
{
    n_printf("SUIAction: %s (layout=%s, element=%s)\n", 
        action->GetClassName().AsCharPtr(),
        action->GetLayoutId().Value(),
        action->GetElementId().Value());
}


} // namespace Base