#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIHandler
    
    Render-thread message handler for the SUI subsystem.
    
    (C) 2009 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "sui/suiprotocol.h"

//------------------------------------------------------------------------------
namespace SUI
{
class SUIHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(SUIHandler);
public:
    /// called once before shutdown
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();

private:
    /// handle the pending CreateLayout messages (wait for resources to be loaded)
    void HandlePendingCreateLayoutMessages();
    /// handle CreateLayout message
    void OnCreateLayout(const Ptr<CreateLayout>& msg);
    /// handle DestroyLayout message
    void OnDestroyLayout(const Ptr<DestroyLayout>& msg);
    /// handle ShowLayout message
    void OnShowLayout(const Ptr<ShowLayout>& msg);
    /// handle HideLayout message
    void OnHideLayout(const Ptr<HideLayout>& msg);
    /// handle ProcessEvents message
    void OnProcessEvents(const Ptr<ProcessEvents>& msg);
    /// handle InvokeAction message
    void OnInvokeAction(const Ptr<InvokeAction>& msg);
    
    Util::Array<Ptr<CreateLayout> > pendingCreateMessages;
};

} // namespace SUI
//------------------------------------------------------------------------------
