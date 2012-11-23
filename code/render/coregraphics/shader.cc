//------------------------------------------------------------------------------
//  shader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shader.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Shader, 'SHDR', Direct3D9::D3D9Shader);
}
#elif __XBOX360__
// FIXME!
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Shader, 'SHDR', Xbox360::Xbox360Shader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Shader, 'SHDR', Wii::WiiShader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Shader, 'SHDR', PS3::PS3Shader);
}
#else
#error "Shader class not implemented on this platform!"
#endif
