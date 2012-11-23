#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::RakNetPacketPriority



    Conversion between nebula3 and platform type.

(C) 2009 Radon Labs GmbH
*/    
#include "internalmultiplayer/base/packetprioritybase.h"
#include "extlibs/raknet/packetpriority.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RakNetPacketPriority : public Base::PacketPriorityBase
{
public:
    /// conversion from platform type to nebula3 type
    static Base::PacketPriorityBase::Code AsNebula3Priority(PacketPriority raknetPriority);
    /// conversion from nebula3 to platform type
    static PacketPriority AsRaknetPriority(Base::PacketPriorityBase::Code nebulaPriority);

};

} // namespace InternalMultiplayer
//-------------------