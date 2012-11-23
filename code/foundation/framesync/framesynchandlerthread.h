#pragma once
//------------------------------------------------------------------------------
/**
    @class FrameSync::FrameSyncHandlerThread
    
    A special handler thread class for synchronizing the render thread and
    the game thread (and possibly other threads which need to run in
    lock-step with the render thread).
    
    The FrameSyncHandlerThread object implements a clearly defined sync point
    which separates the previous frame from the next frame for all 
    participating threads:
    
    - all threads arrive at the frame sync point and wait for the arrival
      of the other threads
    - once all threads have arrived:
        - advance the master time
        - flip message queues
        - start all threads
        
    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#include "messaging/handlerthreadbase.h"
#include "timing/timer.h"
#include "threading/threadbarrier.h"
#include "threading/safequeue.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
namespace FrameSync
{
class FrameSyncHandlerThread : public Messaging::HandlerThreadBase
{
    __DeclareClass(FrameSyncHandlerThread);
    __DeclareInterfaceSingleton(FrameSyncHandlerThread);
public:
    static const SizeT NumSyncThreads = 2;  // render thread and main thread

    /// constructor
    FrameSyncHandlerThread();
    /// destructor
    virtual ~FrameSyncHandlerThread();

    /// enter lock-step mode
    void EnterLockStepMode();
    /// leave lock-step mode
    void LeaveLockStepMode();
    /// return true if currently in lock-step mode
    bool LockStepModeActive() const;
    /// get the current frame count (also available on FrameSyncTimer)
    IndexT GetFrameCount() const;
    /// start fixed frame time mode
    void StartFixedFrameTime(Timing::Time frameTime);
    /// stop fixed frame time mode
    void StopFixedFrameTime();
    /// check whether the fixed frame time mode is currently active
    bool IsFixedFrameTime();

    /// notify arrival at sync point
    void ArriveAtSyncPoint(bool masterThread);

    /// add a message to be handled
    virtual void AddMessage(const Ptr<Messaging::Message>& msg);
    /// cancel a pending message
    virtual void CancelMessage(const Ptr<Messaging::Message>& msg);
    /// wait for message to be handled
    virtual void WaitForMessage(const Ptr<Messaging::Message>& msg);

    /// this method runs in the thread context
    virtual void DoWork();

private:
    friend class FrameSyncTimer;

    /// start the master time (SLOW!)
    void StartMasterTime();
    /// stop the master time (SLOW!)
    void StopMasterTime();
    /// reset the master time (SLOW!)
    void ResetMasterTime();
    /// return true if time is running (SLOW!)
    bool IsMasterTimeRunning() const;
    /// safely get the current frame time (SLOW!)
    Timing::Time SafeGetMasterTime() const;

    /// update the time (called during sync point)
    void SyncUpdateTime();
    /// update the thread-local FrameSyncTimer singleton
    void SyncUpdateThreadLocalFrameSyncTimer();
    /// flip message queues
    void SyncFlipMessageQueue();

    volatile int lockStepMode;
    volatile int frameCount;
    volatile int fixedFrameTimeMode;
    Threading::ThreadBarrier threadBarrier;
    Threading::Event frameSyncEvent;
    Threading::CriticalSection frameCritSect;
    
    Timing::Timer masterTimer;
    Timing::Time curTime;
    Timing::Time fixedFrameTime;
    Timing::Time realTime;

    Threading::SafeQueue<Ptr<Messaging::Message> > msgQueue;   // incoming messages
    Util::Array<Ptr<Messaging::Message> > msgArray;            // processed messages
};    

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameSyncHandlerThread::LockStepModeActive() const
{
    return (0 != this->lockStepMode);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameSyncHandlerThread::GetFrameCount() const
{
    // incremented through interlocked function, no need for crit sect
    return this->frameCount;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameSyncHandlerThread::IsFixedFrameTime()
{
    return (0 != this->fixedFrameTimeMode);
}
} // namespace FrameSync
//------------------------------------------------------------------------------
    