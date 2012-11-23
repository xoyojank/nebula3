//------------------------------------------------------------------------------
//  suirendermodule.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/suirendermodule.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"
#include "sui/rt/suirtplugin.h"

namespace SUI
{
__ImplementClass(SUI::SUIRenderModule, 'SURM', RenderModules::RenderModule);
__ImplementSingleton(SUI::SUIRenderModule);

using namespace RenderModules;
using namespace Graphics;
using namespace Messaging;
using namespace Util;
using namespace Resources;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
SUIRenderModule::SUIRenderModule()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SUIRenderModule::~SUIRenderModule()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // register the SUI render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&SUIRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::Discard()
{
    n_assert(this->IsValid());

    // unregister the SUI render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&SUIRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
    
    RenderModule::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::CreateLayout(const StringAtom& layoutId, const ResourceId& resId)
{    
    n_assert(this->IsValid());
    
    Ptr<SUI::CreateLayout> msg = SUI::CreateLayout::Create();
    msg->SetLayoutId(layoutId);
    msg->SetResourceId(resId);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
    this->pendingCreateLayoutMsgs.Append(msg);
}

//------------------------------------------------------------------------------
/**
    Check if a freshly created layout has finished loading.
*/
bool
SUIRenderModule::IsLayoutLoaded(const StringAtom& layoutId) const
{
    // if the layout isn't in the array of pending layouts, it has finished
    // loading
    IndexT i;
    for (i = 0; i < this->pendingCreateLayoutMsgs.Size(); i++)
    {
        if (this->pendingCreateLayoutMsgs[i]->GetLayoutId() == layoutId)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::DestroyLayout(const StringAtom& layoutId)
{
    n_assert(this->IsValid());
    
    Ptr<SUI::DestroyLayout> msg = SUI::DestroyLayout::Create();
    msg->SetLayoutId(layoutId);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::ShowLayout(const StringAtom& layoutId, int zPrio)
{
    n_assert(this->IsValid());
    
    Ptr<SUI::ShowLayout> msg = SUI::ShowLayout::Create();
    msg->SetLayoutId(layoutId);
    msg->SetZPrio(zPrio);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::HideLayout(const StringAtom& layoutId)
{
    n_assert(this->IsValid());
    
    Ptr<SUI::HideLayout> msg = SUI::HideLayout::Create();
    msg->SetLayoutId(layoutId);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::InvokeAction(const StringAtom& layoutId, const Ptr<SUIAction>& action)
{
    action->SetLayoutId(layoutId);

    Ptr<SUI::InvokeAction> msg = SUI::InvokeAction::Create();
    msg->SetAction(action);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRenderModule::SendInputEvents(const Array<InputEvent>& inputEvents)
{
    // accumulate input events for one frame, and send them as batch 
    // during OnFrame()
    this->inputEvents.AppendArray(inputEvents);
}

//------------------------------------------------------------------------------
/**
*/
bool
SUIRenderModule::HasEventOccured(const SUIEvent& suiEvent) const
{
    int index = this->suiEvents.FindIndex(suiEvent);
    if(InvalidIndex != index)
    {
        return true;
    }
    
    return false;
}

//------------------------------------------------------------------------------
/**
*/
Array<SUIEvent>
SUIRenderModule::GetMatchingEvents(const SUIEvent& suiEvent) const
{
    Array<SUIEvent> events;
    for(IndexT index = 0; index < this->suiEvents.Size(); index++)
    {
        if(this->suiEvents[index] == suiEvent)
        {
            SUIEvent newEvent(suiEvent);
            newEvent.SetInputEvent(this->suiEvents[index].GetInputEvent());
            events.Append(newEvent);
        }
    }
    return events;
}

//------------------------------------------------------------------------------
/**
    The OnFrame() method sends input events and receives SUI events.
*/
void
SUIRenderModule::OnFrame()
{
    // check the pending CreateLayout messages whether they
    // have been handled, this means that their Layout has finished loading
    // start from the end of the array since we're removing elements
    IndexT i;
    for (i = this->pendingCreateLayoutMsgs.Size() - 1; i != InvalidIndex; --i)
    {
        if (this->pendingCreateLayoutMsgs[i]->Handled())
        {
            // message has been handled, remove it from the pending array
            this->pendingCreateLayoutMsgs.EraseIndex(i);
        }
    }
    
    // clear SUI events from previous frame
    this->suiEvents.Clear();

    // check if the previous message has been handled, and get
    // resulting SUI events
    if (this->processEventsMsg.isvalid() && this->processEventsMsg->Handled())
    {        
        this->suiEvents = this->processEventsMsg->GetSUIEvents();
        this->processEventsMsg = 0;
    }

    // may not send new message before previous has been handled
    if (!this->processEventsMsg.isvalid() && this->inputEvents.Size() > 0)
    {
        this->processEventsMsg = ProcessEvents::Create();
        this->processEventsMsg->SetInputEvents(this->inputEvents);
        GraphicsInterface::Instance()->Send(this->processEventsMsg.cast<Message>());
        this->inputEvents.Clear();
    }
}

} // namespace SUI