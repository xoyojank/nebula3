#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::DistributionHost

    This is the host part of the distribution system.
    The distribution host sends world snapshots every 100 ms (by default).
    For this it makes all object views pack their CHANGED data (attributes and actions)
    into a reliable ordered OR unreliable sequenced stream and sends it to the players.

    TODO: Select only relevant objectviews for client(must remember all objectviews known by the client)

    (C) 2009 Radon Labs GmbH
*/
#include "network/multiplayerfeature/base/distributionsystem.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class DistributionHost : public Base::DistributionSystem
{
    __DeclareClass(DistributionHost);
    __DeclareSingleton(DistributionHost);

public:
    /// constructor
    DistributionHost();
    /// destructor
    virtual ~DistributionHost();

    /// on begin frame
    virtual void OnBeginFrame();
    /// on end frame
    virtual void OnEndFrame();

    /// handle data received
    virtual void HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream);

    /// create player focus entity who joined a already running session, will register non-local player entities as focus entities
    Ptr<Game::Entity> CreatePlayerEntity(const Ptr<Multiplayer::Player>& player, const Util::String& category, const Util::String& templateId);
    /// destroy given entity will unregister non-local player entities
    void DestroyPlayerEntity(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& entity);    

    /// register objectview
    virtual void RegisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov, MultiplayerFeature::ObjectView::ObjectViewId id);
    /// unregister objectview
    virtual void UnregisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov);

private: 
    /// register game entity as focus entity for given player
    void RegisterClientEntity(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& entity);
    /// unregister game entity as focus entity for given player
    void UnregisterClientEntity(const Ptr<Multiplayer::Player>& player);

    /// set the player actions received from the network/players
    void AddClientActions(const Ptr<InternalMultiplayer::NetStream>& stream);
    /// on receive of player actions
    void DistributeClientActions();

    /// collect all data and prepare for sending
    void CollectSnapshotData(const Ptr<Multiplayer::Player>& client, Ptr<InternalMultiplayer::NetStream>& reliableStream, Ptr<InternalMultiplayer::NetStream>& unreliableStream);

    void FillStreamFromObjectViews(const Util::Array< Ptr<MultiplayerFeature::ObjectView> >& relevantSet, Ptr<InternalMultiplayer::NetStream>& streamToFill);
    /// initiate object view creation
    void InvokeObjectViewCreationOnClients();
    /// initiate object view creation on new player
    void InvokeObjectViewCreationOnClient(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& focusEntity, bool onlyNew);

    /// initiate object view destruction
    void InvokeEntityDestructionOnClients();
    /// initiate object view destruction on new player
    void InvokeEntityDestructionOnClient(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& focusEntity);
                 
    /// collect new entities in area
    Util::Array< Ptr<MultiplayerFeature::ObjectView> > GatherNewObjectViewsInArea(const Math::point& position, float distance);

    /// create netstream for object view creation in client side
    Ptr<InternalMultiplayer::NetStream> BuildObjectViewCreationStream(const Ptr<Game::Entity>& playerEntity, const Util::Array<Ptr<ObjectView> >& relevantSet);

    Util::Dictionary<MultiplayerFeature::ObjectView::ObjectViewId, Ptr<MultiplayerFeature::ObjectView> > newlyCreatedObjectViews;
    Util::Array<Util::Guid> destroyedEntities;

    Util::Dictionary<Ptr<Multiplayer::Player>, Ptr<Game::Entity> > playerFocusEntities;
    Util::Array<Ptr<InternalMultiplayer::NetStream> > pendingPlayerActions;
};
}; // namespace MultiplayerFeature
//------------------------------------------------------------------------------