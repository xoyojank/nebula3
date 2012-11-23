#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::TestGameState

        
    (C) 2009 RadonLabs GmbH
*/
#include "basegamefeature/statehandlers/gamestatehandler.h"
#include "game/entity.h"
#include "timing/time.h"
#include "math/point.h"
#include "util/dictionary.h"
#include "util/stringatom.h"
#include "multiplayer/player.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestGameState : public BaseGameFeature::GameStateHandler
{
    __DeclareClass(TestGameState);
public:    
    /// constructor
    TestGameState();
    /// destructor
    virtual ~TestGameState();

    /// called each frame as long as state is current, return new state
    virtual Util::String OnFrame();          
    /// called after Db is opened, and before entities are loaded
    virtual void OnLoadBefore();
    /// called after entities are loaded
    virtual void OnLoadAfter();                                       
    /// spawn player entity for joined player
    void SpawnPlayerEntity(const Ptr<Multiplayer::Player>& player);   
    /// remove player entity
    void RemovePlayerEntity(const Ptr<Multiplayer::Player>& player);

    /// only on host
    void SpawnPlayerEntities();
private:

    Util::Dictionary<Ptr<Multiplayer::Player>, Ptr<Game::Entity>> playerEntities;
};
};
//------------------------------------------------------------------------------