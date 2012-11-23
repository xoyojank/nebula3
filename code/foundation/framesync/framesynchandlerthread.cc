//------------------------------------------------------------------------------
//  framesynchandlerthread.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framesync/framesynchandlerthread.h"
#include "framesync/framesynctimer.h"
#include "threading/interlocked.h"

namespace FrameSync
{
__ImplementClass(FrameSync::FrameSyncHandlerThread, 'FSHT', Messaging::HandlerThreadBase);
__ImplementInterfaceSingleton(FrameSync::FrameSyncHandlerThread);

using namespace Util;
using namespace Threading;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
FrameSyncHandlerThread::FrameSyncHandlerThread() :
    lockStepMode(0),
    frameCount(0),
    curTime(0.0),
    fixedFrameTime(0.0),
    fixedFrameTimeMode(0),
    realTime(0.0)
{
    __ConstructInterfaceSingleton;
    this->threadBarrier.Setup(NumSyncThreads);
    this->msgQueue.SetSignalOnEnqueueEnabled(false);
    this->masterTimer.Start();
}

//------------------------------------------------------------------------------
/**
*/
FrameSyncHandlerThread::~FrameSyncHandlerThread()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
    Enter lock-step mode, this method must only be called from the
    slave thread.
*/
void
FrameSyncHandlerThread::EnterLockStepMode()
{
    if (0 == this->lockStepMode)
    {
        Interlocked::Exchange(&this->lockStepMode, 1);
        this->ArriveAtSyncPoint(false);
    }
}

//------------------------------------------------------------------------------
/**
    Leave lock-step mode, this method must only be called from the
    slave thread.
*/
void
FrameSyncHandlerThread::LeaveLockStepMode()
{
    if (1 == this->lockStepMode)
    {
        this->ArriveAtSyncPoint(false);
        Interlocked::Exchange(&this->lockStepMode, 0);
    }
}

//------------------------------------------------------------------------------
/**
    This is the central sync point. Every thread will call this method 
    at the start of a new frame to synchronize with the other lock-step
    frames. Once all threads have arrived, the master time will be advanced
    and the double-buffered message queues will be flipped.
*/
void
FrameSyncHandlerThread::ArriveAtSyncPoint(bool masterThread)
{
    if (0 != this->lockStepMode)
    {
        // lock-step mode enabled
        if (this->threadBarrier.Arrive())
        {
            // all thread have arrived, perform sync actions and signal other
            // threads to continue
            this->frameCritSect.Enter();
            this->SyncUpdateTime();
            this->SyncFlipMessageQueue();
            this->frameCritSect.Leave();
            this->threadBarrier.SignalContinue();
        }
        else
        {
            // other threads still need to finish their frame, wait until they arrive
            this->threadBarrier.Wait();
        }
    }
    else
    {
        // lock-step mode disabled, the master thread is allowed to run,
        // while client threads wait for the master thread's sync point
        if (masterThread)
        {
            this->frameCritSect.Enter();
            this->SyncUpdateTime();
            this->SyncFlipMessageQueue();
            this->frameSyncEvent.Signal();
            this->frameCritSect.Leave();
        }
        else
        {            
            this->frameSyncEvent.Wait();
        }
    }

    // update the thread-local FrameSyncTimer object (if exists)
    this->SyncUpdateThreadLocalFrameSyncTimer();
}

//------------------------------------------------------------------------------
/**
    Start the master timer. This method may be called from any of the
    sync-threads, thus we need to protect with a critical section.
    DO NOT CALL FREQUENTLY!
*/
void
FrameSyncHandlerThread::StartMasterTime()
{
    this->frameCritSect.Enter();
    this->masterTimer.Start();
    this->frameCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    Stop the master timer, may be called from any of the sync-threads.
    DO NOT CALL FREQUENTLY!
*/
void
FrameSyncHandlerThread::StopMasterTime()
{
    this->frameCritSect.Enter();
    this->masterTimer.Stop();
    this->frameCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    Reset the master timer, may be called from any of the sync-threads.
    DO NOT CALL FREQUENTLY!
*/
void
FrameSyncHandlerThread::ResetMasterTime()
{
    this->frameCritSect.Enter();
    this->masterTimer.Reset();
    this->frameCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
    Test if the master time is currently running. May be called from any
    of the sync-threads.
    DO NOT CALL FREQUENTLY!
*/
bool
FrameSyncHandlerThread::IsMasterTimeRunning() const
{
    this->frameCritSect.Enter();
    bool isRunning = this->masterTimer.Running();
    this->frameCritSect.Leave();
    return isRunning;
}

//------------------------------------------------------------------------------
/**
    Safely get the current frame time. This is only used for first-time 
    init of FrameSyncTimer objects
    DO NOT CALL FREQUENTLY!
*/
Timing::Time
FrameSyncHandlerThread::SafeGetMasterTime() const
{
    this->frameCritSect.Enter();
    Timing::Time t = this->curTime;
    this->frameCritSect.Leave();
    return t;
}

//------------------------------------------------------------------------------
/**
    Update the master time. Since this method is called during the 
    sync point, we don't need thread-synchronization.
*/
void
FrameSyncHandlerThread::SyncUpdateTime()
{
    Timing::Time timeDelta(0);
    Timing::Time masterTime = this->masterTimer.GetTime();
    if (1 == this->fixedFrameTimeMode)
    {
        // in fixed frame time mode increase current time
        // by a fixed value
        timeDelta = this->fixedFrameTime;
    }
    else
    {
        // in normal mode increase current time by the time passed
        // since the last SyncUpdateTime call
        timeDelta = masterTime - this->realTime;
    }
    this->curTime += timeDelta;
    this->realTime = masterTime;
    Interlocked::Increment(this->frameCount);
}

//------------------------------------------------------------------------------
/**
    Checks if the current thread has a FrameSyncTimer singleton, and if 
    yes, update its time. Call this method from the frame-sync-point.
*/
void
FrameSyncHandlerThread::SyncUpdateThreadLocalFrameSyncTimer()
{
    if (FrameSyncTimer::HasInstance())
    {
        FrameSyncTimer::Instance()->Update(this->curTime);
    }
}

//------------------------------------------------------------------------------
/**
    Flip the message queues, this is called from the sync point.
*/
void
FrameSyncHandlerThread::SyncFlipMessageQueue()
{
    this->msgQueue.DequeueAll(this->msgArray);
}

//------------------------------------------------------------------------------
/**
    Add a message to the producer queue, since we're using double buffering
    so we don't need thread-synchronization.
*/
void
FrameSyncHandlerThread::AddMessage(const Ptr<Message>& msg)
{
    this->msgQueue.Enqueue(msg);
}

//------------------------------------------------------------------------------
/**
    Cancel a pending message from the producer queue. If the message is
    already in the consumer queue, then it's too late to cancel the
    message!
*/
void
FrameSyncHandlerThread::CancelMessage(const Ptr<Message>& msg)
{
    this->msgQueue.EraseMatchingElements(msg);
}

//------------------------------------------------------------------------------
/**
    Wait for a message to be handled.
*/
void
FrameSyncHandlerThread::WaitForMessage(const Ptr<Message>& msg)
{
    n_assert(!this->lockStepMode);
    while (!msg->Handled())
    {
        this->ArriveAtSyncPoint(false);
    }
}

//------------------------------------------------------------------------------
/**
    The central message processing loop.
*/
void
FrameSyncHandlerThread::DoWork()
{
    this->ThreadOpenHandlers();
    do 
    {
        bool msgHandled = false;

        // update state of deferred messages
        msgHandled = this->ThreadUpdateDeferredMessages();

        // arrive at sync point as master thread
        this->ArriveAtSyncPoint(true);

        // process messages
        msgHandled |= this->ThreadHandleMessages(this->msgArray);
        this->msgArray.Clear();

        // signal if at least one message has been handled
        if (msgHandled)
        {
            this->ThreadSignalMessageHandled();
        }

        // do per-frame update on attached handlers
        this->ThreadUpdateHandlers();
		Thread::YieldThread();    
    }
    while (!this->ThreadStopRequested());

    // cleanup and exit thread
    this->ThreadDiscardDeferredMessages();
    this->ThreadCloseHandlers();
}

//------------------------------------------------------------------------------
/**
    Start the fixed frame time mode. In this mode the current time is
    increased by a fixed value every frame.
*/
void
FrameSyncHandlerThread::StartFixedFrameTime(Timing::Time frameTime)
{
    if (0 == this->fixedFrameTimeMode)
    {
        Interlocked::Exchange(&this->fixedFrameTimeMode, 1);
        this->frameCritSect.Enter();
        this->fixedFrameTime = frameTime;
        this->frameCritSect.Leave();
    }
}

//------------------------------------------------------------------------------
/**
	Stop the fixed frame time mode.
*/
void
FrameSyncHandlerThread::StopFixedFrameTime()
{
    if (1 == this->fixedFrameTimeMode)
    {
        Interlocked::Exchange(&this->fixedFrameTimeMode, 0);
    }
}

} // namespace FrameSync