//------------------------------------------------------------------------------
//  InternalPlayer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/internalplayer.h"

namespace InternalMultiplayer
{
#if __WIN32__ || __PS3__
__ImplementClass(InternalMultiplayer::InternalPlayer, 'INPL', RakNet::RakNetInternalPlayer);
#elif __XBOX360__
#if XBOX360_USE_RAKNET
__ImplementClass(InternalMultiplayer::InternalPlayer, 'INPL', RakNet::RakNetInternalPlayer);
#else
__ImplementClass(InternalMultiplayer::InternalPlayer, 'INPL', Xbox360::Xbox360InternalPlayer);
#endif
#else
#error "InternalPlayer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
InternalPlayer::InternalPlayer()
{
}

//------------------------------------------------------------------------------
/**
*/
InternalPlayer::~InternalPlayer()
{
}

} // namespace InternalMultiplayer
