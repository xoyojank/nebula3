//------------------------------------------------------------------------------
//  memoryindexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memoryindexbufferloader.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryIndexBufferLoader, 'MIBL', Win360::D3D9MemoryIndexBufferLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryIndexBufferLoader, 'MIBL', Base::MemoryIndexBufferLoaderBase);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryIndexBufferLoader, 'MIBL', PS3::PS3MemoryIndexBufferLoader);
}
#else
#error "MemoryIndexBufferLoader class not implemented on this platform!"
#endif
