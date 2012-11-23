//------------------------------------------------------------------------------
//  network/multiplayerfeature/objectview.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/objectview.h"
#include "multiplayerfeature/distributionmanager.h"
#include "util/bitfield.h"
#include "framesync/framesynctimer.h"

namespace MultiplayerFeature
{
__ImplementClass(ObjectView, 'OBVE', Core::RefCounted);

using namespace Attr;
using namespace IO;
using namespace Util;
using namespace Messaging;
using namespace Core;

//------------------------------------------------------------------------------
/**
*/
ObjectView::ObjectView():
    uniqueId(InvalidIndex),
    priority(InternalMultiplayer::PacketPriority::NormalPriority),
    reliable(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ObjectView::~ObjectView()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::Initialize(ObjectViewId id, Ptr<Game::Entity> entity)
{
    // setup
    this->entity = entity;

    // and register yourself    
    DistributionManager::Instance()->RegisterObjectView(Ptr<ObjectView>(this), id);
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::RegisterAttribute(const Ptr<Game::Entity>& entity, const Attr::AttrId& attrId, bool isInitOnly)
{
    n_assert(this->FindAttribute(attrId) == InvalidIndex);

    // create new attribute
    const Ptr<NetworkAttribute>& newOne = NetworkAttribute::Create();
    newOne->Init(entity, attrId, isInitOnly);
    this->attributes.Append(newOne);
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::UnregisterAttribute(const Attr::AttrId& attrId)
{
    IndexT index = this->FindAttribute(attrId);
    n_assert(index != InvalidIndex);
    this->attributes.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::PutNetworkEvent(const Ptr<Multiplayer::NetworkEventBase>& event)
{
    // save index of event
    this->masterEventsRttiMapping.Add(event->GetRtti(), this->networkEvents.Size());
    this->networkEvents.Append(event);                                                 
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::ClearMasterEvents()
{
    this->networkEvents.Clear();
    this->masterEventsRttiMapping.Clear();
}

//------------------------------------------------------------------------------
/**
*/
SizeT
ObjectView::PackRelevantData(const Ptr<InternalMultiplayer::BitWriter>& writer, bool firstCall /*= false*/)
{
    // write the unique entity id
    uint id = this->GetObjectViewId();
    writer->WriteUInt(id);

    SizeT sizeBefore = writer->GetStream().cast<InternalMultiplayer::NetStream>()->GetSizeInBits();
    // do for all attributes
    IndexT index;
    for (index = 0; index < this->attributes.Size(); index++)
    {
        const Attr::AttrId& attr = this->attributes[index]->GetLinkedAttributeId();
        
        // write flag if attribute has changed or its the first call
        if (firstCall || 
            (!this->attributes[index]->IsInitOnly() && this->attributes[index]->HasChanged()))
        {
            writer->WriteBool(true);
        }
        else
        {
            writer->WriteBool(false);
            continue;
        }

        // now write changed data
        ValueType type = attr.GetValueType();
        switch (type)
        {
        case VoidType:
            n_error("Unsupported value type Void");
            break;
        case IntType:
            writer->WriteInt(this->GetEntity()->GetInt(attr));
            break;
        case FloatType:
            writer->WriteFloat(this->GetEntity()->GetFloat(attr));
            break;
        case BoolType:
            writer->WriteBool(this->GetEntity()->GetBool(attr));
            break;
        case Float4Type:
            writer->WriteFloat4(this->GetEntity()->GetFloat4(attr));
            break;
        case Matrix44Type:
            writer->WriteMatrix44(this->GetEntity()->GetMatrix44(attr));
            break;
        case StringType:
            writer->WriteString(this->GetEntity()->GetString(attr));
            break;
        case BlobType:
            writer->WriteBlob(this->GetEntity()->GetBlob(attr));
            break;
        case GuidType:
            writer->WriteGuid(this->GetEntity()->GetGuid(attr));
            break;
        default:
            n_error("ObjectView::PackRelevantData() -> invalid attribute type found!");
            break;
        }
    }

    // pack networkEvents  
    writer->WriteUInt(this->networkEvents.Size());
    for (index = 0; index < this->networkEvents.Size(); ++index)
    {
        writer->WriteUInt(this->networkEvents[index]->GetClassFourCC().AsUInt());
        this->networkEvents[index]->Encode(writer.cast<IO::BinaryWriter>());
    }         
    // clear all networkEvents
    this->ClearMasterEvents();

    SizeT finalSize = writer->GetStream().cast<InternalMultiplayer::NetStream>()->GetSizeInBits();
    SizeT dataSize = finalSize - sizeBefore;
                
    return dataSize;
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectView::UnpackRelevantData(const Ptr<InternalMultiplayer::BitReader>& reader, Timing::Tick absoluteServerTime)
{    
    Timing::Tick relativeTime = reader->GetRelativeTimeStamp();
    Timing::Tick localTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    Timing::Tick snapShotTimeStamp = localTime - relativeTime;
    IndexT index;
    for (index = 0; index < this->attributes.Size(); index++)
    {
        const Ptr<MultiplayerFeature::NetworkAttribute>& networkAttr = this->attributes[index];
        const Attr::AttrId& attr = networkAttr->GetLinkedAttributeId();
        
        // first check if the current attribute has changed
        if (!reader->ReadBool())
        {
            continue;
        }         
        
        // now read the data, add to history
        switch (attr.GetValueType())
        {
        case VoidType:
            n_error("Unsupported value type Void");
            break;
        case IntType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadInt()));
            break;
        case FloatType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadFloat()));
            break;
        case BoolType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadBool()));
            break;
        case Float4Type:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadFloat4()));
            break;
        case Matrix44Type:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadMatrix44()));
            break;
        case StringType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadString()));
            break;
        case BlobType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadBlob()));
            break;
        case GuidType:
            networkAttr->AddNewValue(snapShotTimeStamp, Variant(reader->ReadGuid()));
            break;
        default:
            n_error("ObjectView::PackRelevantData() . invalid attribute type found!");
            break;
        }

        // compute current value and apply on entity attribute
        this->ComputeCurrentValue(networkAttr);
    }

    // unpack networkEvents  
    SizeT numEvents = reader->ReadUInt();
    for (index = 0; index < numEvents; ++index)
    {
        // read FourCC from stream and build a new message
        FourCC classFourCC = reader->ReadUInt();                
        Message* msg = (Message*) Core::Factory::Instance()->Create(classFourCC);
        Multiplayer::NetworkEventBase* playerAction = (Multiplayer::NetworkEventBase*)msg;
        
        // let message initialize itself from the rest of the stream
        playerAction->Decode(reader.cast<IO::BinaryReader>());
        if (reader->HasRelativeTimeStamp())
        {
            // convert server action absolute time to client absolute time
            Timing::Tick serverActionTime = playerAction->GetTimeStamp();
            Timing::Tick serverDiff = absoluteServerTime - serverActionTime;
            Timing::Tick clientActionTime = snapShotTimeStamp - serverDiff;
            playerAction->SetTimeStamp(clientActionTime);
        }

        // send msg directly to entity
        this->entity->SendSync(playerAction);
    }
}

