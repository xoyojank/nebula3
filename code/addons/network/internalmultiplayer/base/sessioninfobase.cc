//------------------------------------------------------------------------------
//  internalplayerbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/base/sessioninfobase.h"

namespace Base
{
//------------------------------------------------------------------------------
/**
*/
SessionInfoBase::SessionInfoBase():
    openPublicSlots(0),
    openPrivateSlots(0),
    filledPublicSlots(0),
    filledPrivateSlots(0),
    curSessionType(InternalMultiplayer::MultiplayerType::CoachCoop)
{

}

//------------------------------------------------------------------------------
/**
*/
Util::String 
SessionInfoBase::GetIpAddressaAsString() const
{
    n_error("SessionInfoBase::GetIpAddressaAsString called!");

    return Util::String();
}
} // namespace InternalMultiplayer
