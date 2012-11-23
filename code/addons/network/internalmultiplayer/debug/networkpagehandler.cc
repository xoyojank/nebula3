//------------------------------------------------------------------------------
//  NetworkPageHandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/debug/networkpagehandler.h"

namespace Debug
{
#if __WIN32__ || __PS3__
    __ImplementClass(Debug::NetworkPageHandler, 'DNPH', RakNet::RakNetNetworkPageHandler);    
#elif __XBOX360__
#if XBOX360_USE_RAKNET
    __ImplementClass(Debug::NetworkPageHandler, 'DNPH', RakNet::RakNetNetworkPageHandler);        
#else
    __ImplementClass(Debug::NetworkPageHandler, 'DNPH', Xbox360::Xbox360NetworkPageHandler);    
#endif
#else
#error "NetworkPageHandler class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
NetworkPageHandler::NetworkPageHandler()
{
}

//------------------------------------------------------------------------------
/**
*/
NetworkPageHandler::~NetworkPageHandler()
{
}

} // namespace Debug