//------------------------------------------------------------------------------
/**
*/
IndexT
ObjectView::FindAttribute(const Attr::AttrId& attrId) const
{
    IndexT attrIndex;
    for (attrIndex = 0; attrIndex < this->attributes.Size(); attrIndex++)
    {
        if (this->attributes[attrIndex]->GetLinkedAttributeId() == attrId)
        {
            return attrIndex;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ObjectView::AnyAttributeChanged() const
{
    IndexT i;
    for (i = 0; i < this->attributes.Size(); ++i)
    {
    	if (!this->attributes[i]->IsInitOnly() 
            && this->attributes[i]->HasChanged()) return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Overwrite for interpolation, extrapolation etc.
*/
void 
ObjectView::ComputeCurrentValue(const Ptr<MultiplayerFeature::NetworkAttribute>& networkAttr)
{
    // for the simplest case copy over attribute value
    Queue<NetworkAttribute::HistoryEntry>& history = networkAttr->GetHistory();
    Variant lastValue = history.Dequeue().value;

    // now apply the data
    networkAttr->ApplyValue(lastValue);
}

//------------------------------------------------------------------------------
/**
*/
void 
ObjectView::Clear()
{
    this->attributes.Clear();
    this->ClearMasterEvents();
    
    // and unregister yourself    
    DistributionManager::Instance()->UnregisterObjectView(Ptr<ObjectView>(this));
}

//------------------------------------------------------------------------------
/**
*/
void 
ObjectView::RenderDebug()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
ObjectView::OverwriteNetworkEvent(const Ptr<Multiplayer::NetworkEventBase>& event)
{
    Core::Rtti* rtti = event->GetRtti();
    if (this->masterEventsRttiMapping.Contains(rtti))
    {
        IndexT masterEventIdx = this->masterEventsRttiMapping[rtti];
        this->networkEvents.EraseIndex(masterEventIdx);
        this->masterEventsRttiMapping.Erase(rtti);
        // check if any master event left
        if (this->masterEventsRttiMapping.Contains(rtti))
        {
             n_error("there are more than one master event with rtti %s", rtti->GetName().AsCharPtr());
        }
    }
    this->PutNetworkEvent(event);
}
} // namespace MultiplayerFeature
