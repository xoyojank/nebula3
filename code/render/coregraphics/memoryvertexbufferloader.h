#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryVertexBufferLoader
    
    Initialize a vertex buffer object from vertex data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9memoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public Win360::D3D9MemoryVertexBufferLoader
{
    __DeclareClass(MemoryVertexBufferLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiimemoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public Wii::WiiMemoryVertexBufferLoader
{
    __DeclareClass(MemoryVertexBufferLoader);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3memoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public PS3::PS3MemoryVertexBufferLoader
{
    __DeclareClass(MemoryVertexBufferLoader);
};
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

