#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalMultiplayerServer
      
    (C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "internalmultiplayer/raknet/raknetinternalmultiplayerserver.h"
namespace InternalMultiplayer
{
    class InternalMultiplayerServer : public RakNet::RakNetInternalMultiplayerServer
    {
        __DeclareClass(InternalMultiplayerServer);
        __DeclareSingleton(InternalMultiplayerServer);
    public:
        /// constructor
        InternalMultiplayerServer();
        /// destructor
        virtual ~InternalMultiplayerServer();
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "internalmultiplayer/raknet/raknetinternalmultiplayerserver.h"
namespace InternalMultiplayer
{
    class InternalMultiplayerServer : public RakNet::RakNetInternalMultiplayerServer
    {
        __DeclareClass(InternalMultiplayerServer);
        __DeclareSingleton(InternalMultiplayerServer);
    public:
        /// constructor
        InternalMultiplayerServer();
        /// destructor
        virtual ~InternalMultiplayerServer();
    };
}
#else
#include "internalmultiplayer/xbox360/xbox360internalmultiplayerserver.h"
namespace InternalMultiplayer
{
    class InternalMultiplayerServer : public Xbox360::Xbox360InternalMultiplayerServer
    {
        __DeclareClass(InternalMultiplayerServer);
        __DeclareSingleton(InternalMultiplayerServer);
    public:
        /// constructor
        InternalMultiplayerServer();
        /// destructor
        virtual ~InternalMultiplayerServer();
    };
}
#endif
#else
#error "InternalMultiplayerServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
