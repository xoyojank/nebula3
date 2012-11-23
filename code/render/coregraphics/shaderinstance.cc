//------------------------------------------------------------------------------
//  shaderinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderinstance.h"

#if __WIN32__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderInstance, 'SINS', Direct3D9::D3D9ShaderInstance);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderInstance, 'SINS', Xbox360::Xbox360ShaderInstance);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderInstance, 'SINS', Wii::WiiShaderInstance);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::ShaderInstance, 'SINS', PS3::PS3ShaderInstance);
}
#else
#error "ShaderInstance class not implemented on this platform!"
#endif
