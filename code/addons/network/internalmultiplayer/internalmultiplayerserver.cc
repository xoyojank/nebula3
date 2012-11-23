//------------------------------------------------------------------------------
//  internalmultiplayerserver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/internalmultiplayerserver.h"

namespace InternalMultiplayer
{
#if __WIN32__ || __PS3__
    __ImplementClass(InternalMultiplayer::InternalMultiplayerServer, 'IMSV', RakNet::RakNetInternalMultiplayerServer);
    __ImplementSingleton(InternalMultiplayer::InternalMultiplayerServer);
#elif __XBOX360__
#if XBOX360_USE_RAKNET
    __ImplementClass(InternalMultiplayer::InternalMultiplayerServer, 'IMSV', RakNet::RakNetInternalMultiplayerServer);    
    __ImplementSingleton(InternalMultiplayer::InternalMultiplayerServer);
#else
    __ImplementClass(InternalMultiplayer::InternalMultiplayerServer, 'IMSV', Xbox360::Xbox360InternalMultiplayerServer);
    __ImplementSingleton(InternalMultiplayer::InternalMultiplayerServer);
#endif
#else
#error "InternalMultiplayerServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerServer::InternalMultiplayerServer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerServer::~InternalMultiplayerServer()
{
    __DestructSingleton;
}

} // namespace InternalMultiplayer
