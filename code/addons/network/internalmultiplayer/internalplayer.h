#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalPlayer
      
    (C) 2009 Radon Labs GmbH
*/    
#if __WIN32__ || __PS3__
#include "InternalMultiplayer/raknet/raknetinternalplayer.h"
namespace InternalMultiplayer
{
    class InternalPlayer : public RakNet::RakNetInternalPlayer
    {
        __DeclareClass(InternalPlayer);
    public:
        /// constructor
        InternalPlayer();
        /// destructor
        virtual ~InternalPlayer();
    };
}
#elif __XBOX360__
#if XBOX360_USE_RAKNET
#include "InternalMultiplayer/raknet/raknetinternalplayer.h"
namespace InternalMultiplayer
{
class InternalPlayer : public RakNet::RakNetInternalPlayer
{
    __DeclareClass(InternalPlayer);
public:
    /// constructor
    InternalPlayer();
    /// destructor
    virtual ~InternalPlayer();
};
}
#else
#include "InternalMultiplayer/xbox360/xbox360internalplayer.h"
namespace InternalMultiplayer
{
    class InternalPlayer : public Xbox360::Xbox360InternalPlayer
    {
        __DeclareClass(InternalPlayer);
    public:
        /// constructor
        InternalPlayer();
        /// destructor
        virtual ~InternalPlayer();
    };
}
#endif
#else
#error "InternalPlayer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
