//------------------------------------------------------------------------------
//  streammeshloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streammeshloader.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Win360::D3D9StreamMeshLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Wii::WiiStreamMeshLoader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Resources::StreamResourceLoader);
}
#else
#error "StreamMeshLoader class not implemented on this platform!"
#endif
