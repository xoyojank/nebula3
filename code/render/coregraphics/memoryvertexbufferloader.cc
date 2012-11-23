//------------------------------------------------------------------------------
//  memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memoryvertexbufferloader.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Win360::D3D9MemoryVertexBufferLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Wii::WiiMemoryVertexBufferLoader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', PS3::PS3MemoryVertexBufferLoader);
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
