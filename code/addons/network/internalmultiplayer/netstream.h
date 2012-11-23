#pragma once
//------------------------------------------------------------------------------
/**
*/
#if __WIN32__ || __PS3__
#include "internalmultiplayer/raknet/raknetstream.h"
namespace InternalMultiplayer
{
    class NetStream : public RakNet::RakNetStream
    {
        __DeclareClass(NetStream);
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "internalmultiplayer/raknet/raknetstream.h"
namespace InternalMultiplayer
{
    class NetStream : public RakNet::RakNetStream
    {
        __DeclareClass(NetStream);
    };
}
#else
#include "internalmultiplayer/xbox360/xbox360netstream.h"
namespace InternalMultiplayer
{
    class NetStream : public Xbox360::Xbox360NetStream
    {
        __DeclareClass(NetStream);
    };
}
#endif
#else
n_error("NetStream not implemented for this platform!");
#endif