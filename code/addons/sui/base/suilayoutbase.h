#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::SUILayoutBase

    Base class for SUI layouts.

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "sui/suievent.h"
#include "resources/resourceid.h"
#include "resources/resource.h"
#include "input/inputevent.h"
#include "timing/time.h"
#include "sui/suiactions.h"
#include "frame/framebatch.h"

//------------------------------------------------------------------------------
namespace Base
{
class SUILayoutBase : public Core::RefCounted
{
    __DeclareClass(SUILayoutBase);
public:
    /// constructor
    SUILayoutBase();
    /// destructor
    virtual ~SUILayoutBase();

    /// setup the layout
    void Setup(const Util::StringAtom& layoutId, const Resources::ResourceId& resId);
    /// discard the layout
    void Discard();
    /// return true if the layout has been setup
    bool IsValid() const;

    /// get the layout's id
    const Util::StringAtom& GetLayoutId() const;
    /// get the layout's resource id
    const Resources::ResourceId& GetResourceId() const;

    /// load layout from disc
    void Load();
    /// return true if load is still pending (call Load() again if not)
    bool IsPending() const;
    /// return true if loading has failed
    bool LoadFailed() const;

    /// return true if the layout is ready to be displayed
    bool IsShowable() const;
    /// show the layout 
    void Show();
    /// hide the layout (may fade out)
    void Hide();
    /// return true if currently shown
    bool IsShown() const;
    
    /// invoke an action on the layout
    void InvokeAction(const Ptr<SUI::SUIAction>& action);
    /// process input events, and append to outEvents array, returns number of generated events
    SizeT ProcessInputEvents(const Util::Array<Input::InputEvent>& inputEvents, Util::Array<SUI::SUIEvent>& outEvents);
    /// update the layout (animation, etc...)
    void Update();
    /// render the layout
    void Render(const Ptr<Frame::FrameBatch>& frameBatch);

protected:
    Resources::Resource::State resState;
    Util::StringAtom layoutId;
    Resources::ResourceId resourceId;
    Timing::Time showTimeStamp;
    Timing::Time hideTimeStamp;
    bool shown;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SUILayoutBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
SUILayoutBase::GetLayoutId() const
{
    return this->layoutId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
SUILayoutBase::GetResourceId() const
{
    return this->resourceId;
}

} // namespace Base
//------------------------------------------------------------------------------

