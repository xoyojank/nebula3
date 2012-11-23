//------------------------------------------------------------------------------
//  framesyncshareddata.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framesyncshareddata.h"

namespace FrameSync
{
__ImplementClass(FrameSyncSharedData, 'FSSD', Core::RefCounted);

using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
FrameSyncSharedData::FrameSyncSharedData() :
    ownerThreadId(Threading::InvalidThreadId),
    clientThreadId(Threading::InvalidThreadId),
    useCount(0),
    dataObjectSize(0)
{
    this->frameSync = FrameSyncHandlerThread::Instance();
    this->dataObjects[0] = 0;
    this->dataObjects[1] = 0;
}
    
//------------------------------------------------------------------------------
/**
*/
FrameSyncSharedData::~FrameSyncSharedData()
{
    n_assert(0 == this->useCount);
    n_assert(0 == this->dataObjects[0]);
    n_assert(0 == this->dataObjects[1]);
}

} // namespace FrameSync
