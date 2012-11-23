#pragma once
//------------------------------------------------------------------------------
/**
    @class FrameSync::FrameSyncSharedData
    
    A lock-less, double buffered data exchange object between 2 threads
    which are running in lockstep through the FrameSync system (e.g.
    the main thread and render thread). The owner thread is defined
    as the thread which owns the FrameSyncSharedData object (usually
    the main thread), and the client thread is usually the render thread.
    
    (C) 2010 Radon Labs GmbH
*/
#include "core/types.h"
#include "threading/threadid.h"
#include "framesync/framesynchandlerthread.h"
#include "threading/interlocked.h"
#include "threading/thread.h"

//------------------------------------------------------------------------------
namespace FrameSync
{
class FrameSyncSharedData : public Core::RefCounted
{
    __DeclareClass(FrameSyncSharedData);
public:
    /// constructor
    FrameSyncSharedData();
    /// destructor
    virtual ~FrameSyncSharedData();

    /// owner-side setup function
    template<typename T> void OwnerSetup(T* ignoreMe=0);
    /// owner-discard, must be called when owner is done with the object
    template<typename T> void OwnerDiscard(T* ignoreMe=0);
    /// access data as owner
    template<typename T> T& Owner();

    /// client-side setup function
    template<typename T> void ClientSetup(T* ignoreMe=0);
    /// client-discard, must be called when the client is done with the object
    template<typename T> void ClientDiscard(T* ignoreMe=0);
    /// access data as client
    template<typename T> T& Client();

private:
    /// create data objects
    template<typename T> void CreateDataObjects(T* ignoreMe=0);
    /// free data objects
    template<typename T> void DestroyDataObjects(T* ignoreMe=0);

    FrameSyncHandlerThread* frameSync;
    Threading::ThreadId ownerThreadId;
    Threading::ThreadId clientThreadId;
    int volatile useCount;      // 0, 1 or 2
    SizeT dataObjectSize;
    void* dataObjects[2];       // double buffered data object
};

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE> inline TYPE&
FrameSyncSharedData::Owner()
{
    n_assert(Threading::Thread::GetMyThreadId() == this->ownerThreadId);
    n_assert(sizeof(TYPE) <= size_t(this->dataObjectSize));
    IndexT evenIndex = this->frameSync->GetFrameCount() & 1;
    return *((TYPE*)this->dataObjects[evenIndex]);
}

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE> inline TYPE&
FrameSyncSharedData::Client()
{
    n_assert(Threading::Thread::GetMyThreadId() == this->clientThreadId);
    n_assert(sizeof(TYPE) <= size_t(this->dataObjectSize));
    IndexT oddIndex = 1 - (this->frameSync->GetFrameCount() & 1);
    return *((TYPE*)this->dataObjects[oddIndex]);
}

//------------------------------------------------------------------------------
/**
    Call this method from the owner thread before handing a smart ptr
    to the object to the client thread!
*/
template<typename TYPE> inline void 
FrameSyncSharedData::OwnerSetup(TYPE* ignoreMe)
{
    n_assert(Threading::InvalidThreadId == this->ownerThreadId);
    n_assert(Threading::InvalidThreadId == this->clientThreadId);
    
    this->ownerThreadId = Threading::Thread::GetMyThreadId();
    if (1 == Threading::Interlocked::Increment(this->useCount))
    {
        this->CreateDataObjects<TYPE>();
    }
    else
    {
        n_assert(sizeof(TYPE) == this->dataObjectSize);
    }
}

//------------------------------------------------------------------------------
/**
    Call this method on the client thread. It must be guaranteed that
    OwnerSetup() has already finished when this method is called! The
    provided template type must be identical between all method calls!
*/
template<typename TYPE> inline void
FrameSyncSharedData::ClientSetup(TYPE* ignoreMe)
{
    n_assert(Threading::InvalidThreadId == this->clientThreadId);

    this->clientThreadId = Threading::Thread::GetMyThreadId();
    n_assert(this->clientThreadId != this->ownerThreadId);

    if (1 == Threading::Interlocked::Increment(this->useCount))
    {
        this->CreateDataObjects<TYPE>();
    }
    else
    {
        n_assert(sizeof(TYPE) == this->dataObjectSize);
    }
}

//------------------------------------------------------------------------------
/**
    Call this method from the owner thread when the owner thread no longer 
    needs the object.
*/
template<typename TYPE> inline void
FrameSyncSharedData::OwnerDiscard(TYPE* ignoreMe)
{
    n_assert(Threading::Thread::GetMyThreadId() == this->ownerThreadId);
    this->ownerThreadId = Threading::InvalidThreadId;
    if (0 == Threading::Interlocked::Decrement(this->useCount))
    {
        this->DestroyDataObjects<TYPE>();
    }
}

//------------------------------------------------------------------------------
/**
    Call this method from the client thread when the client thread no longer
    needs the object.
*/
template<typename TYPE> inline void
FrameSyncSharedData::ClientDiscard(TYPE* ignoreMe)
{
    n_assert(Threading::Thread::GetMyThreadId() == this->clientThreadId);
    this->clientThreadId = Threading::InvalidThreadId;
    if (0 == Threading::Interlocked::Decrement(this->useCount))
    {
        this->DestroyDataObjects<TYPE>();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE> inline void
FrameSyncSharedData::CreateDataObjects(TYPE* ignoreMe)
{
    n_assert(0 == this->dataObjects[0]);
    n_assert(0 == this->dataObjects[1]);
    this->dataObjectSize = sizeof(TYPE);
    this->dataObjects[0] = n_new(TYPE);
    this->dataObjects[1] = n_new(TYPE);
}

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE> inline void
FrameSyncSharedData::DestroyDataObjects(TYPE* ignoreMe)
{
    n_assert(0 != this->dataObjects[0]);
    n_assert(0 != this->dataObjects[1]);
    n_assert(sizeof(TYPE) == this->dataObjectSize);
    n_delete(((TYPE*)this->dataObjects[0]));
    n_delete(((TYPE*)this->dataObjects[1]));
    this->dataObjects[0] = 0;
    this->dataObjects[1] = 0;
    this->dataObjectSize = 0;
}

} // namespace FrameSync
//------------------------------------------------------------------------------
