#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::RakNetPacketChannel


    conversion between nebula3 and platform type

(C) 2009 Radon Labs GmbH
*/    
#include "internalmultiplayer/base/packetchannelbase.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RakNetPacketChannel : public Base::PacketChannelBase
{
public:
    // Raknet can have 32 order channels, we just use 3 cause qnet uses also only 3 for xbox360
    enum RakNetChannel
    {
        RakNetDefaultChannel = 0,
        RakNetChatChannel,
        RakNetMiscChannel
    };
    
    /// conversion from platform type to nebula3 type
    static Base::PacketChannelBase::Code AsNebula3Channel(char raknetChannel);
    /// conversion from nebula3 to platform type
    static char AsRakNetChannel(Base::PacketChannelBase::Code nebulaChannel);

};

} // namespace InternalMultiplayer
//-------------------