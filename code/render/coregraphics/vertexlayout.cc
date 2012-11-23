//------------------------------------------------------------------------------
//  vertexlayout.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayout.h"
#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Win360::D3D9VertexLayout);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Wii::WiiVertexLayout);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Base::VertexLayoutBase);
}
#else
#error "VertexLayout class not implemented on this platform!"
#endif

