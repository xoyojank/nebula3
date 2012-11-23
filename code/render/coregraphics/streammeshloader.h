#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamMeshLoader
    
    Resource loader to setup a Mesh object from a stream.
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9streammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public Win360::D3D9StreamMeshLoader
{
    __DeclareClass(StreamMeshLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public Wii::WiiStreamMeshLoader
{
    __DeclareClass(StreamMeshLoader);
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3streammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public PS3::PS3StreamMeshLoader
{
    __DeclareClass(StreamMeshLoader);
};
}
#else
#error "StreamMeshLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

    