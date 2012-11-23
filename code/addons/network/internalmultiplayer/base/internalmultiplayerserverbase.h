#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalMultiplayerServerBase

    The platform specific InternalMultiplayerServer 
    creates and manages the network system of the platform.
        
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "internalmultiplayer/multiplayertype.h"
#include "multiplayerprotocol.h"
#include "internalmultiplayer/internalsession.h"
#include "http/html/htmlpagewriter.h"

//------------------------------------------------------------------------------
namespace Base
{
class InternalMultiplayerServerBase : public Core::RefCounted
{
    __DeclareClass(InternalMultiplayerServerBase);
    __DeclareSingleton(InternalMultiplayerServerBase);
public:
    /// constructor
    InternalMultiplayerServerBase();
    /// destructor
    virtual ~InternalMultiplayerServerBase();

    /// open the InternalMultiplayer server 
    void Open();
    /// close the InternalMultiplayer server
    void Close();
    /// return true if InternalMultiplayer server is open
    bool IsOpen() const;
    /// perform client-side per-frame updates
    void OnFrame();

    /// on create player
    void OnCreatePlayers(const Ptr<Multiplayer::CreatePlayers>& msg);
    /// on host a session
    void OnSetGameType(/*const Ptr<Multiplayer::SetGameType>& msg*/);
    /// on host a session
    void OnHostSession(const Ptr<Multiplayer::HostSession>& msg);
    /// on kick a player
    void OnKickPlayer(/*const Ptr<Multiplayer::KickPlayer>& msg*/);
    /// on delete session call
    void OnDeleteSession(const Ptr<Multiplayer::DeleteSession>& msg);

    /// on start game search
    void OnStartGameSearch(const Ptr<Multiplayer::StartGameSearch>& msg);

    /// on join a given session
    void OnJoinSession(const Ptr<Multiplayer::JoinSession>& msg);
    /// on set player ready
    void OnSetPlayerReady(const Ptr<Multiplayer::SetPlayerReady>& msg);
    /// on leave current session
    void OnLeaveSession(const Ptr<Multiplayer::LeaveSession>& msg);

    /// add a local player
    void OnAddLocalPlayer(const Ptr<Multiplayer::AddLocalPlayer>& msg);
    /// remove a local player
    void OnRemoveLocalPlayer(const Ptr<Multiplayer::RemoveLocalPlayer>& msg);

    /// on start game
    void OnStartGame(const Ptr<Multiplayer::StartGame>& msg);
    /// on end game called from host
    void OnEndGame(const Ptr<Multiplayer::EndGame>& msg);
    /// client: on ready of starting a game
    void OnStartingGameFinished(const Ptr<Multiplayer::StartingGameFinished>& msg);

    /// on send data to specific player
    void OnSendDataToPlayer(const Ptr<Multiplayer::SendDataToPlayer>& msg);
    /// on send data to all players
    void OnBroadcastData(const Ptr<Multiplayer::BroadcastData>& msg);    
    /// on send data to host
    void OnSendDataToHost(const Ptr<Multiplayer::SendDataToHost>& msg);

    /// get player by unique id
    Ptr<InternalMultiplayer::InternalPlayer> GetPlayerByUniqueId(const InternalMultiplayer::UniquePlayerId& uid);

    /// attach notification handler
    void OnAttachNotificationHandler(const Ptr<Multiplayer::AttachNotificationHandler>& msg);
    /// remove notification handler
    void OnRemoveNotificationHandler(const Ptr<Multiplayer::RemoveNotificationHandler>& msg);
    /// write statistics to html writer, overwrite in subclass
    void WriteStatistics(const Ptr<Http::HtmlPageWriter>& writer);
    /// get current session
    const Ptr<InternalMultiplayer::InternalSession>& GetSession() const;

protected:

    /// put notification
    void PutNotification(const Ptr<Messaging::Message>& msg);
    /// add player, could be local or not, could be in session or not
    void AddLocalPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// add player, could be local or not, could be in session or not
    void AddPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// add player, could be local or not, could be in session or not
    void AddPlayerHandles(const Util::Array<InternalMultiplayer::PlayerHandle>& playerHandles);
    /// add multiple players, could be local or not, could be in session or not
    void AddPlayers(const Util::Array<Ptr<InternalMultiplayer::InternalPlayer> >& players);
    /// remove player
    void RemovePlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// remove player
    void RemoveLocalPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// is system hosting?
    bool IsSystemHosting() const;
    /// update statistics, overwrite in subclass
    virtual void UpdateStatistics();

    bool isOpen;
    bool isHosting;
    Ptr<InternalMultiplayer::InternalSession> activeSession;  
    InternalMultiplayer::MultiplayerType::Code curMultiplayerType;
    Util::Array<Ptr<InternalMultiplayer::InternalPlayer> > players;
    Util::Array<Ptr<InternalMultiplayer::InternalPlayer> > localPlayers;
    Ptr<Base::MultiplayerNotificationHandlerBase> notificationHandler;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalMultiplayerServerBase::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalMultiplayer::InternalSession>& 
InternalMultiplayerServerBase::GetSession() const
{
    return this->activeSession;
}
} // namespace InternalMultiplayer
//------------------------------------------------------------------------------
