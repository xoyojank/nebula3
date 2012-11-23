//------------------------------------------------------------------------------
//  network/multiplayer/TestGameNotificationHandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testnetworkgame/testgamenotificationhandler.h"
#include "multiplayerfeature/distributionmanager.h"
#include "multiplayer/multiplayermanager.h"
#include "tools/testnetworkgame/testnetworkgameapplication.h"
#include "testnetworkgame/testgamestate.h"
#include "multiplayerfeature/distributionmanager.h"

namespace Tools
{
    __ImplementClass(Tools::TestGameNotificationHandler, 'TGNH', MultiplayerFeature::DistributionNotificationHandler);

using namespace Multiplayer;
//------------------------------------------------------------------------------
/**
*/
TestGameNotificationHandler::TestGameNotificationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestGameNotificationHandler::~TestGameNotificationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
host invoked a game start, load level and everything to get ready for gameplay
*/
void 
TestGameNotificationHandler::HandleGameStarting(const Ptr<GameStarting>& msg)
{
    // call base class to set right multiplayer manager state
    DistributionNotificationHandler::HandleGameStarting(msg);
    // start loading level
    Tools::TestNetworkGameApplication::Instance()->RequestState("GameState");
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameNotificationHandler::HandlePlayerLeftSession(const Ptr<PlayerLeftSession>& msg)
{
    // remove player entity
    const Ptr<Multiplayer::Player>& player = Multiplayer::MultiplayerManager::Instance()->GetPlayer(msg->GetPlayerHandle());
    const Ptr<App::StateHandler>& stateHandler = Tools::TestNetworkGameApplication::Instance()->GetCurrentStateHandler();
    if (stateHandler->IsA(TestGameState::RTTI))
    {
        stateHandler.cast<TestGameState>()->RemovePlayerEntity(player);
    }

    DistributionNotificationHandler::HandlePlayerLeftSession(msg);
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameNotificationHandler::HandlePlayerJoinedSession(const Ptr<Multiplayer::PlayerJoinedSession>& msg)
{   
    DistributionNotificationHandler::HandlePlayerJoinedSession(msg);
    // is session in progress?
    MultiplayerManager::SessionState curSessionState = MultiplayerManager::Instance()->GetSessionState();
    if (curSessionState != MultiplayerManager::NoSession)
    {
        const Ptr<Multiplayer::Player>& player = Multiplayer::MultiplayerManager::Instance()->GetPlayer(msg->GetPlayerHandle());
        const Ptr<App::StateHandler>& stateHandler = Tools::TestNetworkGameApplication::Instance()->GetCurrentStateHandler();
        if (stateHandler->IsA(TestGameState::RTTI))
        {
            // spawn new player entity
            stateHandler.cast<TestGameState>()->SpawnPlayerEntity(player);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameNotificationHandler::HandleGameStarted( const Ptr<Multiplayer::GameStarted>& msg )
{
    MultiplayerFeature::DistributionManager::Mode mode = MultiplayerFeature::DistributionManager::Instance()->GetCurrentMode();
    if (mode == MultiplayerFeature::DistributionManager::Host)
    {
        // spawn player entities
        TestNetworkGameApplication::Instance()->GetCurrentStateHandler().cast<TestGameState>()->SpawnPlayerEntities();
    }
}

} // namespace Multiplayer
