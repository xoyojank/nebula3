//------------------------------------------------------------------------------
//  suiserverbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/base/suiserverbase.h"

namespace Base
{
__ImplementClass(Base::SUIServerBase, 'SUSB', Core::RefCounted);
__ImplementSingleton(Base::SUIServerBase);

using namespace Util;
using namespace Resources;
using namespace SUI;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
SUIServerBase::SUIServerBase() :
    isValid(false),
    curTime(0.0),
    curTicks(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SUIServerBase::~SUIServerBase()
{
    n_assert(!this->IsValid());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::Discard()
{
    n_assert(this->IsValid());

    // discard existing layouts
    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        this->layouts.ValueAtIndex(i)->Discard();
    }
    this->layouts.Clear();

    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
    Create a new layout object and start loading it. The loading usually
    is asynchronously (also see SUIServerBase::Update() for details).
*/
Ptr<SUILayout>
SUIServerBase::CreateLayout(const StringAtom& layoutId, const ResourceId& resId)
{
    if(!this->HasLayout(layoutId))
    {
        Ptr<SUILayout> layout = SUILayout::Create();
        layout->Setup(layoutId, resId);
        layout->Load();
        this->layouts.Add(layoutId, layout);
        return layout;
    }
    else
    {
        return this->layouts[layoutId];    
    }
}

//------------------------------------------------------------------------------
/**
    Destroy an existing layout object.
*/
void
SUIServerBase::DestroyLayout(const StringAtom& layoutId)
{
    n_assert(this->HasLayout(layoutId));
    if(this->layouts[layoutId]->IsShown())
    {
        this->HideLayout(layoutId);
    }
    this->layouts[layoutId]->Discard();
    this->layouts.Erase(layoutId);
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::ShowLayout(const StringAtom& layoutId, int zPrio)
{
    n_assert(this->HasLayout(layoutId));
    n_assert(!this->layouts[layoutId]->IsPending());
    KeyValuePair<int, Ptr<SUILayout> > pair(zPrio, this->layouts[layoutId]);
    
    // assert that the specified layout is not in the visible layouts list
    bool found = false;
    IndexT index;
    for ( index = 0; index < this->visibleLayouts.Size(); index++)
    {
        if( this->visibleLayouts[index].Value() == this->layouts[layoutId] )
        {
            found = true;
            break;    
        }
    }
    n_assert(!found);
    
    this->layouts[layoutId]->Show();    
    this->visibleLayouts.Append(pair);
    this->visibleLayouts.Sort();
}

//------------------------------------------------------------------------------
/**
*/
void
SUIServerBase::HideLayout(const StringAtom& layoutId)
{
    n_assert(this->HasLayout(layoutId));
    
    // assert that the specified layout is in the visible layouts list
    bool found = false;
    IndexT index;
    for ( index = 0; index < this->visibleLayouts.Size(); index++)
    {
        if( this->visibleLayouts[index].Value() == this->layouts[layoutId] )
        {
            found = true;
            break;    
        }
    }
    n_assert(found);
    
    this->layouts[layoutId]->Hide();
    this->visibleLayouts.EraseIndex(index);
    this->visibleLayouts.Sort();
}

//------------------------------------------------------------------------------
/**
*/
bool
SUIServerBase::HasLayout(const StringAtom& layoutId) const
{
    return this->layouts.Contains(layoutId);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<SUILayout>
SUIServerBase::GetLayoutById(const StringAtom& layoutId) const
{
    return this->layouts[layoutId];
}

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<SUILayout> >
SUIServerBase::GetLayouts() const
{
    return this->layouts.ValuesAsArray();
}

//------------------------------------------------------------------------------
/**
    This calls the ProcessInputEvents() on ALL layouts which are not pending. 
    A layout should return any resulting SUIEvents in an array, which will then 
    be merged to one big array by this method.
*/
Array<SUIEvent>
SUIServerBase::ProcessInputEvents(const Array<InputEvent>& inputEvents)
{
    Array<SUIEvent> result;
    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->layouts.ValueAtIndex(i);
        if (curLayout->IsShowable())
        {        
            curLayout->ProcessInputEvents(inputEvents, result);
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    This method will be called exactly once per frame. Layouts should
    realize animation and other effects in this method. The method will
    also call the Load() method on all layouts which are still pending.
*/
void
SUIServerBase::Update()
{
#ifdef _DEBUG
    if (!this->messagesPerFrame.IsEmpty())
    {
        n_printf("--------- Print Messages per Frame! ---------\n");
        Util::Array<String> messageKeys = this->messagesPerFrame.KeysAsArray();
        
        for (IndexT i = 0; i < messageKeys.Size(); i++)
        {
            n_printf("Message Id: %s\n", messageKeys[i].AsCharPtr());
            
            Util::Array<msgElement> values = this->messagesPerFrame[messageKeys[i]];
            while (values.Size() > 0)
            {
                n_printf("Layout: %s Element: %s\n", values[0].layoutId.Value(), values[0].elementId.Value());
                values.EraseIndex(0);
            }
            
            this->messagesPerFrame.Erase(messageKeys[i]);
        }
        
        this->messagesPerFrame.Clear();
    }
#endif
    
    // first update times, do a clean timer start in the first frame
    if (!this->timer.Running())
    {
        this->timer.Start();
    }
    this->curTime = this->timer.GetTime();
    this->curTicks = this->timer.GetTicks();

    IndexT i;
    for (i = 0; i < this->layouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->layouts.ValueAtIndex(i);
        if (curLayout->LoadFailed())
        {
            // loading the layout has failed
            n_error("SUIServer: loading layout '%s' from res '%s' failed!\n", 
                curLayout->GetLayoutId().Value(),
                curLayout->GetResourceId().Value());
        }
        else if (curLayout->IsPending())
        {
            // layout still loading... continue to call Load()
            curLayout->Load();
        }
        else
        {
            curLayout->Update();
        }
    }
    
/*    for (i = 0; i < this->visibleLayouts.Size(); i++)
    {
        const Ptr<SUILayout>& curLayout = this->visibleLayouts[i].Value();    
        // layout is ready, call its update method
        curLayout->Update();
    }*/
}

//------------------------------------------------------------------------------
/**
    This method will call the Render() method on all showable layouts. Note
    that Render() will even be called on layouts which are not in the 
    shown state, the layout object needs to decide itself whether it
    wants to render something!
*/
void
SUIServerBase::Render(const Ptr<Frame::FrameBatch>& frameBatch)
{
    IndexT i;
    for (i = 0; i < this->visibleLayouts.Size(); i++)
    {
        if (visibleLayouts[i].Value()->IsShowable())
        {
            visibleLayouts[i].Value()->Render(frameBatch);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Invokes an action on the UI. This is the only way to manipulate the state 
    of the GUI from the outside.
*/
void
SUIServerBase::InvokeAction(const Ptr<SUIAction>& action)
{

/*
#ifdef _DEBUG 
    msgElement msg; 
    msg.layoutId = action->GetLayoutId();
    msg.elementId = action->GetElementId(); 
#endif
  */
    
    // distribute the action to all layers
    if(action->GetLayoutId().AsString().IsEmpty())
    {
        for (SizeT index = 0; index < this->visibleLayouts.Size(); index++)
        {
            this->visibleLayouts[index].Value()->InvokeAction(action);
        }
    }
    else
    {     
        // distribute the action to the right layout object
        IndexT layoutIndex = this->layouts.FindIndex(action->GetLayoutId());
        if (InvalidIndex != layoutIndex)
        {
            this->layouts.ValueAtIndex(layoutIndex)->InvokeAction(action);
        }
        else
        {
            n_printf("SUIServer::InvokeAction() WARNING: layout doesn't exist: '%s'\n", action->GetLayoutId().Value());
        }
    }

/*
#ifdef _DEBUG
    if (this->messagesPerFrame.Contains(action->GetRtti()->GetName()))
    {
        this->messagesPerFrame[action->GetRtti()->GetName()].Append(msg);
    }
    else
    {
        Array<msgElement> ta;
        ta.Append(msg);
        this->messagesPerFrame.Add(action->GetRtti()->GetName(), ta);
    }
#endif
*/
}

} // namespace SUI
