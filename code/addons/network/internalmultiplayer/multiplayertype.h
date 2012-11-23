#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::SeesionType

    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace InternalMultiplayer
{
class MultiplayerType
{
public:
    /// index types enum
    enum Code
    {
        CoachCoop,          // multi player on same system 
        LocalNetwork,       // multi player on a local network or system link
        OnlineStandard,     // multi player over internet 
        OnlineRanked,       // multi player over internet with special constraints for competition play

        NumMultiplayerTypes,
    };
};
}
//------------------------------------------------------------------------------
