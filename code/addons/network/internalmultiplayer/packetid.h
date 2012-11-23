#pragma once
//------------------------------------------------------------------------------
/**
*/
#if __WIN32__ || __PS3__
#include "internalmultiplayer/raknet/raknetpacketid.h"
namespace InternalMultiplayer
{
    typedef RakNet::RakNetPackedId PacketId;
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "internalmultiplayer/raknet/raknetpacketid.h"
namespace InternalMultiplayer
{
    typedef RakNet::RakNetPackedId PacketId;
}
#else
#include "internalmultiplayer/xbox360/xbox360packetid.h"
namespace InternalMultiplayer
{
    typedef Xbox360::Xbox360PackedId PacketId;
}
#endif
#else
n_error("PacketId not implemented for this platform!");
#endif