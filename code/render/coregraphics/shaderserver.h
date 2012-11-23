#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderServer
  
    The ShaderServer object loads the available shaders and can instantiate
    shaders for usage.
    
    (C) 2007 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9shaderserver.h"
namespace CoreGraphics
{
class ShaderServer : public Direct3D9::D3D9ShaderServer
{
    __DeclareClass(ShaderServer);
    __DeclareSingleton(ShaderServer);
public:
    /// constructor
    ShaderServer();
    /// destructor
    virtual ~ShaderServer();
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360shaderserver.h"
namespace CoreGraphics
{
class ShaderServer : public Xbox360::Xbox360ShaderServer
{
    __DeclareClass(ShaderServer);
    __DeclareSingleton(ShaderServer);
public:
    /// constructor
    ShaderServer();
    /// destructor
    virtual ~ShaderServer();
};
}
#elif __WII__
#include "coregraphics/wii/wiishaderserver.h"
namespace CoreGraphics
{
class ShaderServer : public Wii::WiiShaderServer
{
    __DeclareClass(ShaderServer);
    __DeclareSingleton(ShaderServer);
public:
    /// constructor
    ShaderServer();
    /// destructor
    virtual ~ShaderServer();
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3shaderserver.h"
namespace CoreGraphics
{
class ShaderServer : public PS3::PS3ShaderServer
{
    __DeclareClass(ShaderServer);
    __DeclareSingleton(ShaderServer);
public:
    /// constructor
    ShaderServer();
    /// destructor
    virtual ~ShaderServer();
};
}
#else
#error "ShaderServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
