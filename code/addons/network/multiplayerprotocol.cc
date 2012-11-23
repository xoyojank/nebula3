//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerprotocol.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::PlayerMessage, 'PLME', Messaging::Message);
    __ImplementMsgId(PlayerMessage);
    __ImplementClass(Multiplayer::NetworkEventBase, 'NEBA', Messaging::Message);
    __ImplementMsgId(NetworkEventBase);
    __ImplementClass(Multiplayer::CreatePlayers, 'CRPL', Messaging::Message);
    __ImplementMsgId(CreatePlayers);
    __ImplementClass(Multiplayer::AddLocalPlayer, 'ADLP', Messaging::Message);
    __ImplementMsgId(AddLocalPlayer);
    __ImplementClass(Multiplayer::RemoveLocalPlayer, 'RMLP', Messaging::Message);
    __ImplementMsgId(RemoveLocalPlayer);
    __ImplementClass(Multiplayer::HostSession, 'HOSE', PlayerMessage);
    __ImplementMsgId(HostSession);
    __ImplementClass(Multiplayer::StartGameSearch, 'STGS', PlayerMessage);
    __ImplementMsgId(StartGameSearch);
    __ImplementClass(Multiplayer::JoinSession, 'JOSE', PlayerMessage);
    __ImplementMsgId(JoinSession);
    __ImplementClass(Multiplayer::LeaveSession, 'LESE', PlayerMessage);
    __ImplementMsgId(LeaveSession);
    __ImplementClass(Multiplayer::DeleteSession, 'DESE', Messaging::Message);
    __ImplementMsgId(DeleteSession);
    __ImplementClass(Multiplayer::SetPlayerReady, 'SPLR', PlayerMessage);
    __ImplementMsgId(SetPlayerReady);
    __ImplementClass(Multiplayer::StartGame, 'STGA', Messaging::Message);
    __ImplementMsgId(StartGame);
    __ImplementClass(Multiplayer::EndGame, 'ENGA', Messaging::Message);
    __ImplementMsgId(EndGame);
    __ImplementClass(Multiplayer::StartingGameFinished, 'STGF', Messaging::Message);
    __ImplementMsgId(StartingGameFinished);
    __ImplementClass(Multiplayer::SendDataToPlayer, 'SDTP', PlayerMessage);
    __ImplementMsgId(SendDataToPlayer);
    __ImplementClass(Multiplayer::BroadcastData, 'SDTA', Messaging::Message);
    __ImplementMsgId(BroadcastData);
    __ImplementClass(Multiplayer::SendDataToHost, 'SDTH', Messaging::Message);
    __ImplementMsgId(SendDataToHost);
    __ImplementClass(Multiplayer::AttachNotificationHandler, 'ANOH', Messaging::Message);
    __ImplementMsgId(AttachNotificationHandler);
    __ImplementClass(Multiplayer::RemoveNotificationHandler, 'RNOH', Messaging::Message);
    __ImplementMsgId(RemoveNotificationHandler);
} // Multiplayer

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
