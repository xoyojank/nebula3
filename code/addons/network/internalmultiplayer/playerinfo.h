#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::PlayerInfo

    This struct is just used for messages at the moment.

    (C) 2009 Radon Labs GmbH
*/
//------------------------------------------------------------------------------
#include "util/string.h"
#include "internalmultiplayer/handle.h"
#include "internalmultiplayer/base/internalplayerbase.h"

namespace InternalMultiplayer
{
struct PlayerInfo 
{   
    Util::StringAtom gamerTag;
    PlayerHandle playerHandle;
    Base::InternalPlayerBase::SignInState signInState;
};
} // namespace InternalMultiplayer
//-------------------