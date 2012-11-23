//------------------------------------------------------------------------------
//  vertexlayoutserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayoutserver.h"
#if (__WIN32__ || __XBOX360__ || __PS3__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexLayoutServer, 'VLSV', Base::VertexLayoutServerBase);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::VertexLayoutServer, 'VLSV', Wii::WiiVertexLayoutServer);
}
#else
#error "VertexLayoutServer class not implemented on this platform!"
#endif

