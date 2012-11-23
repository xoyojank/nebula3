#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderInstance
    
    A shader instance object is created from a shader and contains a local copy
    of the original shader state which can be modified through ShaderVariable
    objects. Shader instance objects are created directly through the 
    shader server.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shaderinstance.h"
namespace CoreGraphics
{
class ShaderInstance : public Direct3D9::D3D9ShaderInstance
{
    __DeclareClass(ShaderInstance);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360shaderinstance.h"
namespace CoreGraphics
{
class ShaderInstance : public Xbox360::Xbox360ShaderInstance
{
    __DeclareClass(ShaderInstance);
};
}
#elif __WII__
#include "coregraphics/wii/wiishaderinstance.h"
namespace CoreGraphics
{
class ShaderInstance : public Wii::WiiShaderInstance
{
    __DeclareClass(ShaderInstance);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3shaderinstance.h"
namespace CoreGraphics
{
class ShaderInstance : public PS3::PS3ShaderInstance
{
    __DeclareClass(ShaderInstance);
};
}
#else
#error "ShaderInstance class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

