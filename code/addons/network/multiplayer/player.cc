//------------------------------------------------------------------------------
//  network/multiplayer/player.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayer/player.h"

namespace Multiplayer
{
__ImplementClass(Player, 'MPPL', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Player::Player() :  loginState(Base::InternalPlayerBase::NotSignedIn),
                    ready(false),
                    local(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Player::~Player()
{
    // empty
}

} // namespace Multiplayer
