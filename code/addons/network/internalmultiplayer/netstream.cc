//------------------------------------------------------------------------------
//  netstream.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/netstream.h"

namespace InternalMultiplayer
{
#if __WIN32__ || __PS3__
__ImplementClass(InternalMultiplayer::NetStream, 'NEST', RakNet::RakNetStream);
#elif __XBOX360__
__ImplementClass(InternalMultiplayer::NetStream, 'NEST', Base::NetStreamBase);
#else
#error "NetStream class not implemented on this platform!"
#endif
}