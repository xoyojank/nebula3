#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryIndexBufferLoader
    
    Initialize an index buffer object from index data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9memoryindexbufferloader.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public Win360::D3D9MemoryIndexBufferLoader
{
    __DeclareClass(MemoryIndexBufferLoader);
};
}
#elif __WII__
// empty shell on Wii
#include "coregraphics/base/memoryindexbufferloaderbase.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public Base::MemoryIndexBufferLoaderBase
{
    __DeclareClass(MemoryIndexBufferLoader);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3memoryindexbufferloader.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public PS3::PS3MemoryIndexBufferLoader
{
    __DeclareClass(MemoryIndexBufferLoader);
};
}
#else
#error "MemoryIndexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

