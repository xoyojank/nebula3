#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::UniquePlayerId

    A UniquePlayerId identifies each player over the whole network.

    (C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknetuniqueplayerid.h"
namespace InternalMultiplayer
{
    typedef RakNet::RakNetUniquePlayerId UniquePlayerId;
}
#elif __XBOX360__
    #if XBOX360_USE_RAKNET
    #include "InternalMultiplayer/raknet/raknetuniqueplayerid.h"
    namespace InternalMultiplayer
    {
        typedef RakNet::RakNetUniquePlayerId UniquePlayerId;
    }
    #else
    #include "InternalMultiplayer/xbox360/xbox360uniqueplayerid.h"    
    namespace InternalMultiplayer
    {
        typedef Xbox360::Xbox360UniquePlayerId UniquePlayerId;
    }
    #endif
#else
#error "UniquePlayerId class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
