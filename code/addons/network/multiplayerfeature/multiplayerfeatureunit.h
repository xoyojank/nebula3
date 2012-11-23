#pragma once
//------------------------------------------------------------------------------
/**
    @class Multiplayer::MultiplayerFeatureUnit
    
    The MultiplayerFeatureUnit sets up the multiplayer thread and deliveres the
    stuff to host games, join games and all needed functionality behind the scenes

    Trigger this feature AT first, it is important to recieve the whole network
    data before rendering or physics is done!

	(C) 2009 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "multiplayer/multiplayermanager.h"
#include "internalmultiplayer/internalmultiplayerinterface.h"
#include "multiplayerfeature/distributionmanager.h"
#include "multiplayer/base/multiplayernotificationhandlerbase.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class MultiplayerFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(MultiplayerFeatureUnit);
    __DeclareSingleton(MultiplayerFeatureUnit);   

public:
    /// constructor
    MultiplayerFeatureUnit();
    /// destructor
    virtual ~MultiplayerFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
    
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();
    /// flush all messages
    virtual void OnEndFrame();
    /// handle incoming requests from network
    virtual void OnBeginFrame();

	/// called when game debug visualization is on
	virtual void OnRenderDebug();

    /// set notification handler, used for network msg handling
    void SetNotificationHandlerRtti(Core::Rtti* rtti);

protected:
    // client side stuff
    Ptr<Multiplayer::MultiplayerManager> multiplayerManager;
    Ptr<MultiplayerFeature::DistributionManager> distributionManager;
    Core::Rtti* notificationHandlerRtti;
    Util::Array< Ptr<Base::MultiplayerNotificationHandlerBase> > notificationHandler;

    Ptr<InternalMultiplayer::InternalMultiplayerInterface> internalMpInterface;
};
}; // namespace Multiplayer
//------------------------------------------------------------------------------