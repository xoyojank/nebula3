//------------------------------------------------------------------------------
//  InternalMultiplayerinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "InternalMultiplayer/InternalMultiplayerinterface.h"
#include "messaging/runthroughhandlerthread.h"

namespace InternalMultiplayer
{
__ImplementClass(InternalMultiplayer::InternalMultiplayerInterface, 'IMIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(InternalMultiplayer::InternalMultiplayerInterface);

using namespace Threading;
using namespace Interface;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerInterface::InternalMultiplayerInterface()
{
    __ConstructSingleton;
    this->mainThreadId = Thread::GetMyThreadId();
}

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerInterface::~InternalMultiplayerInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerInterface::Open()
{
    // create the message handler
    this->internalMultiplayerHandler = InternalMultiplayerHandler::Create();

    // setup a RunThroughHandlerThread object
    Ptr<RunThroughHandlerThread> handlerThread = RunThroughHandlerThread::Create();
    handlerThread->SetName("InternalMultiplayerInterface Thread");
    handlerThread->SetCoreId(System::Cpu::NetworkThreadCore);
    handlerThread->SetStackSize(65536);
    handlerThread->AttachHandler(this->internalMultiplayerHandler.cast<Handler>());
    this->SetHandlerThread(handlerThread.cast<HandlerThreadBase>());

    this->batchMessage = BatchMessage::Create();
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerInterface::Close()
{
    InterfaceBase::Close();
    this->batchMessage = 0;
    this->internalMultiplayerHandler = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called by the InternalMultiplayerHandler once per frame
    right after pending messages are processed and before starting
    frame rendering. This can be used to synchronize other threads
    with rendering, for instance to prevent the game thread from running
    ahead of the InternalMultiplayer thread and thus spamming its message queue.
*/
void
InternalMultiplayerInterface::SignalFrameEvent()
{
    this->frameEvent.Signal();
}

//------------------------------------------------------------------------------
/**
    This method can be called by any thread who wishes to synchronize itself
    with the InternalMultiplayer thread. The event will become signalled once per
    InternalMultiplayer frame after messages are processed and before rendering starts.
    The best place for a thread to call this method is right before InternalMultiplayer
    messages for the next frame are produced.
*/
void
InternalMultiplayerInterface::WaitForFrameEvent()
{
    this->frameEvent.Wait();
}

//------------------------------------------------------------------------------
/**
    Send a batched message. The message will not be sent to the render thread
    immediately, but instead will be collected in a batch message for later
    sending. Use this method to reduce communication overhead
    between the main and render thread by sending only one batch message
    instead of many messages. This method may only be called from the 
    main thread!
*/
void
InternalMultiplayerInterface::SendBatched(const Ptr<Message>& msg)
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif
    this->batchMessage->AddMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send off the batched messages to the network thread. This reduces 
    thread synchronization overhead dramatically if many messages must
    be sent per-frame. Only the main thread may call this method.
*/
void
InternalMultiplayerInterface::FlushBatchedMessages()
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif

    if (this->batchMessage->GetMessages().Size() > 0)
    {                                               
        // send current batch message
        this->Send(this->batchMessage.cast<Message>());

        // create a new, empty batch message
        this->batchMessage = BatchMessage::Create();   
    }
}

} // namespace InternalMultiplayer
