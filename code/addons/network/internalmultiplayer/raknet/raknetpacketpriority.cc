//------------------------------------------------------------------------------
//  internalplayerbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknetpacketpriority.h"

namespace RakNet
{

//------------------------------------------------------------------------------
/**
*/
Base::PacketPriorityBase::Code 
RakNetPacketPriority::AsNebula3Priority(PacketPriority raknetPriority)
{
    switch (raknetPriority)
    {
    case LOW_PRIORITY: return Base::PacketPriorityBase::LowPriority;
    case MEDIUM_PRIORITY: return Base::PacketPriorityBase::NormalPriority;
    case HIGH_PRIORITY : return Base::PacketPriorityBase::HighPriority;
    };
    return Base::PacketPriorityBase::NormalPriority;
}

//------------------------------------------------------------------------------
/**
*/
PacketPriority 
RakNetPacketPriority::AsRaknetPriority(Base::PacketPriorityBase::Code nebulaPriority)
{
    switch (nebulaPriority)
    {
    case Base::PacketPriorityBase::LowPriority: return LOW_PRIORITY;
    case Base::PacketPriorityBase::NormalPriority: return MEDIUM_PRIORITY;
    case Base::PacketPriorityBase::HighPriority: return HIGH_PRIORITY;
    };
    return MEDIUM_PRIORITY;
}
} // namespace InternalMultiplayer
