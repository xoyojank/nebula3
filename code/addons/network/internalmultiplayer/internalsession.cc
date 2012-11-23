//------------------------------------------------------------------------------
//  internalsession.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/internalsession.h"

namespace InternalMultiplayer
{
__ImplementClass(InternalMultiplayer::InternalSession, 'INSE', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
InternalSession::InternalSession():
    started(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalSession::~InternalSession()
{
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSession::AddPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player, bool publicSlot)
{
    n_assert(!this->joinedPlayers.Contains(player->GetUnqiueId()));
    player->OnJoinSession(this);
    player->SetIsInPublicSlot(publicSlot);
    this->joinedPlayers.Add(player->GetUnqiueId(), player);    

    // update our session info
    if (publicSlot)
    {
        ushort curOpenPublicSlots = this->sessionInfo.GetOpenPublicSlots();
        n_assert(curOpenPublicSlots > 0);
        this->sessionInfo.SetOpenPublicSlots(curOpenPublicSlots - 1);
        ushort curFilledPublicSlots = this->sessionInfo.GetFilledPublicSlots();
        this->sessionInfo.SetFilledPublicSlots(curFilledPublicSlots + 1);
    }
    else
    {
        ushort curOpenPrivateSlots = this->sessionInfo.GetOpenPrivateSlots();
        n_assert(curOpenPrivateSlots > 0);
        this->sessionInfo.SetOpenPrivateSlots(curOpenPrivateSlots - 1);
        ushort curFilledPrivateSlots = this->sessionInfo.GetFilledPrivateSlots();
        this->sessionInfo.SetFilledPrivateSlots(curFilledPrivateSlots + 1);
    }

    if (player->IsHost())
    {
        n_assert(!this->hostPlayer.isvalid());
        this->SetHostPlayer(player);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSession::RemovePlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{
    n_assert(this->joinedPlayers.Contains(player->GetUnqiueId()));
    player->OnLeaveSession();
    this->joinedPlayers.Erase(player->GetUnqiueId());

    // update our session info
    if (player->GetIsInPublicSlot())
    {
        ushort curFilledPublicSlots = this->sessionInfo.GetFilledPublicSlots();
        n_assert(curFilledPublicSlots > 0);
        this->sessionInfo.SetFilledPublicSlots(curFilledPublicSlots - 1);
        ushort curOpenPublicSlots = this->sessionInfo.GetOpenPublicSlots();
        this->sessionInfo.SetOpenPublicSlots(curOpenPublicSlots + 1);
    }
    else
    {
        ushort curFilledPrivateSlots = this->sessionInfo.GetFilledPrivateSlots();
        n_assert(curFilledPrivateSlots > 0);
        this->sessionInfo.SetFilledPrivateSlots(curFilledPrivateSlots - 1);
        ushort curOpenPrivateSlots = this->sessionInfo.GetOpenPrivateSlots();
        this->sessionInfo.SetOpenPrivateSlots(curOpenPrivateSlots + 1);
    }

    if (player->IsHost())
    {
        n_assert(this->hostPlayer.isvalid());
        this->SetHostPlayer(0);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSession::StartSession()
{
    n_assert(!this->started);
    this->started = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSession::EndSession()
{
    n_assert(this->started);
    this->started = false;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalPlayer>& 
InternalSession::GetPlayerByUniqueId(const UniquePlayerId& id) const
{
    n_assert(this->joinedPlayers.Contains(id));
    return this->joinedPlayers[id];
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalSession::ClearSession()
{
    Util::Array<Ptr<InternalPlayer> > players = this->joinedPlayers.ValuesAsArray();
    IndexT i;
    for (i = 0; i < players.Size(); ++i)
    {
    	this->RemovePlayer(players[i]);
    }        
}
} // namespace InternalMultiplayer
