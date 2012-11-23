//------------------------------------------------------------------------------
//  InternalMultiplayerhandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/internalmultiplayerhandler.h"
#include "internalmultiplayer/internalmultiplayerinterface.h"
#include "internalmultiplayer/debug/networkpagehandler.h"
#include "multiplayerprotocol.h"

namespace InternalMultiplayer
{
__ImplementClass(InternalMultiplayer::InternalMultiplayerHandler, 'IMHD', Messaging::Handler);

using namespace Messaging;
using namespace Timing;
using namespace FrameSync;

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerHandler::InternalMultiplayerHandler() :
    isInternalMultiplayerRuntimeValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalMultiplayerHandler::~InternalMultiplayerHandler()
{
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerHandler::Open()
{
    n_assert(!this->IsOpen());
    InterfaceHandlerBase::Open();

    this->internalMultiplayerServer = InternalMultiplayerServer::Create();
    this->internalMultiplayerServer->Open();
    this->frameSyncTimer = FrameSyncTimer::Create();
    this->frameSyncTimer->Setup();

#if __NEBULA3_HTTP__
    // setup http page handlers
    this->httpServerProxy = Http::HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::NetworkPageHandler::Create());
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
InternalMultiplayerHandler::Close()
{
    n_assert(this->IsOpen());    

    this->internalMultiplayerServer->Close();

#if __NEBULA3_HTTP__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
#endif

    this->frameSyncTimer->Discard();
    this->frameSyncTimer = 0;
    this->internalMultiplayerServer = 0;
    InterfaceHandlerBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalMultiplayerHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(Multiplayer::HostSession::Id))
    {
        this->internalMultiplayerServer->OnHostSession(msg.cast<Multiplayer::HostSession>());
    }
    else if (msg->CheckId(Multiplayer::StartGameSearch::Id))
    {
        this->internalMultiplayerServer->OnStartGameSearch(msg.cast<Multiplayer::StartGameSearch>());
    }
    else if (msg->CheckId(Multiplayer::JoinSession::Id))
    {
        this->internalMultiplayerServer->OnJoinSession(msg.cast<Multiplayer::JoinSession>());
    }
    else if (msg->CheckId(Multiplayer::LeaveSession::Id))
    {
        this->internalMultiplayerServer->OnLeaveSession(msg.cast<Multiplayer::LeaveSession>());
    }
    else if (msg->CheckId(Multiplayer::CreatePlayers::Id))
    {
        this->internalMultiplayerServer->OnCreatePlayers(msg.cast<Multiplayer::CreatePlayers>());
    }
    else if (msg->CheckId(Multiplayer::AttachNotificationHandler::Id))
    {
        this->internalMultiplayerServer->OnAttachNotificationHandler(msg.cast<Multiplayer::AttachNotificationHandler>());
    }
    else if (msg->CheckId(Multiplayer::RemoveNotificationHandler::Id))
    {
        this->internalMultiplayerServer->OnRemoveNotificationHandler(msg.cast<Multiplayer::RemoveNotificationHandler>());
    }
    else if (msg->CheckId(Multiplayer::SetPlayerReady::Id))
    {
        this->internalMultiplayerServer->OnSetPlayerReady(msg.cast<Multiplayer::SetPlayerReady>());
    }
    else if (msg->CheckId(Multiplayer::StartGame::Id))
    {
        this->internalMultiplayerServer->OnStartGame(msg.cast<Multiplayer::StartGame>());
    }
    else if (msg->CheckId(Multiplayer::StartingGameFinished::Id))
    {
        this->internalMultiplayerServer->OnStartingGameFinished(msg.cast<Multiplayer::StartingGameFinished>());
    }
    else if (msg->CheckId(Multiplayer::EndGame::Id))
    {
        this->internalMultiplayerServer->OnEndGame(msg.cast<Multiplayer::EndGame>());
    }
    else if (msg->CheckId(Multiplayer::SendDataToPlayer::Id))
    {
        this->internalMultiplayerServer->OnSendDataToPlayer(msg.cast<Multiplayer::SendDataToPlayer>());        
    } 
    else if (msg->CheckId(Multiplayer::SendDataToHost::Id))
    {
        this->internalMultiplayerServer->OnSendDataToHost(msg.cast<Multiplayer::SendDataToHost>());        
    }
    else if (msg->CheckId(Multiplayer::BroadcastData::Id))
    {
        this->internalMultiplayerServer->OnBroadcastData(msg.cast<Multiplayer::BroadcastData>());
    }
    else if (msg->CheckId(Multiplayer::DeleteSession::Id))
    {
        this->internalMultiplayerServer->OnDeleteSession(msg.cast<Multiplayer::DeleteSession>());
    }
    else if (msg->CheckId(Multiplayer::AddLocalPlayer::Id))
    {
        this->internalMultiplayerServer->OnAddLocalPlayer(msg.cast<Multiplayer::AddLocalPlayer>());
    }
    else if (msg->CheckId(Multiplayer::RemoveLocalPlayer::Id))
    {
        this->internalMultiplayerServer->OnRemoveLocalPlayer(msg.cast<Multiplayer::RemoveLocalPlayer>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
    This is the per-frame method which implements the asynchronous render-loop.
*/
void
InternalMultiplayerHandler::DoWork()
{    
    n_assert(this->IsOpen());

    // update time by polling the FrameSyncThreadHandler
    this->frameSyncTimer->UpdateTimePolling();

    // first signal the global frameEvent, to notify other threads
    // that we are finished with message processing
    InternalMultiplayerInterface::Instance()->SignalFrameEvent();    

    // trigger network system
    this->internalMultiplayerServer->OnFrame();

#if __NEBULA3_HTTP__
    // allow audio-thread HttpRequests to be processed
    this->httpServerProxy->HandlePendingRequests();
#endif
    Core::SysFunc::Sleep(0.04);
}

} // namespace InternalMultiplayer

