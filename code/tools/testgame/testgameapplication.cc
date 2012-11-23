//------------------------------------------------------------------------------
//  testgameapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testgame/testgameapplication.h"

#include "basegamefeature/statehandlers/gamestatehandler.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"


namespace Tools
{
using namespace BaseGameFeature;
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;

__ImplementSingleton(TestGameApplication);   
//------------------------------------------------------------------------------
/**
*/
TestGameApplication::TestGameApplication()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TestGameApplication::~TestGameApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameApplication::SetupStateHandlers()
{
    // create our standard gamestatehandler
    Ptr<GameStateHandler> gameState = GameStateHandler::Create();
    gameState->SetSetupMode(GameStateHandler::NewGame);
    gameState->SetName("GameState");
    gameState->SetLevelName("testlevel");
    
    this->AddStateHandler(gameState.get());

    this->SetState("GameState");
}

//------------------------------------------------------------------------------
/**
*/
void
TestGameApplication::SetupGameFeatures()
{
    GameApplication::SetupGameFeatures();

    // create and attach default graphic features
    this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
    this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
    this->graphicsFeature->SetRenderDebug(true);
    this->gameServer->AttachGameFeature(this->graphicsFeature.cast<Game::FeatureUnit>());

    // game features
    this->baseGameFeature = BaseGameFeature::BaseGameFeatureUnit::Create();
    this->gameServer->AttachGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());

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
TestGameApplication::CleanupGameFeatures()
{    
    this->gameServer->RemoveGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());  
    this->baseGameFeature = 0;
    this->gameServer->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
    this->graphicsFeature = 0;
#if __USE_PHYSICS__
    this->gameServer->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
    this->physicsFeature = 0;
 #endif
    GameApplication::CleanupGameFeatures();
}

//------------------------------------------------------------------------------
/**
*/
bool 
TestGameApplication::Open()
{
    bool ret = App::GameApplication::Open();
    return ret;
}

} // namespace Tools
