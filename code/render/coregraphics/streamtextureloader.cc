//------------------------------------------------------------------------------
//  streamtextureloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamtextureloader.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Direct3D9::D3D9StreamTextureLoader);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Xbox360::Xbox360StreamTextureLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Wii::WiiStreamTextureLoader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', PS3::PS3StreamTextureLoader);
}
#else
#error "StreamTextureLoader class not implemented on this platform!"
#endif
