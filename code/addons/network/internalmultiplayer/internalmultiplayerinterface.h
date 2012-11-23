#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalMultiplayerInterface

    The MultiplayerInterface is created from Mainthread and starts the NetworkThread.
    Any Message send to will be handled by the InternalMultiplayerHandler.
      
    (C) 2009 Radon Labs GmbH
*/    
#include "interface/interfacebase.h"
#include "core/singleton.h"
#include "messaging/batchmessage.h"
#include "internalmultiplayer/internalmultiplayerhandler.h"

//------------------------------------------------------------------------------
namespace InternalMultiplayer
{
class InternalMultiplayerInterface : public Interface::InterfaceBase
{
    __DeclareClass(InternalMultiplayerInterface);
    __DeclareInterfaceSingleton(InternalMultiplayerInterface);
public:
    /// constructor
    InternalMultiplayerInterface();
    /// destructor
    virtual ~InternalMultiplayerInterface();

    /// open the interface object
    virtual void Open();
    /// close the interface object
    virtual void Close();
    
    /// send a batched messages (call FlushBatchedMessages() to send them to render thread)
    void SendBatched(const Ptr<Messaging::Message>& msg);
    /// flush batched messages, call before WaitForFrameEvent()
    void FlushBatchedMessages();

    /// wait for the InternalMultiplayer thread to process messages
    void WaitForFrameEvent();

protected:
    friend class InternalMultiplayerHandler;

    /// signal the per-frame event, called by InternalMultiplayerHandler
    void SignalFrameEvent();
    /// signal the resource loaded event, called by InternalMultiplayerHandler
    void SignalPendingResourceEvent();

    Threading::ThreadId mainThreadId;
    Threading::Event frameEvent;
    Ptr<InternalMultiplayerHandler> internalMultiplayerHandler;
    Ptr<Messaging::BatchMessage> batchMessage;
};

} // namespace InternalMultiplayer
//------------------------------------------------------------------------------
