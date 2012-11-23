#pragma once
#ifndef TOOLS_TESTGAMEAPPLICATION_H
#define TOOLS_TESTGAMEAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestNetGameApplication
    
    Nebula3 Application layer test app.

    (C) 2006 Radon Labs GmbH
*/
#include "appgame/gameapplication.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "stateobjectfeature/stateobjectfeatureunit.h"
#include "multiplayerfeature/multiplayerfeatureunit.h"

#if __USE_PHYSICS__
#include "physicsfeature/physicsfeatureunit.h"
#endif

//------------------------------------------------------------------------------
namespace Tools
{
class TestNetworkGameApplication : public App::GameApplication
{
    __DeclareSingleton(TestNetworkGameApplication);   
public:
    /// constructor
    TestNetworkGameApplication();
    /// destructor
    virtual ~TestNetworkGameApplication();
    
protected:
    /// setup application state handlers
    virtual void SetupStateHandlers();
    /// setup game features
    virtual void SetupGameFeatures();
    /// cleanup game features
    virtual void CleanupGameFeatures(); 

private:
#if __USE_PHYSICS__
    Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
#endif    
    Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
    Ptr<BaseGameFeature::BaseGameFeatureUnit> baseGameFeature;
    Ptr<MultiplayerFeature::MultiplayerFeatureUnit> multiplayerFeature;
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
