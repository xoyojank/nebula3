#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::PacketChannelBase

    ChannelIds for different channels where packets can be send thru.

(C) 2009 Radon Labs GmbH
*/    
//------------------------------------------------------------------------------
#include "core/types.h"

namespace Base
{
class PacketChannelBase
{
public:
    enum Code
    {
        DefaultChannel = 0, // default channel
        ChatChannel,        // channel for sending chat messages
        MiscChannel         // channel for own ordering
    };
};

} // namespace InternalMultiplayer
//-------------------