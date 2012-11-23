//------------------------------------------------------------------------------
//  testnetworkgame/testgamestate.cc
//  (C) 2009 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testnetworkgame/testgamestate.h"
#include "basegamefeature/managers/entitymanager.h"
#include "debugrender/debugrender.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "input/key.h"
#include "multiplayer/multiplayermanager.h"
#include "multiplayerfeature/distributionmanager.h"
#include "internalmultiplayer/multiplayertype.h"
#include "internalmultiplayer/internalmultiplayerinterface.h"      
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/managers/factorymanager.h"
#include "basegamefeature/managers/focusmanager.h"
#include "basegamefeature/basegameattr/basegameattributes.h"

namespace Tools
{
__ImplementClass(Tools::TestGameState, 'TTGS', BaseGameFeature::GameStateHandler);

using namespace Util;
using namespace BaseGameFeature;
using namespace Input;
using namespace Multiplayer;
using namespace MultiplayerFeature;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
TestGameState::TestGameState() 
{
}

//------------------------------------------------------------------------------
/**
*/
TestGameState::~TestGameState()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
TestGameState::OnFrame()
{
    return GameStateHandler::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameState::OnLoadBefore()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameState::OnLoadAfter()
{
    // notify server that we have ended level loading    
    MultiplayerManager::Instance()->StartingFinished();
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameState::SpawnPlayerEntities()
{
    Util::Array<Ptr<Player>> players = MultiplayerManager::Instance()->GetSession()->GetPrivatePlayers();
    players.AppendArray(MultiplayerManager::Instance()->GetSession()->GetPublicPlayers()); 

    Util::String category("Player");
    Util::String templateId("mensch");
    matrix44 spawnPos = matrix44::translation(0,0,4);
    matrix44 rotate45 = matrix44::rotationy(n_deg2rad(45));
        
    IndexT i;
    for (i = 0; i < players.Size(); ++i)
    { 
        const Ptr<DistributionHost>& host = DistributionManager::Instance()->GetDistributionSystem().cast<DistributionHost>();
        Ptr<Game::Entity> entity = host->CreatePlayerEntity(players[i], category, templateId);
        entity->SetMatrix44(Attr::Transform, spawnPos);
        spawnPos = matrix44::multiply(spawnPos, rotate45);
        BaseGameFeature::EntityManager::Instance()->AttachEntity(entity);                   

        playerEntities.Add(players[i], entity);     
    }
}   

//------------------------------------------------------------------------------
/**
*/
void 
TestGameState::SpawnPlayerEntity(const Ptr<Multiplayer::Player>& player)
{
    Util::String category("Player");
    Util::String templateId("mensch");
    matrix44 spawnPos = matrix44::translation(0,0,4);
    matrix44 rotate45 = matrix44::rotationy(n_deg2rad(45));
        
    const Ptr<DistributionHost>& host = DistributionManager::Instance()->GetDistributionSystem().cast<DistributionHost>();
    Ptr<Game::Entity> entity = host->CreatePlayerEntity(player, category, templateId);
    entity->SetMatrix44(Attr::Transform, spawnPos);
    spawnPos = matrix44::multiply(spawnPos, rotate45);
    BaseGameFeature::EntityManager::Instance()->AttachEntity(entity);                   

    playerEntities.Add(player, entity);
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameState::RemovePlayerEntity(const Ptr<Multiplayer::Player>& player)
{
    n_assert(this->playerEntities.Contains(player));
    const Ptr<DistributionHost>& host = DistributionManager::Instance()->GetDistributionSystem().cast<DistributionHost>();
    host->DestroyPlayerEntity(player, this->playerEntities[player]);
    EntityManager::Instance()->RemoveEntity(this->playerEntities[player]);
    this->playerEntities.Erase(player);
}
} // namespace Application