//------------------------------------------------------------------------------
//  suihandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/rt/suihandler.h"
#include "sui/rt/suiserver.h"

namespace SUI
{
__ImplementClass(SUI::SUIHandler, 'SUHD', Interface::InterfaceHandlerBase);

using namespace Messaging;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::Close()
{
    this->pendingCreateMessages.Clear();
    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool
SUIHandler::HandleMessage(const Ptr<Message>& msg)
{
    if (msg->CheckId(CreateLayout::Id))
    {
        this->OnCreateLayout(msg.cast<CreateLayout>());
    }
    else if (msg->CheckId(DestroyLayout::Id))
    {
        this->OnDestroyLayout(msg.cast<DestroyLayout>());
    }
    else if (msg->CheckId(ShowLayout::Id))
    {        
        this->OnShowLayout(msg.cast<ShowLayout>());
    }
    else if (msg->CheckId(HideLayout::Id))
    {
        this->OnHideLayout(msg.cast<HideLayout>());
    }
    else if (msg->CheckId(ProcessEvents::Id))
    {        
        this->OnProcessEvents(msg.cast<ProcessEvents>());
    }
    else if (msg->CheckId(InvokeAction::Id))
    {
        this->OnInvokeAction(msg.cast<InvokeAction>());
    }
    else
    {
        return false;
    }
    return true;    
}

//------------------------------------------------------------------------------
/**
    Do per-frame work, this simply check the pending CreateLayout messages
    whether their layouts have completely loaded.
*/
void
SUIHandler::DoWork()
{
    this->HandlePendingCreateLayoutMessages();   
}

//------------------------------------------------------------------------------
/**
    Set messages to "deferred handled" which have been waiting for
    their layout to be loaded.
*/
void
SUIHandler::HandlePendingCreateLayoutMessages()
{
    SUIServer* suiServer = SUIServer::Instance();

    // start at the end since we will remove messages from the array
    IndexT i;
    for (i = this->pendingCreateMessages.Size() - 1; i != InvalidIndex; --i)
    {
        // check if the layout has loaded
        const StringAtom& layoutId = this->pendingCreateMessages[i]->GetLayoutId();
        if (suiServer->HasLayout(layoutId))
        {
            Ptr<SUILayout> layout = suiServer->GetLayoutById(layoutId);
            if (layout->IsShowable())
            {
                this->pendingCreateMessages[i]->SetDeferredHandled(true);
                this->pendingCreateMessages.EraseIndex(i);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    NOTE: the create message will switch to handled state when the
    layout has been loaded.
*/
void
SUIHandler::OnCreateLayout(const Ptr<CreateLayout>& msg)
{
    SUIServer::Instance()->CreateLayout(msg->GetLayoutId(), msg->GetResourceId());
    
    // add message to pending layouts array
    msg->SetDeferred(true);
    this->pendingCreateMessages.Append(msg);
}

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::OnDestroyLayout(const Ptr<DestroyLayout>& msg)
{
    SUIServer::Instance()->DestroyLayout(msg->GetLayoutId());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::OnShowLayout(const Ptr<ShowLayout>& msg)
{
    SUIServer::Instance()->ShowLayout(msg->GetLayoutId(), msg->GetZPrio());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::OnHideLayout(const Ptr<HideLayout>& msg)
{
    SUIServer::Instance()->HideLayout(msg->GetLayoutId());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::OnProcessEvents(const Ptr<ProcessEvents>& msg)
{
    msg->SetSUIEvents(SUIServer::Instance()->ProcessInputEvents(msg->GetInputEvents()));
}

//------------------------------------------------------------------------------
/**
*/
void
SUIHandler::OnInvokeAction(const Ptr<InvokeAction>& msg)
{
    SUIServer::Instance()->InvokeAction(msg->GetAction());
}
      
} // namespace SUI
