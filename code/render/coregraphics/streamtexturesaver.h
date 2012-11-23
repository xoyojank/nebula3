#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamTextureSaver
    
    Allows to save texture data in a standard file format into a stream.    
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9streamtexturesaver.h"
namespace CoreGraphics
{
class StreamTextureSaver : public Win360::D3D9StreamTextureSaver
{
    __DeclareClass(StreamTextureSaver);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreamtexturesaver.h"
namespace CoreGraphics
{
class StreamTextureSaver : public Wii::WiiStreamTextureSaver
{
    __DeclareClass(StreamTextureSaver);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3streamtexturesaver.h"
namespace CoreGraphics
{
class StreamTextureSaver : public PS3::PS3StreamTextureSaver
{
    __DeclareClass(StreamTextureSaver);
};
}
#else
#error "StreamTextureSaver class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

    