//------------------------------------------------------------------------------
//  network/multiplayer/distributionclient.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/distributionclient.h"
#include "multiplayerfeature/distributionproperty.h"
#include "multiplayer/multiplayermanager.h"
#include "internalmultiplayer/base/packetprioritybase.h"
#include "basegamefeature/basegameattr/basegameattributes.h"
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/managers/factorymanager.h"
#include "multiplayerfeature/distributionid.h"
#include "distributionprotocol.h"
#include "db/reader.h"
#include "db/dbserver.h"
#include "framesync/framesynctimer.h"
#include "multiplayerfeature/distributionmanager.h"

namespace MultiplayerFeature
{
__ImplementClass(DistributionClient, 'DSCL', Base::DistributionSystem);
__ImplementSingleton(DistributionClient);

using namespace Multiplayer;
using namespace InternalMultiplayer;

//------------------------------------------------------------------------------
/**
*/
DistributionClient::DistributionClient()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DistributionClient::~DistributionClient()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    OnFrame the manager
*/
void
DistributionClient::OnFrame()
{   
    // create new object views and its entities
    if (this->pendingCreateObjectViews.Size() > 0)
    {
        this->CreateObjectViews();
    }
    // distribute incoming snapshots
    if (this->pendingSnapshots.Size() > 0)
    {
        this->DistributeSnapshots();
    }           
    // destory entities
    if (this->pendingDestroyEntities.Size() > 0)
    {
        this->DestroyEntities();
    }

    // and collect player actions, if its time for that
    if (MultiplayerManager::Instance()->GetGameState() == MultiplayerManager::Running)
    {
        Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();    
        if (curTime > this->lastSnapshotTime + this->snapShotInterval)
        {
            this->lastSnapshotTime = curTime;   

            Ptr<InternalMultiplayer::NetStream> stream = InternalMultiplayer::NetStream::Create();
            if (this->CollectPlayerActions(stream))
            {
                this->SendPlayerActions(stream);  
            }                                               
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionClient::DistributeSnapshots()
{   
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    n_printf("%i, : Distributing Snapshot\n", FrameSync::FrameSyncTimer::Instance()->GetTicks());
#endif

    // distribute the collected data here
    // it comes from network; should be set throud setsnapshot(playerindex; data)
    // first get entity id; find entity; get their object view and call unpack data        

    // just do one unreliable
    int newestUnreliableIndex = -1;

    // 1. do this for all pending snapshots
    IndexT index;
    for (index = 0; index < this->pendingSnapshots.Size(); index++)
    {
        // get the oldest one
        const Ptr<InternalMultiplayer::NetStream>& snapshot = this->pendingSnapshots[index];
        this->reader->SetStream(snapshot.cast<IO::Stream>());
        if (this->reader->Open())
        {
            // check if this one is reliable, just do the reliable at first ...
            if (this->reader->IsStreamReliable())
            {
                this->DistributeCurrentSnapshot();
            }
            else
            {
                // ... and find last unreliable
                if (index > newestUnreliableIndex)
                {
                    newestUnreliableIndex = index;
                }
            }
            this->reader->Close();
        }
    }  

    // 2. now do the last unreliable
    if (newestUnreliableIndex >= 0)
    {
        const Ptr<InternalMultiplayer::NetStream>& unreliableSnapshot = this->pendingSnapshots[newestUnreliableIndex];
        this->reader->SetStream(unreliableSnapshot.cast<IO::Stream>());
        if (this->reader->Open())
        {
            this->DistributeCurrentSnapshot();
            this->reader->Close();
        }
    }

    // 3. finally cleanup the pending snapshots
    this->pendingSnapshots.Clear();
}     


//------------------------------------------------------------------------------
/**
    call this method only, if you allready open a netstream snapshot with the
    member reader.
*/
void 
DistributionClient::DistributeCurrentSnapshot()
{
    n_assert(this->reader->IsOpen());

    // just do the reliable in the correct order
    DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
    n_assert(id == DistributionId::WorldSnapShot);    
    // write absolute server time
    Timing::Tick absoluteServerTime = reader->ReadInt();
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    Timing::Tick ping = this->reader->GetRelativeTimeStamp();
    Util::String reliability;
    this->reader->IsStreamReliable() ? reliability = "RELIABLE" : reliability = "UNRELIABLE";
    n_printf("%i, (serverTime: %i, packetTime: %i, packetId: %i) : Distributing RELIABLE Snapshot\n", FrameSync::FrameSyncTimer::Instance()->GetTicks(),
        absoluteServerTime,
        ping,
        this->reader->ReadUInt());
#endif

    // first read number of object views, that have changed
    SizeT numberObjectViews = this->reader->ReadUInt();

    // first jump to end and get all datasizes
    SizeT readIndexBeforeObvs = this->reader->GetReadIndex();
    SizeT sizeInBits = this->reader->GetStream().cast<InternalMultiplayer::NetStream>()->GetSizeInBits();
    SizeT offset = sizeInBits - (numberObjectViews * sizeof(uint) * 8);
    this->reader->SetReadIndex(offset);
    Util::Array<SizeT> obvSizes;
    IndexT i;
    for (i = 0; i < numberObjectViews; ++i)
    {
        obvSizes.Append(this->reader->ReadUInt());                            
    }  
    // reset to read index before data block
    this->reader->SetReadIndex(readIndexBeforeObvs);

    for (i = 0; i < numberObjectViews; ++i)
    {
        // identify object view
        ObjectView::ObjectViewId obvId = this->reader->ReadUInt(); 

#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
        n_printf("          -> distributing ObjectviewID %i\n", obvId); 
#endif
        if  (objectViews.Contains(obvId))        
        {
            // let object view read its attribute values that have changed
            // must be in same order in which host has written them
            objectViews[obvId]->UnpackRelevantData(this->reader, absoluteServerTime); 
        }
        else
        {
            reader->IgnoreBits(obvSizes[i]);   
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            n_printf("          -> Skip objectView: %i bits: %i\n", obvId, obvSizes[i]);
#endif
        }            
    }    
}

//------------------------------------------------------------------------------
/**
*/
bool
DistributionClient::CollectPlayerActions(Ptr<InternalMultiplayer::NetStream>& streamToFill)
{
    Util::Array< Ptr<ObjectView> > changedObvs = this->GetChangedObjectViews();
    if (changedObvs.Size() > 0)
    {
        // go thru all object views and look for any player actions
        Ptr<InternalMultiplayer::BitWriter> writer = InternalMultiplayer::BitWriter::Create();
        writer->SetPacketId(PacketId::NebulaMessage);
        writer->SetStream(streamToFill.cast<IO::Stream>());  
        Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
        writer->SetWriteTimeStamp(curTime);


        if (writer->Open())
        {
            // write delta world snapshot id 
            writer->WriteChar(DistributionId::ClientActions);

            // write absolute client time
            writer->WriteInt(curTime);

        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            writer->WriteUInt(this->debugPacketId);
            n_printf("%i, (packetId: %i) : Collecting player actions\n",  curTime,
                this->debugPacketId++);
        #endif

            // write number of objectviews
            writer->WriteUInt(changedObvs.Size());

            // now do for all               
            Util::Array<SizeT> obvDataSizes;
            IndexT index;
            for (index = 0; index < changedObvs.Size(); index++)
            {
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
                n_printf("          -> adding changed objectViewId %i\n", changedObvs[index]->GetObjectViewId());
#endif
                const Ptr<ObjectView>& curOV = changedObvs[index]; 
                SizeT dataSizeWritten = curOV->PackRelevantData(writer);    
                obvDataSizes.Append(dataSizeWritten);                        
            }
            // write datasizes at the end 
            IndexT i;
            for (i = 0; i < obvDataSizes.Size(); ++i)
            {
                writer->WriteUInt(obvDataSizes[i]);
            }
            writer->Close();
        }
        return true;
    }
    // return empty ptr
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionClient::SendPlayerActions(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    // send all the collected data here to the host, currently reliable and ordered
    MultiplayerManager::Instance()->SendStreamToHost(stream, true, false, true);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::AddSnapshot(const Ptr<InternalMultiplayer::NetStream>& snapShot)
{
    this->pendingSnapshots.Append(snapShot);    
   
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    Timing::Tick absoluteServerTime = reader->ReadInt();
    Timing::Tick ping = this->reader->GetRelativeTimeStamp();
    n_printf("%i, (serverTime: %i, packetTime: %i, packetId: %i) : Snapshot received\n", FrameSync::FrameSyncTimer::Instance()->GetTicks(),
        absoluteServerTime,
        ping,
        this->reader->ReadUInt());
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    // read our distribution id from stream    
    this->reader->SetStream(stream.cast<IO::Stream>());    
    if (this->reader->Open())
    {
        DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
        switch (id)
        {        
        case DistributionId::CreateObjectView:
            // directly create object views            
            this->AddCreateObjectViewStream(stream);
            break;
        case DistributionId::WorldSnapShot:
            // snapshot is not distributed immediately, so we just save the snapshot
            this->AddSnapshot(stream);
            break;
        case DistributionId::DestroyEntities:
            this->AddDestroyEntitiesStream(stream);
            break;
        default:
            n_error("DistributionClient::HandleDataStream: Unkown Distribution Id!");
            break;
        }

        this->reader->Close();
    }    
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::CreateObjectViews()
{
    const Ptr<InternalMultiplayer::NetStream>& stream = this->pendingCreateObjectViews.Dequeue();    
    this->reader->SetStream(stream.cast<IO::Stream>());
    if (this->reader->Open())
    {
        DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
        n_assert(id == DistributionId::CreateObjectView);   
        n_assert(this->reader->IsOpen());

        Timing::Tick absoluteServerTime = reader->ReadInt();
        Timing::Tick ping = reader->GetRelativeTimeStamp();
        Timing::Tick localTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();

#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT    
        n_printf("%i, (serverTime: %i, packetTime: %i, packetId: %i) : Creating objectviews\n", localTime,
                                                                       absoluteServerTime,
                                                                       ping,
                                                                       this->reader->ReadUInt());
#endif
        SizeT numViews = reader->ReadUInt();          
        IndexT idx;
        for (idx = 0; idx < numViews; ++idx)
        {      
            // check if its entity already exists
            Util::Guid entityGUID = reader->ReadGuid();
            bool isMaster = reader->ReadBool();
            Util::String category;
            Util::String templateId;
            this->ParseEntityCategoryTemplateIndices(reader, category, templateId);        
            ObjectView::ObjectViewId obvId = reader->ReadUInt();
            n_assert(!this->objectViews.Contains(obvId));  

            Ptr<Game::Entity> entity = BaseGameFeature::EntityManager::Instance()->GetEntityByAttr(Attr::Attribute(Attr::Guid, entityGUID), false);
            if (!entity.isvalid())    
            {
                // first create entity
                // decide if it has to be a entity with all properties or only distributed ones
                bool onlyRemoteProperties = !isMaster;
                // create entity from category and template id
                entity = BaseGameFeature::FactoryManager::Instance()->CreateEntityByTemplate(category, templateId, onlyRemoteProperties);
                // match guid from host
                entity->SetGuid(Attr::Guid, entityGUID);
                entity->SetBool(Attr::IsMaster, isMaster);
                // attach to entity manager to make active
                BaseGameFeature::EntityManager::Instance()->AttachEntity(entity);
            #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
                n_printf("          -> Create new entity %s \n", entity->GetGuid(Attr::Guid).AsString().AsCharPtr());
            #endif
            }
            // send create obj view to entity, its property will create the view
            Ptr<CreateObjectView> createMsg = CreateObjectView::Create();
            n_assert(obvId != InvalidIndex);
            createMsg->SetObjectViewId(obvId);
            entity->SendSync(createMsg.cast<Messaging::Message>());

        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT       
            n_printf("          -> CreateObjectViews: entity %s, new objectView with id %i\n",
                                        entity->GetGuid(Attr::Guid).AsString().AsCharPtr(),
                                        obvId);
        #endif  

            n_assert(objectViews.Contains(obvId))                    
            // let object view read its attribute values that have changed
            // must be in same order in which host has written them
            objectViews[obvId]->UnpackRelevantData(this->reader, absoluteServerTime);                        
        }
        this->reader->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::DestroyEntities()
{
    const Ptr<InternalMultiplayer::NetStream>& stream = this->pendingDestroyEntities.Dequeue();    
    this->reader->SetStream(stream.cast<IO::Stream>());
    if (this->reader->Open())
    {
        DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
        n_assert(id == DistributionId::DestroyEntities);   
        n_assert(this->reader->IsOpen());

        Timing::Tick absoluteServerTime = reader->ReadInt();
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT    
        Timing::Tick ping = this->reader->GetRelativeTimeStamp();
        n_printf("%i, (serverTime: %i, packetTime: %i, packetId: %i) : Destroing entites\n", FrameSync::FrameSyncTimer::Instance()->GetTicks(),
                                                                                            absoluteServerTime,
                                                                                            ping,
                                                                                            this->reader->ReadUInt());
#endif
        SizeT numEntities = reader->ReadUInt();

        IndexT idx;
        for (idx = 0; idx < numEntities; ++idx)
        { 
            Util::Guid entityGuid = reader->ReadGuid();    
            Ptr<Game::Entity> entity = BaseGameFeature::EntityManager::Instance()->GetEntityByAttr(Attr::Attribute(Attr::Guid, entityGuid), false);
            n_assert(entity.isvalid());
                               
        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            n_printf("          -> DestroyEntity: %s\n", 
                entity->GetGuid(Attr::Guid).AsString().AsCharPtr());
        #endif

            BaseGameFeature::EntityManager::Instance()->RemoveEntity(entity); 
        }   
        this->reader->Close(); 
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::AddCreateObjectViewStream(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    this->pendingCreateObjectViews.Enqueue(stream);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionClient::AddDestroyEntitiesStream(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    this->pendingDestroyEntities.Enqueue(stream);
}
} // namespace MultiplayerFeature

