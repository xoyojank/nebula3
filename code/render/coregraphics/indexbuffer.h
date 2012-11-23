#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::IndexBuffer
  
    A resource which holds an array of indices into an array of vertices.  
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9indexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public Win360::D3D9IndexBuffer
{
    __DeclareClass(IndexBuffer);
};
}
#elif __WII__
#include "coregraphics/wii/wiiindexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public Wii::WiiIndexBuffer
{
    __DeclareClass(IndexBuffer);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3indexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public PS3::PS3IndexBuffer
{
    __DeclareClass(IndexBuffer);
};
}
#else
#error "IndexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

