#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::DistributionClient

    This is the client part of the distribution system.

    The client distribution system sends the player actions of its master object views
    to the host.
    It also distributes all received world snapshots to its slave object views.
    
    TODO: perhaps send also to the other clients for faster turnaround times?!

    (C) 2009 Radon Labs GmbH
*/
#include "network/multiplayerfeature/base/distributionsystem.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class DistributionClient : public Base::DistributionSystem
{
    __DeclareClass(DistributionClient);
    __DeclareSingleton(DistributionClient);

public:
    /// constructor
    DistributionClient();
    /// destructor
    virtual ~DistributionClient();        

    /// on frame
    virtual void OnFrame();

    /// handle data received
    virtual void HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream);
       
private:                                       
    /// set the snapshots for the players received from the network
    void AddSnapshot(const Ptr<InternalMultiplayer::NetStream>& snapShot);

    /// distribute received data to all objects
    void DistributeSnapshots();
    /// distribute one special snapshot
    void DistributeCurrentSnapshot();

    /// collect all input relevant player data
    bool CollectPlayerActions(Ptr<InternalMultiplayer::NetStream>& streamToFill);
    /// send player behaviour data to host
    void SendPlayerActions(const Ptr<InternalMultiplayer::NetStream>& stream);

    /// add create object view stream
    void AddCreateObjectViewStream(const Ptr<InternalMultiplayer::NetStream>& stream);
    /// create entity if not existing and finally create and setup the object view
    void CreateObjectViews();                           
    /// add destroy object view stream
    void AddDestroyEntitiesStream(const Ptr<InternalMultiplayer::NetStream>& stream);
    /// destroy object views and its entities
    void DestroyEntities();

    Util::Array<Ptr<InternalMultiplayer::NetStream> > pendingSnapshots;
    Util::Queue<Ptr<InternalMultiplayer::NetStream> > pendingCreateObjectViews;
    Util::Queue<Ptr<InternalMultiplayer::NetStream> > pendingDestroyEntities;
};
}; // namespace MultiplayerFeature
//------------------------------------------------------------------------------