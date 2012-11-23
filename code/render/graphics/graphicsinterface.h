#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsInterface
  
    Implements the asynchronous interface to the InternalGraphics subsystem. Usually
    the application doesn't call methods on the GraphicsInterface
    directly, but instead uses one of the Graphics objects to
    communicate with the graphics thread.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "interface/interfacebase.h"
#include "core/singleton.h"
#include "messaging/batchmessage.h"
#include "graphics/graphicshandler.h"
#include "framesync/framesynchandlerthread.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GraphicsInterface : public Interface::InterfaceBase
{
    __DeclareClass(GraphicsInterface);
    __DeclareInterfaceSingleton(GraphicsInterface);
public:
    /// constructor
    GraphicsInterface();
    /// destructor
    virtual ~GraphicsInterface();

    /// open the interface object
    virtual void Open();
    /// close the interface object
    virtual void Close();
    
    /// send a batched messages (call FlushBatchedMessages() to send them to render thread)
    void SendBatched(const Ptr<Messaging::Message>& msg);
    /// flush batched messages, call before WaitForFrameEvent()
    void FlushBatchedMessages();

    /// wait for the graphics thread to process messages
    //void WaitForFrameEvent();
    /// wait until all pending resources are loaded
    void WaitForPendingResources();

    /// enter lock-step mode
    void EnterLockStepMode();
    /// leave lock-step mode
    void LeaveLockStepMode();
    /// call when game thread arrives at frame sync point
    void GameThreadWaitForFrameSync();

protected:
    friend class GraphicsHandler;

    /// signal the per-frame event, called by GraphicsHandler
    //void SignalFrameEvent();
    /// signal the resource loaded event, called by GraphicsHandler
    void SignalPendingResourceEvent();

    Threading::ThreadId mainThreadId;
    Threading::Event frameEvent;
    Ptr<FrameSync::FrameSyncHandlerThread> frameSyncHandlerThread;
    Ptr<GraphicsHandler> graphicsHandler;
    Ptr<Messaging::BatchMessage> batchMessage;
};

} // namespace Graphics
//------------------------------------------------------------------------------
