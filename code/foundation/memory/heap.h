#pragma once
//------------------------------------------------------------------------------
/**
    @class Memory::Heap
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__XBOX360__ || __WIN32__)
#include "memory/win360/win360heap.h"
namespace Memory
{
typedef Win360::Win360Heap Heap;
}
#elif __WII__
#include "memory/wii/wiiheap.h"
namespace Memory
{
typedef Wii::WiiHeap Heap;
}
#elif __PS3__
#include "memory/ps3/ps3heap.h"
namespace Memory
{
typedef PS3::PS3Heap Heap;
}
#elif __OSX__
#include "memory/osx/osxheap.h"
namespace Memory
{
typedef OSX::OSXHeap Heap;
}
#else
#error "IMPLEMENT ME!"
#endif
