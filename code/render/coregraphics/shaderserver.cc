//------------------------------------------------------------------------------
//  shaderserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderserver.h"

namespace CoreGraphics
{
#if __WIN32__
__ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Direct3D9::D3D9ShaderServer);
__ImplementSingleton(CoreGraphics::ShaderServer);
#elif __XBOX360__
// FIXME!
__ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Xbox360::Xbox360ShaderServer);
__ImplementSingleton(CoreGraphics::ShaderServer);
#elif __WII__
__ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Wii::WiiShaderServer);
__ImplementSingleton(CoreGraphics::ShaderServer);
#elif __PS3__
__ImplementClass(CoreGraphics::ShaderServer, 'SHSV', PS3::PS3ShaderServer);
__ImplementSingleton(CoreGraphics::ShaderServer);
#else
#error "ShaderServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
ShaderServer::ShaderServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShaderServer::~ShaderServer()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
