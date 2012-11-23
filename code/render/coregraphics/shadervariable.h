#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderVariable
    
    Provides direct access to a shader's global variable.
    The fastest way to change the value of a shader variable is to
    obtain a pointer to a shader variable once, and use it repeatedly
    to set new values.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shadervariable.h"
namespace CoreGraphics
{
class ShaderVariable : public Direct3D9::D3D9ShaderVariable
{
    __DeclareClass(ShaderVariable);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360shadervariable.h"
namespace CoreGraphics
{
class ShaderVariable : public Xbox360::Xbox360ShaderVariable
{
    __DeclareClass(ShaderVariable);
};
}
#elif __WII__
#include "coregraphics/wii/wiishadervariable.h"
namespace CoreGraphics
{
class ShaderVariable : public Wii::WiiShaderVariable
{
    __DeclareClass(ShaderVariable);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3shadervariable.h"
namespace CoreGraphics
{
class ShaderVariable : public PS3::PS3ShaderVariable
{
    __DeclareClass(ShaderVariable);
};
}
#else
#error "ShaderVariable class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

