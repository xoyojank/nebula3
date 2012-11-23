#pragma once
//------------------------------------------------------------------------------
/**
    @class OSX::OSXThreadId
 
    A thread id uniquely identifies a thread within the process.
 
    (C) 2010 Radon Labs GmbH
*/
#include "core/types.h"

namespace Threading
{
typedef pthread_t ThreadId;
static const ThreadId InvalidThreadId = 0;
}
//------------------------------------------------------------------------------
