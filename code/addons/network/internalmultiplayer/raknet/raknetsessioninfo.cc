//------------------------------------------------------------------------------
//  internalplayerbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknetsessioninfo.h"

namespace RakNet
{
//------------------------------------------------------------------------------
/**
*/
RakNetSessionInfo::RakNetSessionInfo()
{

}

//------------------------------------------------------------------------------
/**
*/
Util::String 
RakNetSessionInfo::GetIpAddressaAsString() const
{
    Util::String address(this->externalIp.ToString());
    return address;
}
} // namespace InternalMultiplayer
