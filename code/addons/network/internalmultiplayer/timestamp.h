#pragma once
//------------------------------------------------------------------------------
/**
@class InternalMultiplayer::TimeStamp

    A TimeStamp will be same on each system.
    Therefore its platform depended implementation must synchronize its time value.

(C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknettimestamp.h"
namespace InternalMultiplayer
{
    typedef RakNet::RakNetTimeStamp TimeStamp;
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "InternalMultiplayer/raknet/raknettimestamp.h"
namespace InternalMultiplayer
{
    typedef RakNet::RakNetTimeStamp TimeStamp;
}
#else
#include "internalmultiplayer/xbox360/xbox360timestamp.h"
namespace InternalMultiplayer
{
    typedef Xbox360::Xbox360TimeStamp TimeStamp;
}
#endif
#else
#error "TimeStamp class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
