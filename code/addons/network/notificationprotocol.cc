//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "notificationprotocol.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::ReturnNotification, 'RENO', Messaging::Message);
    __ImplementMsgId(ReturnNotification);
    __ImplementClass(Multiplayer::LocalPlayerAdded, 'LPSA', Messaging::Message);
    __ImplementMsgId(LocalPlayerAdded);
    __ImplementClass(Multiplayer::LocalPlayerRemoved, 'LPSR', Messaging::Message);
    __ImplementMsgId(LocalPlayerRemoved);
    __ImplementClass(Multiplayer::CreatePlayersSucceeded, 'CPLS', ReturnNotification);
    __ImplementMsgId(CreatePlayersSucceeded);
    __ImplementClass(Multiplayer::CreatePlayersFailed, 'CPLF', ReturnNotification);
    __ImplementMsgId(CreatePlayersFailed);
    __ImplementClass(Multiplayer::HostSessionSucceeded, 'HSSU', ReturnNotification);
    __ImplementMsgId(HostSessionSucceeded);
    __ImplementClass(Multiplayer::HostSessionFailed, 'HSFA', ReturnNotification);
    __ImplementMsgId(HostSessionFailed);
    __ImplementClass(Multiplayer::StartGameSearchFinished, 'SGSF', ReturnNotification);
    __ImplementMsgId(StartGameSearchFinished);
    __ImplementClass(Multiplayer::JoinSessionSucceeded, 'JSES', ReturnNotification);
    __ImplementMsgId(JoinSessionSucceeded);
    __ImplementClass(Multiplayer::JoinSessionFailed, 'JSFA', ReturnNotification);
    __ImplementMsgId(JoinSessionFailed);
    __ImplementClass(Multiplayer::StartGameSucceeded, 'SGAS', ReturnNotification);
    __ImplementMsgId(StartGameSucceeded);
    __ImplementClass(Multiplayer::StartGameFailed, 'SGAF', ReturnNotification);
    __ImplementMsgId(StartGameFailed);
    __ImplementClass(Multiplayer::EndGameSucceeded, 'EGAS', ReturnNotification);
    __ImplementMsgId(EndGameSucceeded);
    __ImplementClass(Multiplayer::EndGameFailed, 'EGAF', ReturnNotification);
    __ImplementMsgId(EndGameFailed);
    __ImplementClass(Multiplayer::LeaveSessionSucceeded, 'LSES', ReturnNotification);
    __ImplementMsgId(LeaveSessionSucceeded);
    __ImplementClass(Multiplayer::LeaveSessionFailed, 'LSEF', ReturnNotification);
    __ImplementMsgId(LeaveSessionFailed);
    __ImplementClass(Multiplayer::DeleteSessionSucceded, 'DSES', ReturnNotification);
    __ImplementMsgId(DeleteSessionSucceded);
    __ImplementClass(Multiplayer::DeleteSessionFailed, 'DSEF', ReturnNotification);
    __ImplementMsgId(DeleteSessionFailed);
    __ImplementClass(Multiplayer::PlayerSignInChanged, 'PSIC', Messaging::Message);
    __ImplementMsgId(PlayerSignInChanged);
    __ImplementClass(Multiplayer::PlayerJoinedSession, 'PLJO', Messaging::Message);
    __ImplementMsgId(PlayerJoinedSession);
    __ImplementClass(Multiplayer::PlayerLeftSession, 'PLEO', Messaging::Message);
    __ImplementMsgId(PlayerLeftSession);
    __ImplementClass(Multiplayer::DisconnectFromServer, 'DCSR', Messaging::Message);
    __ImplementMsgId(DisconnectFromServer);
    __ImplementClass(Multiplayer::GameStarting, 'GSTI', Messaging::Message);
    __ImplementMsgId(GameStarting);
    __ImplementClass(Multiplayer::GameStarted, 'GAST', Messaging::Message);
    __ImplementMsgId(GameStarted);
    __ImplementClass(Multiplayer::GameEnding, 'GEIG', Messaging::Message);
    __ImplementMsgId(GameEnding);
    __ImplementClass(Multiplayer::GameEnded, 'GEND', Messaging::Message);
    __ImplementMsgId(GameEnded);
    __ImplementClass(Multiplayer::PlayerReadyChanged, 'PRCH', Messaging::Message);
    __ImplementMsgId(PlayerReadyChanged);
    __ImplementClass(Multiplayer::DataReceived, 'DARC', Messaging::Message);
    __ImplementMsgId(DataReceived);
} // Multiplayer

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
