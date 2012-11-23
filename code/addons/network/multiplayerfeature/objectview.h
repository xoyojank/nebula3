#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::ObjectView

    This class is created and initialized by a property, wich knows and handles
    some attributes wich should be watched and shared by the distribution system
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "game/entity.h"
#include "multiplayerfeature/networkattribute.h"
#include "internalmultiplayer/packetpriority.h"
#include "internalmultiplayer/bitreader.h"
#include "internalmultiplayer/bitwriter.h"
#include "multiplayerprotocol.h"

// forward declaration
namespace Base
{
    class DistributionSystem;
}

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class DistributionManager;
class ObjectView : public Core::RefCounted
{
    __DeclareClass(ObjectView);
public:
    /// the unique id for this object view and all its remote object views
    /// is mapped to the GUID of an entity
    typedef /*unsigned*/ int ObjectViewId;
    
    /// constructor
    ObjectView();
    /// destructor
    virtual ~ObjectView();       

    /// initialize 
    void Initialize(ObjectViewId id,
                    Ptr<Game::Entity> entity);

    /// clear, unregister from manager
    void Clear();

    /// get entity
    const Ptr<Game::Entity>& GetEntity() const;
    /// get the objectview id
    ObjectViewId GetObjectViewId() const;

    /// get priority
    Base::PacketPriorityBase::Code GetPriority() const;
    /// set packet priority
    void SetPriority(InternalMultiplayer::PacketPriority::Code prio);
    /// is objectview's data reliable ordered?
    bool IsReliableOrdered() const;
    /// set reliable ordered to true, otherwise objectview's data is send unreliable sequenced
    void SetReliableOrdered(bool reliable);

    /// return true if any of the network attributes has changed
    bool AnyAttributeChanged() const;    
    /// has player networkEvents
    bool HasMasterEvents() const;   

    /// register attribute for monitoring
    void RegisterAttribute(const Ptr<Game::Entity>& entity, const Attr::AttrId& attrId, bool isInitOnly);
    /// unregister attribute
    void UnregisterAttribute(const Attr::AttrId& attrId);

    /// put a master event which should be send to the host
    void PutNetworkEvent(const Ptr<Multiplayer::NetworkEventBase>& event);
    /// get all master event  that might be received through network
    const Util::Array< Ptr<Multiplayer::NetworkEventBase> >& GetNetworkEvents() const;
    /// overwrite a master event which should be send to the host, any previous event will be discarded
    void OverwriteNetworkEvent(const Ptr<Multiplayer::NetworkEventBase>& event);

    /// get attribute (use this if you are host), returns size of written data
    SizeT PackRelevantData(const Ptr<InternalMultiplayer::BitWriter>& writer, bool firstCall = false);
    /// set the attributes (use this if you are the client)
    void UnpackRelevantData(const Ptr<InternalMultiplayer::BitReader>& reader, Timing::Tick absoluteServerTime);

    /// render debug
    virtual void RenderDebug();

protected:
    /// compute and apply attribute, overwrite in subclass if attr should not directly copied over 
    virtual void ComputeCurrentValue(const Ptr<MultiplayerFeature::NetworkAttribute>&  networkAttr);

private:
    friend class Base::DistributionSystem;
    friend class DistributionHost;

    /// set unique id
    void SetObjectViewId(ObjectViewId id);
    /// find a network attribute for a game attribute
    IndexT FindAttribute(const Attr::AttrId& attrId) const;    
    /// clear the input based player networkEvents
    void ClearMasterEvents();

    ObjectViewId uniqueId;
    Ptr<Game::Entity> entity;                                   // pointer to entity wich is observed
    Base::PacketPriorityBase::Code priority;                    // priority of object view content
    bool reliable;
    bool sequenced;
    bool ordered;

    Util::Array<Ptr<NetworkAttribute> > attributes;              // set of network attributes
    Util::Array< Ptr<Multiplayer::NetworkEventBase> > networkEvents;  // list of currently added player networkEvents (will be purged each frame)
    Util::Dictionary<Core::Rtti*, IndexT> masterEventsRttiMapping; 
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Game::Entity>&
ObjectView::GetEntity() const
{
    return this->entity;
}

//------------------------------------------------------------------------------
/**
*/
inline ObjectView::ObjectViewId
ObjectView::GetObjectViewId() const
{
    return this->uniqueId;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::PacketPriorityBase::Code
ObjectView::GetPriority() const
{
    return this->priority;
}         

//------------------------------------------------------------------------------
/**
*/
inline void 
ObjectView::SetPriority(InternalMultiplayer::PacketPriority::Code prio)
{
    this->priority = prio;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ObjectView::SetReliableOrdered(bool reliable)
{
    this->reliable = reliable;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ObjectView::IsReliableOrdered() const
{
    return this->reliable;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array< Ptr< Multiplayer::NetworkEventBase> >&
ObjectView::GetNetworkEvents() const
{
    return this->networkEvents;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ObjectView::HasMasterEvents() const
{
    return this->networkEvents.Size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ObjectView::SetObjectViewId(ObjectViewId id)
{
    this->uniqueId = id;
}
}; // namespace MultiplayerFeature
//------------------------------------------------------------------------------