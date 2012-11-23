//------------------------------------------------------------------------------
//  shadervariation.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariation.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariation, 'SHVR', Direct3D9::D3D9ShaderVariation);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariation, 'SHVR', Xbox360::Xbox360ShaderVariation);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariation, 'SHVR', Wii::WiiShaderVariation);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderVariation, 'SHVR', PS3::PS3ShaderVariation);
}
#else
#error "ShaderVariation class not implemented on this platform!"
#endif
