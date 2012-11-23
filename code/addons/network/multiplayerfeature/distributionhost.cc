//------------------------------------------------------------------------------
//  network/multiplayer/distributionhost.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/distributionhost.h"
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
#include "multiplayerfeature/distributionmanager.h"
#include "framesync/framesynctimer.h"

namespace MultiplayerFeature
{
__ImplementClass(DistributionHost, 'DSHO', Base::DistributionSystem);
__ImplementSingleton(DistributionHost);

using namespace Multiplayer;
using namespace InternalMultiplayer;

//------------------------------------------------------------------------------
/**
*/
DistributionHost::DistributionHost()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DistributionHost::~DistributionHost()
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
void
DistributionHost::OnBeginFrame()
{    
    // clear all master events from last frame
    IndexT i;
    for (i = 0; i < DistributionSystem::objectViews.Size(); ++i)
    {
        DistributionSystem::objectViews.ValueAtIndex(i)->ClearMasterEvents();
    }

    // distribute any client player actions
    if (pendingPlayerActions.Size() > 0)
    {
        this->DistributeClientActions();
    }                                                      
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionHost::OnEndFrame()
{       
    if (this->IsTimeToUpdate())
    {
        // initiate any newly object view creation on clients immediately
        if (newlyCreatedObjectViews.Size() > 0)
        {
            this->InvokeObjectViewCreationOnClients();
        }
        // only send world snapshots if game is in running state and anyone has connected
        if (MultiplayerManager::Instance()->GetGameState() == MultiplayerManager::Running
            && MultiplayerManager::Instance()->GetSession()->GetNumRemotePlayers() > 0)
        {   
            // collect and send all delta world snapshots for each player 
            IndexT playerIdx;
            for (playerIdx = 0; playerIdx < playerFocusEntities.Size(); ++playerIdx)
            {
                // create streams
                const Ptr<Multiplayer::Player>& player = playerFocusEntities.KeyAtIndex(playerIdx);
                Ptr<InternalMultiplayer::NetStream> reliableOrderedStream = InternalMultiplayer::NetStream::Create();
                reliableOrderedStream->SetReliable(true);
                Ptr<InternalMultiplayer::NetStream> unreliableSequencedStream = InternalMultiplayer::NetStream::Create();
                unreliableSequencedStream->SetReliable(false);

                // collect data
                this->CollectSnapshotData(player, reliableOrderedStream, unreliableSequencedStream);
                
                // send snapshots to player, reliable ordered and unreliable sequenced
                if (reliableOrderedStream.isvalid() && reliableOrderedStream->GetSizeInBits() > 0)
                {
                    // Be aware: Specification for snapshot data: reliable stream are always send ordered
                    // so all packets arrive in the right order (the most overhead and latency) !!!!!!!!!!!
                    Multiplayer::MultiplayerManager::Instance()->SendStreamToPlayer(player, reliableOrderedStream, true, false, true);
                }
                if (unreliableSequencedStream.isvalid() && unreliableSequencedStream->GetSizeInBits() > 0)
                {
                    // Be aware: Specification for snapshot data: unreliable stream are always send sequenced
                    // packets could be dropped, older packets will be ignored !!!!!!!!!!!!
                    Multiplayer::MultiplayerManager::Instance()->SendStreamToPlayer(player, unreliableSequencedStream, false, true, false);  
                }                   
            }  
        }   
        // destroy objectviews AFTER any snapshot was send
        if (destroyedEntities.Size() > 0)
        {
            this->InvokeEntityDestructionOnClients();
        }    
    }
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalMultiplayer::NetStream> 
DistributionHost::BuildObjectViewCreationStream(const Ptr<Game::Entity>& playerEntity, const Util::Array<Ptr<ObjectView> >& relevantSet)
{
    // stream and writer
    Ptr<InternalMultiplayer::NetStream> creationStream = InternalMultiplayer::NetStream::Create();
    Ptr<InternalMultiplayer::BitWriter> writer = InternalMultiplayer::BitWriter::Create();
    writer->SetPacketId(PacketId::NebulaMessage);
    writer->SetStream(creationStream.cast<IO::Stream>());    
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    writer->SetWriteTimeStamp(curTime);

    if (writer->Open())
    {
        // write id 
        writer->WriteChar(DistributionId::CreateObjectView);
        // write absolute server time
        writer->WriteInt(curTime);
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT      
        // write snapshot id
        writer->WriteUInt(this->debugPacketId++); 
        n_printf("%i, (packetId: %i) : Building object view creation stream\n", curTime,this->debugPacketId-1);
#endif
        // write number of views
        writer->WriteUInt(relevantSet.Size());
        // now do for all       
        Util::Array<SizeT> obvDataSizes; 
        IndexT index;
        for (index = 0; index < relevantSet.Size(); index++)
        {
            const Ptr<ObjectView>& curOV = relevantSet[index]; 
            // first write the GUID of its entity
            const Ptr<Game::Entity>& entity = curOV->GetEntity();
            writer->WriteGuid(entity->GetGuid(Attr::Guid));
            // write bool if entity is master on client
            writer->WriteBool(playerEntity == entity);
            // now write index of category and templateid in case client has to create the entity
            this->WriteEntityCategoryTemplateIndices(writer, entity);
            SizeT dataSizeWritten = curOV->PackRelevantData(writer, true);      
            obvDataSizes.Append(dataSizeWritten); 

        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            // debug
            n_printf("          -> entity %s: new objectView with id %i\n", 
                     entity->GetGuid(Attr::Guid).AsString().AsCharPtr(),
                     curOV->GetObjectViewId());
        #endif
        }     
         
        // write datasizes at the end 
        IndexT i;
        for (i = 0; i < obvDataSizes.Size(); ++i)
        {
            writer->WriteUInt(obvDataSizes[i]);
        }
        writer->Close();
    }
    return creationStream;
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionHost::CollectSnapshotData(const Ptr<Multiplayer::Player>& client, Ptr<InternalMultiplayer::NetStream>& reliableStream, Ptr<InternalMultiplayer::NetStream>& unreliableStream)
{         
    // at first find all relevant object views, and fill relevant set
    const Ptr<Game::Entity>& focusEntity = playerFocusEntities[client];
    // TODO: get network lod distance from msg or somewhere else ????
    Math::point position = focusEntity->GetMatrix44(Attr::Transform).get_position();
    float distance = 100.0;    
    // relevant set for player
    SortedObjectViews relevantSet = this->GatherObjectViewsInArea(position, distance, true);  

#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    if (relevantSet.reliableObvs.Size() > 0
        || relevantSet.unreliableObvs.Size() > 0)
    {
        n_printf("%i : Collecting Snapshot data\n", curTime);
    }
#endif
    
    if (relevantSet.reliableObvs.Size() > 0)
    {
        
    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
        n_printf("          -> Reliable:   ");
    #endif  
        // stream for all reliable obvs
        this->FillStreamFromObjectViews(relevantSet.reliableObvs, reliableStream);
    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
        n_printf("\n");
    #endif  
    }
    if (relevantSet.unreliableObvs.Size() > 0)
    {      
    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT        
        n_printf("          -> Unreliable: ");
    #endif  
        // stream for all unreliable obvs   
        this->FillStreamFromObjectViews(relevantSet.unreliableObvs, unreliableStream);
    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
        n_printf("\n");
    #endif
    }   
}   

//------------------------------------------------------------------------------
/**
    After any new object view was created on server (either on level load or during gameplay)
    the host invokes creation on all clients
*/
void 
DistributionHost::InvokeObjectViewCreationOnClients()
{
    // gather for each client if any new object view is relevant for him
    IndexT i;
    for (i = 0; i < this->playerFocusEntities.Size(); ++i)
    {
        this->InvokeObjectViewCreationOnClient(this->playerFocusEntities.KeyAtIndex(i), this->playerFocusEntities.ValueAtIndex(i), true);
    }    
    newlyCreatedObjectViews.Clear();
}

//------------------------------------------------------------------------------
/**
    Send object view creation stream to client.
    If onlynew is set, only the objectviews in the newlyCreatedObjectViews array will be send, 
    otherwise all objectviews will be send, e.g. for clients who has connected while the game is already running.
*/
void 
DistributionHost::InvokeObjectViewCreationOnClient(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& focusEntity, bool onlyNew)
{
    // at first find all relevant object views, and fill relevant set
    // TODO: get network lod distance from msg or somewhere else ????
    // TODO: save objectview lists per client
    Math::point position = focusEntity->GetMatrix44(Attr::Transform).get_position();
    float distance = 100.0;
    Util::Array< Ptr< ObjectView > > relevantSet;
    if (onlyNew)
    {
        relevantSet = this->GatherNewObjectViewsInArea(position, distance);
    }
    else
    {
        relevantSet = this->GatherObjectViewsInArea(position, distance, false).reliableObvs;
        relevantSet.AppendArray(this->GatherObjectViewsInArea(position, distance, false).unreliableObvs);
    }
    if (relevantSet.Size() > 0)
    {
        Ptr<InternalMultiplayer::NetStream> creationStream = this->BuildObjectViewCreationStream(focusEntity, relevantSet);

        // send stream to player
        Multiplayer::MultiplayerManager::Instance()->SendStreamToPlayer(player, creationStream, true, false, true);
    }                                                                                               
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    // read our distribution id from stream    
    this->reader->SetStream(stream.cast<IO::Stream>());    
    if (this->reader->Open())
    {
        DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
        switch (id)
        {        
        case DistributionId::ClientActions:
            // set handle client actions for later distribution
            this->AddClientActions(stream);
            break;
        default:
            n_error("DistributionHost::HandleDataStream: Unkown Distribution Id!");
            break;
        }

        this->reader->Close();
    }    
}

//------------------------------------------------------------------------------
/**
    create a player focus entity for a player who join a already running session
*/
Ptr<Game::Entity> 
DistributionHost::CreatePlayerEntity(const Ptr<Multiplayer::Player>& player, const Util::String& category, const Util::String& templateId)
{
    const bool onlyDistributedProperties = !player->IsLocal();
    Ptr<Game::Entity> entity = BaseGameFeature::FactoryManager::Instance()->CreateEntityByTemplate(category, templateId, onlyDistributedProperties);
    entity->SetBool(Attr::IsMaster, player->IsLocal());

    // only register the focus entities of clients, for distributing snapshots to them
    if (!player->IsLocal())
    {
        DistributionHost::Instance()->RegisterClientEntity(player, entity);

        // send any previous created object views to new player
        this->InvokeObjectViewCreationOnClient(player, entity, false);
    } 
    return entity;
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::DestroyPlayerEntity(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& entity)
{           
    // only register the focus entities of clients, for distributing snapshots to them
    if (!player->IsLocal())
    {
        DistributionHost::Instance()->UnregisterClientEntity(player);
    }             
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::AddClientActions(const Ptr<InternalMultiplayer::NetStream>& stream)
{
    n_assert(stream.isvalid());

#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    n_printf("%i : Adding client actions\n", FrameSync::FrameSyncTimer::Instance()->GetTicks());
#endif

    this->pendingPlayerActions.Append(stream);
}    

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::DistributeClientActions()
{            
    IndexT streamIdx;
    for (streamIdx = 0; streamIdx < this->pendingPlayerActions.Size(); ++streamIdx)
    {                 
        const Ptr<InternalMultiplayer::NetStream>& actionStream = this->pendingPlayerActions[streamIdx];
        this->reader->SetStream(actionStream.cast<IO::Stream>());
        if (this->reader->Open())
        {
            DistributionId::DistributionIdCode id = (DistributionId::DistributionIdCode)this->reader->ReadChar();
            n_assert(id == DistributionId::ClientActions);  
            Timing::Tick absoluteClientTime = this->reader->ReadInt();

        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
            int packetId = this->reader->ReadUInt();
            n_printf("%i, (clientTime: %i, packetTime: %i, packetId: %i) : Distributing client actions\n",  curTime,
                                                                                            absoluteClientTime,
                                                                                            reader->GetRelativeTimeStamp(),
                                                                                            packetId);
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
                if (objectViews.Contains(obvId))
                {

                #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
                    n_printf("          -> unpacking data for objectViewId %i\n", obvId);
                #endif

                    // let object view read its actions
                    // must be in same order in which host has written them
                    objectViews[obvId]->UnpackRelevantData(this->reader, absoluteClientTime);
                }
                else
                {
                    // obj isn't valid anymore, skip its updates
                    reader->IgnoreBits(obvSizes[i]);

                #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
                    n_printf("          -> ignoring objectViewId %i\n", obvId);
                #endif
                }          
            }   
            reader->Close();
        }
    }
    this->pendingPlayerActions.Clear();                  
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::RegisterClientEntity(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& entity)
{
    n_assert(!this->playerFocusEntities.Contains(player));
    this->playerFocusEntities.Add(player, entity);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::UnregisterClientEntity(const Ptr<Multiplayer::Player>& player)
{
    n_assert(this->playerFocusEntities.Contains(player));
    this->playerFocusEntities.Erase(player);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::RegisterObjectView( const Ptr<MultiplayerFeature::ObjectView>& ov, MultiplayerFeature::ObjectView::ObjectViewId id )
{
    // demand creation on all clients
    this->newlyCreatedObjectViews.Add(id, ov);

    Base::DistributionSystem::RegisterObjectView(ov, id);
} 

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::UnregisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov)
{
    if (InvalidIndex == this->destroyedEntities.FindIndex(ov->GetEntity()->GetGuid(Attr::Guid)))
    {
        this->destroyedEntities.Append(ov->GetEntity()->GetGuid(Attr::Guid));
    }                     

    Base::DistributionSystem::UnregisterObjectView(ov);
}

//------------------------------------------------------------------------------
/**
*/
Util::Array< Ptr<ObjectView> > 
DistributionHost::GatherNewObjectViewsInArea(const Math::point& position, float distance)
{
    Util::Array< Ptr<ObjectView> > array;

    IndexT i;
    for (i = 0; i < this->newlyCreatedObjectViews.Size(); ++i)
    {
        // TODO: for testing distribute all
        array.Append(this->newlyCreatedObjectViews.ValueAtIndex(i));
    }    

    return array;         
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionHost::FillStreamFromObjectViews(const Util::Array< Ptr<MultiplayerFeature::ObjectView> >& relevantSet, Ptr<InternalMultiplayer::NetStream>& streamToFill)
{
    // writer
    Ptr<InternalMultiplayer::BitWriter> writer = InternalMultiplayer::BitWriter::Create();
    writer->SetPacketId(PacketId::NebulaMessage);
    writer->SetStream(streamToFill.cast<IO::Stream>());
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    writer->SetWriteTimeStamp(curTime);    

    if (writer->Open())
    {
        // write delta world snapshot id 
        writer->WriteChar(DistributionId::WorldSnapShot);
        // write absolute server time
        writer->WriteInt(curTime);
    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT      
        // write snapshot id
        writer->WriteUInt(this->debugPacketId++); 
        n_printf("(packetId: %i)", this->debugPacketId-1);
    #endif
        // write number of objectviews
        writer->WriteUInt(relevantSet.Size());

        Util::Array<SizeT> obvDataSizes;
        // now do for all
        IndexT index;

    #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT    
        if (relevantSet.Size() > 0)
        {
            n_printf(", objectViewIds: ");
        }
    #endif
        for (index = 0; index < relevantSet.Size(); index++)
        {
            const Ptr<ObjectView>& curOV = relevantSet[index]; 
            SizeT dataSizeWritten = curOV->PackRelevantData(writer);    
            obvDataSizes.Append(dataSizeWritten);

        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT    
            n_printf("%i, ", curOV->GetObjectViewId());
        #endif
        }
        // write data sizes at the end 
        IndexT i;
        for (i = 0; i < obvDataSizes.Size(); ++i)
        {
            writer->WriteUInt(obvDataSizes[i]);
        }
        writer->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::InvokeEntityDestructionOnClients()
{
    // gather for each client if any new object view is relevant for him
    IndexT i;
    for (i = 0; i < this->playerFocusEntities.Size(); ++i)
    {
        this->InvokeEntityDestructionOnClient(this->playerFocusEntities.KeyAtIndex(i), this->playerFocusEntities.ValueAtIndex(i));
    }    
    destroyedEntities.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionHost::InvokeEntityDestructionOnClient(const Ptr<Multiplayer::Player>& player, const Ptr<Game::Entity>& focusEntity)
{
    // stream and writer
    Ptr<InternalMultiplayer::NetStream> destructionStream = InternalMultiplayer::NetStream::Create();
    Ptr<InternalMultiplayer::BitWriter> writer = InternalMultiplayer::BitWriter::Create();
    writer->SetPacketId(PacketId::NebulaMessage);
    writer->SetStream(destructionStream.cast<IO::Stream>());    
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    writer->SetWriteTimeStamp(curTime);
    
    if (writer->Open())
    {
        // write id 
        writer->WriteChar(DistributionId::DestroyEntities);
        // write absolute server time
        writer->WriteInt(curTime);
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT      
        // write snapshot id
        writer->WriteUInt(this->debugPacketId++); 
        n_printf("%i, (packetId: %i) : Invoking entity destruction on client:\n", curTime, this->debugPacketId-1);
#endif
        // write number of views
        writer->WriteUInt(this->destroyedEntities.Size());
        // now write guids for all        
        IndexT index;
        for (index = 0; index < this->destroyedEntities.Size(); index++)
        {
        #if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
            n_printf("          -> guid: %s\n", this->destroyedEntities[index].AsString().AsCharPtr());
        #endif 
            writer->WriteGuid(this->destroyedEntities[index]);             
        }        
        writer->Close();
    }

    Multiplayer::MultiplayerManager::Instance()->SendStreamToPlayer(player, destructionStream, true, false, true);      
}
} // namespace MultiplayerFeature