#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::SessionInfo

(C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknetsessioninfo.h"
namespace InternalMultiplayer
{
class SessionInfo : public RakNet::RakNetSessionInfo
{
public:
    /// constructor
    SessionInfo();
};
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "InternalMultiplayer/raknet/raknetsessioninfo.h"
namespace InternalMultiplayer
{
class SessionInfo : public RakNet::RakNetSessionInfo
{
public:
    /// constructor
    SessionInfo();
};
}
#else
#include "InternalMultiplayer/xbox360/xbox360sessioninfo.h"
namespace InternalMultiplayer
{
    class SessionInfo : public Xbox360::Xbox360SessionInfo
    {
    public:
        /// constructor
        SessionInfo();
    };
}
#endif
#else
#error "SessionInfo class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
