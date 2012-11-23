#pragma once
//------------------------------------------------------------------------------
/**
@class InternalMultiplayer::NetworkPageHandler

(C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "internalmultiplayer/debug/raknet/raknetnetworkpagehandler.h"
namespace Debug
{
    class NetworkPageHandler : public RakNet::RakNetNetworkPageHandler
    {
        __DeclareClass(NetworkPageHandler);
    public:
        /// constructor
        NetworkPageHandler();
        /// destructor
        virtual ~NetworkPageHandler();
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "internalmultiplayer/debug/raknet/raknetnetworkpagehandler.h"
namespace Debug
{
    class NetworkPageHandler : public RakNet::RakNetInternalMultiplayerServer
    {
        __DeclareClass(NetworkPageHandler);
    public:
        /// constructor
        NetworkPageHandler();
        /// destructor
        virtual ~NetworkPageHandler();
    };
}
#else
#include "internalmultiplayer/debug/xbox360/xbox360networkpagehandler.h"
namespace Debug
{
    class NetworkPageHandler : public Xbox360::Xbox360NetworkPageHandler
    {
        __DeclareClass(NetworkPageHandler);
    public:
        /// constructor
        NetworkPageHandler();
        /// destructor
        virtual ~NetworkPageHandler();
    };
}
#endif
#else
#error "NetworkPageHandler class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
