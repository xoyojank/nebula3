#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::Shader
  
    A shader object manages the entire render state required to render
    a mesh.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shader.h"
namespace CoreGraphics
{
class Shader : public Direct3D9::D3D9Shader
{
    __DeclareClass(Shader);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360shader.h"
namespace CoreGraphics
{
class Shader : public Xbox360::Xbox360Shader
{
    __DeclareClass(Shader);
};
}
#elif __WII__
#include "coregraphics/wii/wiishader.h"
namespace CoreGraphics
{
class Shader : public Wii::WiiShader
{
    __DeclareClass(Shader);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3shader.h"
namespace CoreGraphics
{
class Shader : public PS3::PS3Shader
{
    __DeclareClass(Shader);
};
}
#else
#error "Shader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

