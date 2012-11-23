//------------------------------------------------------------------------------
//  indexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/indexbuffer.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', Win360::D3D9IndexBuffer);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', Wii::WiiIndexBuffer);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', PS3::PS3IndexBuffer);
}
#else
#error "IndexBuffer class not implemented on this platform!"
#endif

