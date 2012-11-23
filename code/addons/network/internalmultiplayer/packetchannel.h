#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::PacketChannel

    ChannelIds for different channels where packets can be send thru.

(C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknetpacketchannel.h"
namespace InternalMultiplayer
{
    class PacketChannel : public RakNet::RakNetPacketChannel
    {
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "InternalMultiplayer/raknet/raknetpacketchannel.h"
namespace InternalMultiplayer
{
    class PacketChannel : public RakNet::RakNetPacketChannel
    {
    };
}
#else
#include "InternalMultiplayer/xbox360/xbox360packetChannel.h"
namespace InternalMultiplayer
{
    class PacketChannel : public Xbox360::Xbox360PacketChannel
    {
    };
}
#endif
#else
#error "PacketChannel class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
