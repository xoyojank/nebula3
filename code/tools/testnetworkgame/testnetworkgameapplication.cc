//------------------------------------------------------------------------------
//  testgameapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testnetworkgame/testnetworkgameapplication.h"
#include "tools/testnetworkgame/testgamestate.h"
#include "tools/testnetworkgame/lobbygamestate.h"
#include "tools/testnetworkgame/testgamenotificationhandler.h"
#include "application/basegamefeature/statehandlers/gamestatehandler.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "render/debugrender/debugrender.h"

namespace Tools
{
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;

__ImplementSingleton(TestNetworkGameApplication);   
//------------------------------------------------------------------------------
/**
*/
TestNetworkGameApplication::TestNetworkGameApplication()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TestNetworkGameApplication::~TestNetworkGameApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
TestNetworkGameApplication::SetupStateHandlers()
{  
    // create our lobby gamestatehandler
    Ptr<Tools::LobbyGameState> lobbyState = Tools::LobbyGameState::Create();
    lobbyState->SetSetupMode(BaseGameFeature::GameStateHandler::NewGame);
    lobbyState->SetName("LobbyState");
    lobbyState->SetLevelName("testlevel");  
    this->AddStateHandler(lobbyState.get());

    // create our standard gamestatehandler
    Ptr<Tools::TestGameState> gameState = Tools::TestGameState::Create();
    gameState->SetSetupMode(BaseGameFeature::GameStateHandler::NewGame);
    gameState->SetName("GameState");
    // set level from session info
    gameState->SetLevelName("testlevel");    
    this->AddStateHandler(gameState.get());

    this->SetState("LobbyState");
}

//------------------------------------------------------------------------------
/**
*/
void
TestNetworkGameApplication::SetupGameFeatures()
{
    GameApplication::SetupGameFeatures();

    this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
    this->gameServer->AttachGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
    this->baseGameFeature = BaseGameFeature::BaseGameFeatureUnit::Create();
    this->gameServer->AttachGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());
    this->multiplayerFeature = MultiplayerFeature::MultiplayerFeatureUnit::Create();
    this->multiplayerFeature->SetNotificationHandlerRtti(&TestGameNotificationHandler::RTTI);
    this->gameServer->AttachGameFeature(this->multiplayerFeature.upcast<Game::FeatureUnit>());

#if __USE_PHYSICS__
    // create and attach core features
    this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create();    
    this->gameServer->AttachGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
#endif	
}

//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
TestNetworkGameApplication::CleanupGameFeatures()
{    
#if __USE_PHYSICS__
    this->gameServer->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
    this->physicsFeature = 0;
#endif
    this->gameServer->RemoveGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());
    this->gameServer->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
    this->gameServer->RemoveGameFeature(this->multiplayerFeature.upcast<Game::FeatureUnit>());

    this->baseGameFeature = 0;
    this->graphicsFeature = 0;
    this->multiplayerFeature = 0;

    GameApplication::CleanupGameFeatures();
}
} // namespace Tools
