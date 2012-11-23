#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::PacketPriority

(C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknetpacketpriority.h"
namespace InternalMultiplayer
{
    class PacketPriority : public RakNet::RakNetPacketPriority
    {
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "InternalMultiplayer/raknet/raknetpacketpriority.h"
namespace InternalMultiplayer
{
    class PacketPriority : public RakNet::RakNetPacketPriority
    {
    };
}
#else
#include "InternalMultiplayer/xbox360/xbox360packetpriority.h"
namespace InternalMultiplayer
{
    class PacketPriority : public Xbox360::Xbox360PacketPriority
    {
    };
}
#endif
#else
#error "PacketPriority class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
