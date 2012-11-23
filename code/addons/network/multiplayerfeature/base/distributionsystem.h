#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::DistributionSystem

    This base class holds all shared data in static arrays. The data is shared 
    between the distribution host and distribution client.

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/point.h"
#include "internalmultiplayer/netstream.h"
#include "internalmultiplayer/bitreader.h"
#include "multiplayer/player.h"
#include "util/queue.h"
#include "multiplayerfeature/objectview.h"

//------------------------------------------------------------------------------
namespace Base
{
class DistributionSystem : public Core::RefCounted
{
    __DeclareClass(DistributionSystem);
    __DeclareSingleton(DistributionSystem);

public:
    /// constructor
    DistributionSystem();
    /// destructor
    virtual ~DistributionSystem();        

    /// open the ui subsystem
    virtual bool Open();
    /// close the ui subsystem
    virtual void Close();
    /// return true if currently open
    virtual bool IsOpen() const;

    /// on begin frame
    virtual void OnBeginFrame();
    /// on frame
    virtual void OnFrame();
    /// on end frame
    virtual void OnEndFrame();

    /// handle data received
    virtual void HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream);

    /// register objectview
    virtual void RegisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov, MultiplayerFeature::ObjectView::ObjectViewId id);
    /// unregister objectview
    virtual void UnregisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov);

    void FreeObjectViewId(MultiplayerFeature::ObjectView::ObjectViewId id );
    /// set snapshot update rate
    void SetSnapShotUpdateRate(Timing::Tick ticks);  
    /// get snapshot update rate
    Timing::Tick GetSnapShotUpdateRate() const;

protected:         

    struct SortedObjectViews
    {
        Util::Array< Ptr<MultiplayerFeature::ObjectView> > reliableObvs;
        Util::Array< Ptr<MultiplayerFeature::ObjectView> > unreliableObvs;
    };  

    /// get all changed object views
    Util::Array< Ptr<MultiplayerFeature::ObjectView> > GetChangedObjectViews();

    /// get new unique for object view
    MultiplayerFeature::ObjectView::ObjectViewId GetNewObjectViewId();

    /// collect entities in area
    SortedObjectViews GatherObjectViewsInArea(const Math::point& position, float distance, bool onlyChanged);

    /// write category and template id from given entity
    void WriteEntityCategoryTemplateIndices(const Ptr<InternalMultiplayer::BitWriter>& writer, const Ptr<Game::Entity>& entity);
    /// get string for indices of category and template
    void ParseEntityCategoryTemplateIndices(const Ptr<InternalMultiplayer::BitReader>& reader, Util::String& outCat, Util::String& outTemplateId);

    //// SHARED STATIC DATA
    static Util::Dictionary<MultiplayerFeature::ObjectView::ObjectViewId, Ptr<MultiplayerFeature::ObjectView> > objectViews;             // dictionary for all registered object views, handle with care cause of multiple mappings
    static Util::Queue<MultiplayerFeature::ObjectView::ObjectViewId> freeObIds; 
    static MultiplayerFeature::ObjectView::ObjectViewId numObjectViewIds;    
                            
    /// check if its time to send world state
    bool IsTimeToUpdate() const;
    mutable Timing::Tick lastSnapshotTime;
    Timing::Tick snapShotInterval;

    // reader for incoming data (declared as member, to avoid creation in each method)
    Ptr<InternalMultiplayer::BitReader> reader;
    bool isOpen;

    uint debugPacketId;
};
//------------------------------------------------------------------------------
/**
*/
inline bool
DistributionSystem::IsOpen() const
{
    return this->isOpen;
}      
    
//------------------------------------------------------------------------------
/**
*/
inline void 
DistributionSystem::SetSnapShotUpdateRate(Timing::Tick ticks)
{
    this->snapShotInterval = ticks;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick 
DistributionSystem::GetSnapShotUpdateRate() const
{
    return this->snapShotInterval;
}
}; // namespace Base
//------------------------------------------------------------------------------