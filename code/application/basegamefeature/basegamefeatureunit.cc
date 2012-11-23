//------------------------------------------------------------------------------
//  game/basegamefeature.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#if __USE_PHYSICS__
#include "physicsfeature/physicsfeatureunit.h"
#endif
#include "appgame/gameapplication.h"
#include "core/factory.h"
#include "basegameattr/basegameattributes.h"
#include "loader/loaderserver.h"
#include "appgame/appconfig.h"
#include "game/gameserver.h"
#include "game/gameserver.h"
#include "addons/db/dbserver.h"
#include "io/ioserver.h"
#include "io/console.h"
#include "loader/entityloader.h"
#include "loader/environmentloader.h"
#include "basegametiming/systemtimesource.h"
#include "basegametiming/inputtimesource.h"
#include "basegametiming/gametimesource.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "debugrender/debugrender.h"
#include "http/httpserverproxy.h"
#include "debug/objectinspectorhandler.h"

// include all properties for known by managers::factorymanager
#include "properties/timeproperty.h"
#include "properties/transformableproperty.h"

namespace BaseGameFeature
{
__ImplementClass(BaseGameFeatureUnit, 'GAGF' , Game::FeatureUnit);
__ImplementSingleton(BaseGameFeatureUnit);

using namespace Timing;
using namespace App;
using namespace Game;
using namespace GraphicsFeature;

//------------------------------------------------------------------------------
/**
*/
BaseGameFeatureUnit::BaseGameFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
BaseGameFeatureUnit::~BaseGameFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::OnActivate()
{
	FeatureUnit::OnActivate();
    
    // setup database subsystem
    this->dbServer = Db::DbServer::Create();
    
    #if(__WII__ || __XBOX360__ || __PS3__)
    this->dbServer->SetWorkingDbInMemory(true);
    #endif
    
    if (!this->dbServer->OpenStaticDatabase("export:db/static.db4"))
    {
        n_error("BaseGameFeature: Failed to open static database 'export/db/static.db4'!");
    }
    // open game database to load global attrs
    if (!this->dbServer->OpenGameDatabase("export:db/game.db4"))
    {
        n_error("BaseGameFeature: Failed to open static database 'export/db/static.db4'!");
    }
    // create additional servers    
    this->loaderServer = BaseGameFeature::LoaderServer::Create();
    this->loaderServer->Open();

    // create audio interface
    this->audioPlayer = Audio2::Audio2Player::Create();

    // create audio interface
    this->audioInterface = Audio2::Audio2Interface::Create();
    this->audioInterface->Open();

    //Create AudioServer
    this->audioServer = Audio2::Audio2Server::Create();
    this->audioServer->Open();

    // attach loader to BaseGameFeature::LoaderServer
    Ptr<BaseGameFeature::EntityLoader> entityloader = BaseGameFeature::EntityLoader::Create();
    Ptr<BaseGameFeature::EnvironmentLoader> environmentloader = BaseGameFeature::EnvironmentLoader::Create();
    this->loaderServer->AttachEntityLoader(entityloader.upcast<BaseGameFeature::EntityLoaderBase>());
    this->loaderServer->AttachEntityLoader(environmentloader.upcast<BaseGameFeature::EntityLoaderBase>());

    // create manager and attach to fetaure
    this->timeManager = TimeManager::Create();
    if (!this->factoryManager.isvalid())
    {
        this->factoryManager = FactoryManager::Create();
    }
    this->focusManager = FocusManager::Create();
    this->entityManager = EntityManager::Create();
    this->globalAttrManager = GlobalAttrsManager::Create();
    this->categoryManager = CategoryManager::Create();
    this->envEntityManager = EnvEntityManager::Create();
    this->animEventManager = AnimEventManager::Create();

    this->AttachManager(this->timeManager.upcast<Game::Manager>());
    this->AttachManager(this->factoryManager.upcast<Game::Manager>());
    this->AttachManager(this->focusManager.upcast<Game::Manager>());
    this->AttachManager(this->entityManager.upcast<Game::Manager>());
    this->AttachManager(this->globalAttrManager.upcast<Game::Manager>()); 
    this->AttachManager(this->categoryManager.upcast<Game::Manager>());
    this->AttachManager(this->envEntityManager.upcast<Game::Manager>());
    this->AttachManager(this->animEventManager.upcast<Game::Manager>());

#if __USE_PHYSICS__
    this->envQueryManager = EnvQueryManager::Create();    
    this->AttachManager(this->envQueryManager.upcast<Game::Manager>());
#endif

    Ptr<SystemTimeSource> systemTimeSource = SystemTimeSource::Create();
    Ptr<GameTimeSource> gameTimeSource = GameTimeSource::Create();
    Ptr<InputTimeSource> inputTimeSource = InputTimeSource::Create();
         
    timeManager->AttachTimeSource(systemTimeSource.upcast<TimeSource>());
    timeManager->AttachTimeSource(gameTimeSource.upcast<TimeSource>());
    timeManager->AttachTimeSource(inputTimeSource.upcast<TimeSource>());

#if __NEBULA3_HTTP__
    // create handler for http debug requests
    this->debugRequestHandler = Debug::ObjectInspectorHandler::Create();
    Http::HttpServerProxy::Instance()->AttachRequestHandler(this->debugRequestHandler);
#endif

    // close after all globals loading from globalattrmanager and everyone who needs it on start
    this->dbServer->CloseGameDatabase();

    // setup vib interface
    this->vibInterface = Vibration::VibrationInterface::Create();
    this->vibInterface->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::OnDeactivate()
{
#if __NEBULA3_HTTP__
    // create handler for http debug requests
    Http::HttpServerProxy::Instance()->RemoveRequestHandler(this->debugRequestHandler);
    this->debugRequestHandler = 0;
#endif

    this->RemoveManager(this->factoryManager.upcast<Game::Manager>());
    this->RemoveManager(this->focusManager.upcast<Game::Manager>());
    this->RemoveManager(this->entityManager.upcast<Game::Manager>());
    this->RemoveManager(this->globalAttrManager.upcast<Game::Manager>()); 
    this->RemoveManager(this->categoryManager.upcast<Game::Manager>());
    this->RemoveManager(this->envEntityManager.upcast<Game::Manager>());
    this->RemoveManager(this->animEventManager.upcast<Game::Manager>()); 
    this->RemoveManager(this->timeManager.upcast<Game::Manager>());

    this->envEntityManager = 0;  
    this->factoryManager = 0;
    this->focusManager = 0;
    this->entityManager = 0;
    this->globalAttrManager = 0;
    this->categoryManager = 0;
    this->animEventManager = 0;
    this->timeManager = 0;

#if __USE_PHYSICS__  
    this->RemoveManager(this->envQueryManager.upcast<Game::Manager>());
    this->envQueryManager = 0;
#endif

    this->loaderServer->Close();
    this->loaderServer = 0;
    
    this->dbServer->Close();
    this->dbServer = 0;

    this->audioServer->Close();
    this->audioServer = 0;

    this->audioInterface->Close();
    this->audioInterface = 0;

    this->audioPlayer= 0;

    this->vibInterface->Close();
    this->vibInterface = 0;

    FeatureUnit::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::OnRenderDebug()
{
    // render debug for all entities and its properties
    // this->entityManager->OnRenderDebug();
    FeatureUnit::OnRenderDebug();

    // print fps
    Timing::Time frameTime = SystemTimeSource::Instance()->GetFrameTime();        
	Util::String txt;
	txt.Format("FPS: %.0f \n", 1/frameTime);
	_debug_text(txt, Math::float2(0.7f, 0.0f), Math::float4(1, 1, 1, 1));
	//IO::Console::Instance()->Print("FPS: %.0f \n", 1/frameTime);
}

//------------------------------------------------------------------------------
/**
    This method returns the startup level which should be loaded when 
    the game starts. The startup level is recorded in the level template
    table as having the StartLevel attribute set to true.
*/
Util::String
BaseGameFeatureUnit::GetStartupLevel()
{    
    // check command line args for overwrite level
    if (this->args.HasArg("-level"))
    {
        // overwrite level set, use this
        return this->args.GetString("-level");
    }
    else
    {
        // go through the category manager, don't make a direct db access!
        Ptr<Db::ValueTable> values = CategoryManager::Instance()->GetTemplateTable("Levels");
        Util::Array<IndexT> rowIndices = values->FindRowIndicesByAttr(Attr::Attribute(Attr::StartLevel, true), false);
        if (rowIndices.IsEmpty())
        {
            n_error("No start level defined!\nCheck db/levels.xml and do a batch export!");
            return "";
        }
        else if (rowIndices.Size() > 1)
        {
            n_error("More then one start level defined!\nCheck db/levels.xml and do a batch export!");
            return "";
        }
        else
        {
            return values->GetString(Attr::Id, rowIndices[0]);
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method starts a new game by creating a copy of the initial
    world database into the current user profile's directory. This copy is 
    the mutable world database where is game is played from. After the copy
    has been created, SetupManager::SetupWorldFromLevel() will be called with
    the given initialLevel.
*/
bool
BaseGameFeatureUnit::NewGame()
{
    BaseGameFeature::UserProfile* userProfile = BaseGameFeature::LoaderServer::Instance()->GetUserProfile();    
    App::StateHandler* curAppStateHandler = App::GameApplication::Instance()->GetCurrentStateHandler();
    n_assert(curAppStateHandler);

    // open database in NewGame mode
    bool dbOpened = false;
#if __WIN32__ 
    dbOpened = Db::DbServer::Instance()->OpenNewGame(userProfile->GetProfileDirectory(), userProfile->GetDatabasePath());    
#else 
    dbOpened = Db::DbServer::Instance()->OpenNewGame(userProfile->GetProfileDirectory(), "export:db/game.db4");
#endif
    n_assert(dbOpened);

    // load attributes and reload categories
    GlobalAttrsManager::Instance()->LoadAttributes();

    // setup the world
    Util::String startupLevel = this->GetStartupLevel();
    this->SetCurrentLevel(startupLevel);
    curAppStateHandler->OnLoadBefore();
    this->SetupWorldFromCurrentLevel();
    curAppStateHandler->OnLoadAfter();

    return true;
}

//------------------------------------------------------------------------------
/**
    Return true if a current game exists (created by NewGame()).
*/
bool
BaseGameFeatureUnit::CurrentGameExists()
{
    return Db::DbServer::Instance()->CurrentGameExists(BaseGameFeature::LoaderServer::Instance()->GetUserProfile()->GetProfileDirectory());
}

//------------------------------------------------------------------------------
/**
    This method continues the game from the last known state (the existing
    world database file in the user profile's directory, created by StartNewGame().
    The method will fail hard if a continue game is not possible
    (you can check this beforehand with the method CurrentGameExists()). The
    method will eventually call SetupManager::Instance()->SetupWorldFromLevel().
*/
bool
BaseGameFeatureUnit::ContinueGame()
{
    n_assert(this->CurrentGameExists());
    BaseGameFeature::LoaderServer* loaderServer = BaseGameFeature::LoaderServer::Instance();
    App::StateHandler* curAppStateHandler = App::GameApplication::Instance()->GetCurrentStateHandler();
    n_assert(curAppStateHandler);

    // open database in NewGame mode
    bool dbOpened = Db::DbServer::Instance()->OpenContinueGame(loaderServer->GetUserProfile()->GetProfileDirectory());
    n_assert(dbOpened);

    // load attributes and reload categories
    GlobalAttrsManager::Instance()->LoadAttributes();

    // setup world
    curAppStateHandler->OnLoadBefore();
    this->SetupWorldFromCurrentLevel();
    curAppStateHandler->OnLoadAfter();

     return true;
}

//------------------------------------------------------------------------------
/**
    Create a new savegame. This will flush all unwritten data back to the
    database, and make a copy of the database.
*/
bool
BaseGameFeatureUnit::SaveGame(const Util::String& saveGameName)
{
    // flush unwritten data back to database
    Game::GameServer::Instance()->NotifyGameSave();

    // save global attributes
    GlobalAttrsManager::Instance()->SaveAttributes();

    // tell CategoryManager to write instance data back into the database
    CategoryManager::Instance()->CommitChangesToDatabase();

    // create the save game
    BaseGameFeature::UserProfile* userProfile = BaseGameFeature::LoaderServer::Instance()->GetUserProfile();    
    bool success = Db::DbServer::Instance()->CreateSaveGame(userProfile->GetProfileDirectory(), userProfile->GetDatabasePath(), userProfile->GetSaveGamePath(saveGameName));
    return success;
}

//------------------------------------------------------------------------------
/**
    Load a saved game. This will overwrite the current world database
    with the saved game database file then call 
    SetupManager::SetupWorldFromCurrentLevel(). If the savegame file
    doesn't exist the method returns false and nothing will change.
*/
bool
BaseGameFeatureUnit::LoadGame(const Util::String& saveGameName)
{
    BaseGameFeature::UserProfile* userProfile = BaseGameFeature::LoaderServer::Instance()->GetUserProfile();    
    App::StateHandler* curAppStateHandler = App::GameApplication::Instance()->GetCurrentStateHandler();
    n_assert(curAppStateHandler);

    // cleanup level
    this->CleanupWorld();

    // open database in NewGame mode
    bool dbOpened = Db::DbServer::Instance()->OpenLoadGame(userProfile->GetProfileDirectory(), userProfile->GetDatabasePath(), userProfile->GetSaveGamePath(saveGameName));
    n_assert(dbOpened);
    
    // load attributes and reload categories
    GlobalAttrsManager::Instance()->LoadAttributes();

    // setup world from saved game
    curAppStateHandler->OnLoadBefore();
    this->SetupWorldFromCurrentLevel();
    curAppStateHandler->OnLoadAfter();

    return true;
}

//------------------------------------------------------------------------------
/**
    Load a level from current active db.
*/
bool
BaseGameFeatureUnit::LoadLevel(const Util::String& levelName)
{
    BaseGameFeature::UserProfile* userProfile = BaseGameFeature::LoaderServer::Instance()->GetUserProfile();    
    App::StateHandler* curAppStateHandler = App::GameApplication::Instance()->GetCurrentStateHandler();
    n_assert(curAppStateHandler);

    // open database in NewGame mode
    bool dbOpened = Db::DbServer::Instance()->OpenNewGame(userProfile->GetProfileDirectory(), userProfile->GetDatabasePath());
    n_assert(dbOpened);

    // load attributes and reload categories
    GlobalAttrsManager::Instance()->LoadAttributes();

    // setup the world
    this->SetCurrentLevel(levelName);
    curAppStateHandler->OnLoadBefore();
    this->SetupWorldFromCurrentLevel();
    curAppStateHandler->OnLoadAfter();

    return true;
}

//------------------------------------------------------------------------------
/**
    Set the current level name, this will immediately update the
    global database attribute "CurrentLevel".
*/
void
BaseGameFeatureUnit::SetCurrentLevel(const Util::String& levelName)
{
    GlobalAttrsManager::Instance()->SetString(Attr::CurrentLevel, levelName);
}

//------------------------------------------------------------------------------
/**
    Get the current level name directly from the database.
*/
Util::String
BaseGameFeatureUnit::GetCurrentLevel() const
{
    if (GlobalAttrsManager::Instance()->HasAttr(Attr::CurrentLevel))
    {
        return GlobalAttrsManager::Instance()->GetString(Attr::CurrentLevel);
    }
    else
    {
        return Util::String("");
    }
}

//------------------------------------------------------------------------------
/**
    Setup a new, empty world.
*/
void
BaseGameFeatureUnit::SetupEmptyWorld()
{
    n_assert(!EntityManager::Instance()->HasActiveEntities());

    // if no world exist create default one
    if (!GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage().isvalid())
    {
        GraphicsFeature::GraphicsFeatureUnit::Instance()->SetupDefaultGraphicsWorld();
    }
    
#if __USE_PHYSICS__
    // create a new physics world
    if (PhysicsFeature::PhysicsFeatureUnit::HasInstance())
    {
        PhysicsFeature::PhysicsFeatureUnit::Instance()->CreateDefaultPhysicsWorld();
    }
    else
    {
        n_printf("No PhysicsFeatureUnit created!");
    }    
#endif
}

//------------------------------------------------------------------------------
/**
    Setup a new world from the level name defined by the currently
    set level name.
*/
void
BaseGameFeatureUnit::SetupWorldFromCurrentLevel()
{
    n_assert(!EntityManager::Instance()->HasActiveEntities());
    // if no world exist create default one
    if (!GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage().isvalid())
    {
        GraphicsFeature::GraphicsFeatureUnit::Instance()->SetupDefaultGraphicsWorld();
    }
    // load level from database
    BaseGameFeature::LoaderServer::Instance()->LoadLevel(this->GetCurrentLevel());

    // invoke OnLoad() on everything
    Game::GameServer::Instance()->NotifyGameLoad();
}

//------------------------------------------------------------------------------
/**
    Cleanup the game world. This should undo the stuff in SetupWorld().
    Override this method in a subclass if your app needs different 
    behaviour.
*/
void
BaseGameFeatureUnit::CleanupWorld()
{
    // flush world state back into the database
    if (Db::DbServer::Instance()->IsGameDatabaseOpen())
    {
        Game::GameServer::Instance()->NotifyGameSave();
    }

    // cleanup enviornment entity
    this->envEntityManager->ClearEnvEntity();

    // cleanup entities
    this->entityManager->Cleanup();

    // cleanup graphics world
    GraphicsFeature::GraphicsFeatureUnit::Instance()->DiscardDefaultGraphicsWorld();

#if __USE_PHYSICS__
    // cleanup physics world
    if (PhysicsFeature::PhysicsFeatureUnit::HasInstance())
    {
        PhysicsFeature::PhysicsFeatureUnit::Instance()->CleanupPhysicsWorld();
    }
#endif
    // TODO:
    //// shutdown some subsystems
    //Navigation::Server::Instance()->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::OnEndFrame()
{
    FeatureUnit::OnEndFrame();
    if (Input::InputServer::HasInstance())
    {
        this->HandleInput();
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::HandleInput()
{
    Input::InputServer* inputServer = Input::InputServer::Instance();
    if (inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F5))
    {
        // save game
        this->SaveGame("AutoSave");
    }
    if (inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F8))
    {
        // load game
        this->LoadGame("AutoSave");
    }

}

//------------------------------------------------------------------------------
/**
*/
void
BaseGameFeatureUnit::OnFrame()
{
    this->audioServer->OnFrame();
    FeatureUnit::OnFrame();
}

} // namespace Game
