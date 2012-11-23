#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::PacketPriorityBase

    Priority of Packets, used by Platform specific network system.
    Look at its implementation for further information how this priority is used.

(C) 2009 Radon Labs GmbH
*/    
//------------------------------------------------------------------------------
#include "core/types.h"

namespace Base
{
class PacketPriorityBase
{
public:
    enum Code
    {
        LowPriority = 0,
        NormalPriority,
        HighPriority,

        NumPacketPriorities
    };
};

} // namespace InternalMultiplayer
//-------------------