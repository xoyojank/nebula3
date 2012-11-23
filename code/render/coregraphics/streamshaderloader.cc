//------------------------------------------------------------------------------
//  streamshaderloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamshaderloader.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamShaderLoader, 'SSDL', Direct3D9::D3D9StreamShaderLoader);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamShaderLoader, 'SSDL', Xbox360::Xbox360StreamShaderLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamShaderLoader, 'SSDL', Wii::WiiStreamShaderLoader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamShaderLoader, 'SSDL', PS3::PS3StreamShaderLoader);
}
#else
#error "StreamShaderLoader class not implemented on this platform!"
#endif
