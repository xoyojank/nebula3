//------------------------------------------------------------------------------
//  loader/levelloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "loader/levelloader.h"
#include "loader/loaderserver.h"
#include "appgame/appconfig.h"
#if __USE_PHYSICS__
#include "physics/physicsserver.h"
#include "physics/level.h"
#endif
#include "graphicsfeature/graphicsfeatureunit.h"
#include "core/factory.h"
#include "addons/db/dbserver.h"
#include "addons/db/reader.h"
#include "basegameattr/basegameattributes.h"
#include "loader/environmentloader.h"
#include "loader/entityloader.h"
//#include "loader/navmaploader.h"
#include "game/gameserver.h"
#include "loader/loaderserver.h"
#include "managers/categorymanager.h"

namespace BaseGameFeature
{

//------------------------------------------------------------------------------
/**
*/
bool
LevelLoader::Load(const Util::String& levelName)
{
    // update progress bar window
    BaseGameFeature::LoaderServer* loaderServer = BaseGameFeature::LoaderServer::Instance();
    //loaderServer->SetProgressText("Query Database...");
    //loaderServer->UpdateProgressDisplay(); 
    
    // query level instance attributes from database
    Ptr<Db::Reader> dbReader = Db::Reader::Create();
    dbReader->SetDatabase(Db::DbServer::Instance()->GetGameDatabase());
    dbReader->SetTableName("_Instance_Levels");
    dbReader->AddFilterAttr(Attr::Attribute(Attr::Id, levelName));
    bool success = dbReader->Open();
    n_assert(success);
    if (dbReader->GetNumRows() == 0)
    {
        n_error("LevelLoader::Load(): level '%s' not found in world database!", levelName.AsCharPtr());
        return false;
    }
    else if (dbReader->GetNumRows() > 1)
    {
        // more then one level of that name, corrupt database?
        n_error("LevelLoader::Load(): more then one level '%s' in world database!", levelName.AsCharPtr());
        return false;
    }
    dbReader->SetToRow(0);

    #if __USE_PHYSICS__
    // setup new physics level
	Ptr<Physics::Level> physicsLevel = Physics::Level::Create();
    Physics::PhysicsServer::Instance()->SetLevel(physicsLevel);
    #endif

    // get the active layers from the level
    Util::Array<Util::String> activeLayers = dbReader->GetString(Attr::_Layers).Tokenize(";");

    // ask CategoryManager to load level entities
    CategoryManager* categoryManager = CategoryManager::Instance();
    categoryManager->LoadInstances(levelName);
    //loaderServer->SetMaxProgressValue(categoryManager->GetNumInstances());
    loaderServer->LoadEntities(activeLayers);

    if (GraphicsFeature::GraphicsFeatureUnit::HasInstance())
    {
        GraphicsFeature::GraphicsFeatureUnit::Instance()->OnEntitiesLoaded();
    } 
    // update progress bar window
    //const Util::String& navMeshFile = dbReader->GetString(Attr::NavMesh);
    //loaderServer->SetProgressText("Loading Navigation Map...");
    //loaderServer->UpdateProgressDisplay();
    //NavMapLoader::Load(levelName, navMeshFile);

    //loaderServer->SetProgressText("Level Loader Done...");
    //loaderServer->UpdateProgressDisplay();
    return true;
}

}; // namespace BaseGameFeature