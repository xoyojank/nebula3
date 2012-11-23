//------------------------------------------------------------------------------
//  internalmultiplayerserverbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/base/internalmultiplayerserverbase.h"
#include "multiplayer/base/multiplayernotificationhandlerbase.h"
#include "internalmultiplayer/playerinfo.h"
#include "notificationprotocol.h"

namespace Base
{
__ImplementClass(Base::InternalMultiplayerServerBase, 'IMSB', Core::RefCounted);
__ImplementSingleton(Base::InternalMultiplayerServerBase);

using namespace Util;
using namespace InternalMultiplayer;

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerServerBase::InternalMultiplayerServerBase() :
    isOpen(false),
    isHosting(false),
    curMultiplayerType(MultiplayerType::CoachCoop)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerServerBase::~InternalMultiplayerServerBase()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerServerBase::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerServerBase::Close()
{
    n_assert(this->isOpen);
    this->isOpen = false;
}    

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnFrame()
{
#if NEBULA3_ENABLE_PROFILING
    this->UpdateStatistics();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnHostSession(const Ptr<Multiplayer::HostSession>& msg)
{
    n_error("InternalMultiplayerServerBase::OnHostSession called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnStartGameSearch(const Ptr<Multiplayer::StartGameSearch>& msg)
{
    n_error("InternalMultiplayerServerBase::OnStartGameSearch called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnJoinSession(const Ptr<Multiplayer::JoinSession>& msg)
{
    n_error("InternalMultiplayerServerBase::OnJoinSession called");
}


//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnAddLocalPlayer( const Ptr<Multiplayer::AddLocalPlayer>& msg )
{
    // session exists? one slot free?
    n_assert(this->activeSession.isvalid());
    n_assert(this->activeSession->GetSessionInfo().GetOpenPrivateSlots() > 0 ||
             this->activeSession->GetSessionInfo().GetOpenPublicSlots() > 0);

    // create internal players
    Ptr<InternalMultiplayer::InternalPlayer> player = InternalMultiplayer::InternalPlayer::Create();    
    player->SetPlayerName(Util::StringAtom("LocalTestPlayer" + Util::String::FromInt(this->localPlayers.Size())));
    player->SetSignInState(InternalMultiplayer::InternalPlayer::SignedInOnline);
    player->SetLocal(true);
    // directly add to our server
    this->AddLocalPlayer(player); 

    this->activeSession->AddPlayer(player);

    // show sign in gui to allow user to sign in
    // TODO: handle sign in, need friend / buddy system
    InternalMultiplayer::PlayerInfo newInfo;
    newInfo.gamerTag = player->GetPlayerName();
    newInfo.playerHandle = player.get();
    newInfo.signInState = player->GetSignInState();

    // send notification             
    Ptr<Multiplayer::LocalPlayerAdded> notifyMsg = Multiplayer::LocalPlayerAdded::Create();
    notifyMsg->SetPlayerInfo(newInfo);
    this->PutNotification(notifyMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnRemoveLocalPlayer( const Ptr<Multiplayer::RemoveLocalPlayer>& msg )
{
    // from server
    this->RemoveLocalPlayer(msg->GetPlayerHandle().cast<InternalMultiplayer::InternalPlayer>());

    // from active session
    this->activeSession->RemovePlayer(msg->GetPlayerHandle().cast<InternalMultiplayer::InternalPlayer>());

    // send notification             
    Ptr<Multiplayer::LocalPlayerRemoved> notifyMsg = Multiplayer::LocalPlayerRemoved::Create();
    notifyMsg->SetPlayerHandle(msg->GetPlayerHandle());
    this->PutNotification(notifyMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalMultiplayer::InternalPlayer> 
InternalMultiplayerServerBase::GetPlayerByUniqueId(const InternalMultiplayer::UniquePlayerId& uid)
{
    IndexT i;
    for (i = 0; i < this->players.Size(); ++i)
    {
        const UniquePlayerId& playerId = this->players[i]->GetUnqiueId();
        if (uid == playerId)
        {
            return this->players[i];
        }
    }
    return Ptr<InternalPlayer>();
}

//------------------------------------------------------------------------------
/**
    Invoked from MAINTHREAD
*/
void 
InternalMultiplayerServerBase::OnAttachNotificationHandler(const Ptr<Multiplayer::AttachNotificationHandler>& msg)
{
    n_assert(msg->GetNotificationHandler().isvalid());
    this->notificationHandler = msg->GetNotificationHandler();
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnRemoveNotificationHandler(const Ptr<Multiplayer::RemoveNotificationHandler>& msg)
{
    this->notificationHandler = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::PutNotification(const Ptr<Messaging::Message>& msg)
{
    n_assert(this->notificationHandler.isvalid());
    this->notificationHandler->PutNotification(msg);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnSetPlayerReady(const Ptr<Multiplayer::SetPlayerReady>& msg)
{
    n_error("InternalMultiplayerServerBase::OnSetPlayerReady called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnStartGame(const Ptr<Multiplayer::StartGame>& msg)
{
    n_error("InternalMultiplayerServerBase::OnStartGame called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnEndGame(const Ptr<Multiplayer::EndGame>& msg)
{
    n_error("InternalMultiplayerServerBase::OnEndGame called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnSendDataToPlayer(const Ptr<Multiplayer::SendDataToPlayer>& msg)
{
    n_error("InternalMultiplayerServerBase::OnSendDataToPlayer called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnBroadcastData(const Ptr<Multiplayer::BroadcastData>& msg)
{
    n_error("InternalMultiplayerServerBase::OnBroadcastData called");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::AddPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{    
    // all players 
    if (this->players.FindIndex(player) == InvalidIndex)
    {
        this->players.Append(player);
    }
    
    if (this->activeSession.isvalid())
    {
        // if any non local player joins we must have a valid session         
        this->activeSession->AddPlayer(player);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::AddPlayers(const Util::Array<Ptr<InternalMultiplayer::InternalPlayer> >& players)
{
    IndexT i;
    for (i = 0; i < players.Size(); ++i)
    {      
        this->AddPlayer(players[i]);	
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::AddPlayerHandles(const Util::Array<InternalMultiplayer::PlayerHandle>& playerHandles)
{
    IndexT i;
    for (i = 0; i < playerHandles.Size(); ++i)
    {
        const Ptr<InternalPlayer>& player = playerHandles[i].cast<InternalPlayer>();        
        this->AddPlayer(player);	
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::RemovePlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{
    // first remove from any session
    if (player->IsInSession())
    {
        // if any non local player leaves we must have a valid session
        n_assert(this->activeSession.isvalid());
        this->activeSession->RemovePlayer(player);
    }

    IndexT arrayIndex = this->players.FindIndex(player);
    n_assert(arrayIndex != InvalidIndex);
    this->players.EraseIndex(arrayIndex);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::AddLocalPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{
    n_assert(this->localPlayers.FindIndex(player) == InvalidIndex);
    n_assert(player->IsLocal());
    this->localPlayers.Append(player);    
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::RemoveLocalPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player)
{
    IndexT arrayIndex = this->localPlayers.FindIndex(player);
    n_assert(arrayIndex != InvalidIndex);
    this->localPlayers.EraseIndex(arrayIndex);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnStartingGameFinished(const Ptr<Multiplayer::StartingGameFinished>& msg)
{
    n_error("InternalMultiplayerServerBase::OnStartingGameFinished called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::OnSendDataToHost(const Ptr<Multiplayer::SendDataToHost>& msg)
{                                      
    n_error("InternalMultiplayerServerBase::OnSendDataToHost called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::UpdateStatistics()
{     
    n_error("InternalMultiplayerServerBase::UpdateStatistics called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalMultiplayerServerBase::WriteStatistics(const Ptr<Http::HtmlPageWriter>& writer)
{      
    n_error("InternalMultiplayerServerBase::WriteStatistics called!");      
}
} // namespace InternalMultiplayer
