#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalSession

    The InternalSession has a proxy object on the mainthread side.
    A Session allows joining and leaving of players.
    It encloses Multiplayer states like GameLobby and GamePlay. 
      
    (C) 2009 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "internalmultiplayer/internalplayer.h"
#include "internalmultiplayer/handle.h"
#include "internalmultiplayer/sessioninfo.h"

namespace Base
{
    class InternalMultiplayerServerBase;
}
//------------------------------------------------------------------------------
namespace InternalMultiplayer
{

class InternalSession : public Core::RefCounted
{
    __DeclareClass(InternalSession);
public:
    /// constructor
    InternalSession();
    /// destructor
    virtual ~InternalSession();
    /// clear session, removes all players
    void ClearSession();
    /// start this session
    void StartSession();
    /// end this session
    void EndSession();
    /// is session started
    bool IsStarted() const;
    /// get SessionInfo	
    const InternalMultiplayer::SessionInfo& GetSessionInfo() const;
    /// set SessionInfo
    void SetSessionInfo(const InternalMultiplayer::SessionInfo& val);
    /// get players in session
    Util::Array<Ptr<InternalPlayer> >  GetPlayers(bool noHost) const;
    /// get player by unique id
    const Ptr<InternalPlayer>& GetPlayerByUniqueId(const UniquePlayerId& id) const;  
    /// get HostPlayer	
    const Ptr<InternalMultiplayer::InternalPlayer>& GetHostPlayer() const;

protected:
    friend class Base::InternalMultiplayerServerBase;
    /// add player to session
    void AddPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player, bool publicSlot = true);
    /// remove player from session
    void RemovePlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// set host player
    void SetHostPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);

    InternalMultiplayer::SessionInfo sessionInfo;
    bool started;
    Util::Dictionary<UniquePlayerId, Ptr<InternalMultiplayer::InternalPlayer> > joinedPlayers;  
    Ptr<InternalMultiplayer::InternalPlayer> hostPlayer;
};

//------------------------------------------------------------------------------
/**
*/
inline const InternalMultiplayer::SessionInfo& 
InternalSession::GetSessionInfo() const
{
    return this->sessionInfo;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalSession::SetSessionInfo(const InternalMultiplayer::SessionInfo& val)
{
    this->sessionInfo = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<Ptr<InternalPlayer> > 
InternalSession::GetPlayers(bool noLocalPlayers) const
{
    if (noLocalPlayers)
    {
        Util::Array<Ptr<InternalPlayer> > playersWithoutHost = this->joinedPlayers.ValuesAsArray();
        IndexT i;
        for (i = 0; i < playersWithoutHost.Size(); ++i)
        {
        	if (playersWithoutHost[i]->IsLocal())
            {
                playersWithoutHost.EraseIndex(i);
                --i;
            }
        }
        return playersWithoutHost;
    }
    return this->joinedPlayers.ValuesAsArray();
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
InternalSession::IsStarted() const
{
    return this->started;
}  

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalMultiplayer::InternalPlayer>& 
InternalSession::GetHostPlayer() const
{
    return this->hostPlayer;
}   

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalSession::SetHostPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{
    this->hostPlayer = player;
}
} // namespace InternalMultiplayer
//------------------------------------------------------------------------------