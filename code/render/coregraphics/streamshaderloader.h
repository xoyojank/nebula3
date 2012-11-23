#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamShaderLoader
    
    Resource loader to setup a Shader object from a stream.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Direct3D9::D3D9StreamShaderLoader
{
    __DeclareClass(StreamShaderLoader);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Xbox360::Xbox360StreamShaderLoader
{
    __DeclareClass(StreamShaderLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Wii::WiiStreamShaderLoader
{
    __DeclareClass(StreamShaderLoader);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public PS3::PS3StreamShaderLoader
{
    __DeclareClass(StreamShaderLoader);
};
}
#else
#error "StreamShaderLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

    