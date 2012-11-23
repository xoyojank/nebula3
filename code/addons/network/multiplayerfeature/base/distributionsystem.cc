//------------------------------------------------------------------------------
//  network/multiplayer/base/distributionsystem.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/base/distributionsystem.h"
#include "multiplayer/multiplayermanager.h"
#include "internalmultiplayer/base/packetprioritybase.h"
#include "internalmultiplayer/internalmultiplayerinterface.h"
#include "basegamefeature/basegameattr/basegameattributes.h"
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/managers/factorymanager.h"
#include "multiplayerfeature/distributionid.h"
#include "distributionprotocol.h"
#include "db/reader.h"
#include "db/dbserver.h"

namespace Base
{
__ImplementClass(DistributionSystem, 'DISY', Core::RefCounted);
__ImplementSingleton(DistributionSystem);

//// SHARED STATIC DATA
Util::Dictionary<MultiplayerFeature::ObjectView::ObjectViewId, Ptr<MultiplayerFeature::ObjectView> > DistributionSystem::objectViews;
Util::Queue<MultiplayerFeature::ObjectView::ObjectViewId> DistributionSystem::freeObIds;
MultiplayerFeature::ObjectView::ObjectViewId DistributionSystem::numObjectViewIds = 0;

using namespace Multiplayer;
using namespace InternalMultiplayer;
using namespace MultiplayerFeature;

//------------------------------------------------------------------------------
/**
*/
DistributionSystem::DistributionSystem() : 
    isOpen(false),
    debugPacketId(0)
{
    snapShotInterval = 100;
    lastSnapshotTime = 0;
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DistributionSystem::~DistributionSystem()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
DistributionSystem::Open()
{
    n_assert(!this->isOpen);
	this->isOpen = true;
    
    // update snapshotrate and network thread rate
    this->SetSnapShotUpdateRate(100);
    // create reader
    this->reader = BitReader::Create();

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionSystem::Close()
{
    n_assert(this->isOpen);

    // remove reader
    this->reader = 0;
  
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionSystem::RegisterObjectView(const Ptr<ObjectView>& ov, ObjectView::ObjectViewId id)
{       
    // if id is invalid, we must get a new id
    if (id == InvalidIndex)
    {
        id = this->GetNewObjectViewId(); 
        n_assert(id != InvalidIndex);
    }                    
    ov->SetObjectViewId(id);  
    objectViews.Add(id, ov);
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionSystem::UnregisterObjectView(const Ptr<ObjectView>& ov)
{
    n_assert(objectViews.Contains(ov->GetObjectViewId()));
    objectViews.Erase(ov->GetObjectViewId());
    ObjectView::ObjectViewId id = ov->GetObjectViewId();
    // a id is free for new objectview
    FreeObjectViewId(id);

}

//------------------------------------------------------------------------------
/**
*/
void
DistributionSystem::OnBeginFrame()
{ 
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
    OnFrame the manager
*/
void
DistributionSystem::OnFrame()
{   
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionSystem::OnEndFrame()
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionSystem::HandleDataStream( const Ptr<InternalMultiplayer::NetStream>& stream )
{
    // implement in subclass
    n_error("DistributionSystem::HandleDataStream() not implemented!\n");
}

//------------------------------------------------------------------------------
/**
*/
DistributionSystem::SortedObjectViews
DistributionSystem::GatherObjectViewsInArea(const Math::point& position, float distance, bool onlyChanged)
{
    DistributionSystem::SortedObjectViews array;

    IndexT obvIdx;
    for (obvIdx = 0; obvIdx < objectViews.Size(); ++obvIdx)
    {
    	const Ptr<ObjectView>& curOV = objectViews.ValueAtIndex(obvIdx);
        if (!onlyChanged || curOV->AnyAttributeChanged() || curOV->HasMasterEvents())
        {
            // TODO: for testing distribute all            
            if (curOV->IsReliableOrdered())
            {
                array.reliableObvs.Append(curOV);
            }
            else
            {
                array.unreliableObvs.Append(curOV);
            }
        }
    }

    return array;
}

//------------------------------------------------------------------------------
/**
*/
ObjectView::ObjectViewId 
DistributionSystem::GetNewObjectViewId()
{
    if (this->numObjectViewIds > 100000 && freeObIds.Size() > 0)
    {
        return freeObIds.Dequeue();
    }
    ObjectView::ObjectViewId newId = this->numObjectViewIds++;
    n_assert(!objectViews.Contains(newId));
    return newId;    
}     

//------------------------------------------------------------------------------
/**
*/
void 
DistributionSystem::FreeObjectViewId(ObjectView::ObjectViewId id)
{
    this->freeObIds.Enqueue(id);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionSystem::WriteEntityCategoryTemplateIndices(const Ptr<InternalMultiplayer::BitWriter>& writer, const Ptr<Game::Entity>& entity)
{
    n_assert(writer.isvalid());
    n_assert(entity.isvalid());

    const Util::String& category = entity->GetCategory();
    const Util::String& entityId = entity->GetString(Attr::Id);

    Ptr<Db::Reader> dbReader = Db::Reader::Create();
    dbReader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    dbReader->SetTableName("_Categories");    
    if (!dbReader->Open())
    {
        n_error("DistributionSystem::WriteEntityCategoryTemplateIndices: failed to load table _Categories in static database");        
    }
    const Ptr<Db::ValueTable>& valueTable = dbReader->GetValueTable();
    IndexT catIndex = valueTable->FindRowIndexByAttr(Attr::Attribute(Attr::CategoryName, category));
    // write index os stream
    writer->WriteUInt(catIndex);
    dbReader->Close();

    Util::String templateTableName("_Template_" + category);
    dbReader->SetTableName(templateTableName);       
    dbReader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    if (!dbReader->Open())
    {
        n_error("DistributionSystem::WriteEntityCategoryTemplateIndices: failed to load table %s in static database", templateTableName.AsCharPtr());        
    }
    const Ptr<Db::ValueTable>& idValueTable = dbReader->GetValueTable();
    IndexT idIndex = idValueTable->FindRowIndexByAttr(Attr::Attribute(Attr::Id, entityId));
    // write index os stream
    writer->WriteUInt(idIndex);
    dbReader->Close();
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionSystem::ParseEntityCategoryTemplateIndices(const Ptr<InternalMultiplayer::BitReader>& reader, Util::String& outCat, Util::String& outTemplateId)
{
    n_assert(reader.isvalid());
    IndexT catIndex = reader->ReadUInt();
    IndexT templateIdIndex = reader->ReadUInt();

    Ptr<Db::Reader> dbReader = Db::Reader::Create();
    dbReader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    dbReader->SetTableName("_Categories");    
    if (!dbReader->Open())
    {
        n_error("DistributionSystem::ParseEntityCategoryTemplateIndices: failed to load table _Categories in static database");        
    }
    dbReader->SetToRow(catIndex);
    outCat = dbReader->GetString(Attr::CategoryName);
    dbReader->Close();

    Util::String templateTableName("_Template_" + outCat);
    dbReader->SetTableName(templateTableName);             
    dbReader->SetDatabase(Db::DbServer::Instance()->GetStaticDatabase());
    if (!dbReader->Open())
    {
        n_error("DistributionSystem::ParseEntityCategoryTemplateIndices: failed to load table %s in static database", templateTableName.AsCharPtr());        
    }
    dbReader->SetToRow(templateIdIndex);
    outTemplateId = dbReader->GetString(Attr::Id);
    dbReader->Close();
}

//------------------------------------------------------------------------------
/**
*/
Util::Array< Ptr<ObjectView> > 
DistributionSystem::GetChangedObjectViews()
{
    Util::Array< Ptr<ObjectView> > changedObvs;
    IndexT obvIdx;
    for (obvIdx = 0; obvIdx < objectViews.Size(); ++obvIdx)
    {
        const Ptr<ObjectView>& curOV = objectViews.ValueAtIndex(obvIdx);
        if (curOV->AnyAttributeChanged() || curOV->HasMasterEvents())
        {
            // TODO: for testing distribute all
            changedObvs.Append(objectViews.ValueAtIndex(obvIdx));
        }
    }
    return changedObvs;
}    

//------------------------------------------------------------------------------
/**
*/
bool 
DistributionSystem::IsTimeToUpdate() const
{      
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks(); 
    if (curTime > this->lastSnapshotTime + this->snapShotInterval)
    {     
        this->lastSnapshotTime = curTime;  
        return true;
    }
    return false;
}

} // namespace Base