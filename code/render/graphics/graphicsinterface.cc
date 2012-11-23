//------------------------------------------------------------------------------
//  graphicsinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicshandler.h"
#include "debugrender/debuggraphicshandler.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsInterface, 'GRIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Graphics::GraphicsInterface);

using namespace Threading;
using namespace Interface;
using namespace Messaging;
using namespace FrameSync;

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::GraphicsInterface()
{
    __ConstructSingleton;
    this->mainThreadId = Thread::GetMyThreadId();
}

//------------------------------------------------------------------------------
/**
*/
GraphicsInterface::~GraphicsInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Open()
{
    // setup the message handler
    this->graphicsHandler = GraphicsHandler::Create();

    // setup a FrameSyncHandlerThread object
    this->frameSyncHandlerThread = FrameSyncHandlerThread::Create();
    this->frameSyncHandlerThread->SetName("GraphicsInterface Thread");
    this->frameSyncHandlerThread->SetCoreId(System::Cpu::RenderThreadCore);
    this->frameSyncHandlerThread->SetStackSize(128 * 1024);
    this->frameSyncHandlerThread->AttachHandler(this->graphicsHandler.cast<Handler>());
    this->frameSyncHandlerThread->AttachHandler(Debug::DebugGraphicsHandler::Create());
    this->SetHandlerThread(this->frameSyncHandlerThread.cast<HandlerThreadBase>());

    this->batchMessage = BatchMessage::Create();
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::Close()
{
    InterfaceBase::Close();
    this->batchMessage = 0;
    this->graphicsHandler = 0;
    this->frameSyncHandlerThread = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::EnterLockStepMode()
{
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    this->frameSyncHandlerThread->EnterLockStepMode();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::LeaveLockStepMode()
{
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    this->frameSyncHandlerThread->LeaveLockStepMode();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsInterface::GameThreadWaitForFrameSync()
{
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    this->frameSyncHandlerThread->ArriveAtSyncPoint(false);
}

//------------------------------------------------------------------------------
/**
    This method will wait until the graphics handler signals that all pending
    resources have been loaded.
*/
void
GraphicsInterface::WaitForPendingResources()
{
    this->graphicsHandler->WaitForPendingResources();
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
GraphicsInterface::SendBatched(const Ptr<Message>& msg)
{
    #if NEBULA3_DEBUG
    n_assert(Thread::GetMyThreadId() == this->mainThreadId);
    #endif
    this->batchMessage->AddMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send off the batched messages to the render thread. This reduces 
    thread synchronization overhead dramatically if many messages must
    be sent per-frame. Only the main thread may call this method.
*/
void
GraphicsInterface::FlushBatchedMessages()
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

} // namespace Graphics
