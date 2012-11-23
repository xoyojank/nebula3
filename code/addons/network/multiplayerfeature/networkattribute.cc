//------------------------------------------------------------------------------
//  network/multiplayer/networkattribute.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/networkattribute.h"

namespace MultiplayerFeature
{
__ImplementClass(NetworkAttribute, 'NETA', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
NetworkAttribute::NetworkAttribute()
{
}

//------------------------------------------------------------------------------
/**
*/
void
NetworkAttribute::Init(const Ptr<Game::Entity>& entity, Attr::AttrId attrId, bool initOnly)
{
    this->gameEntity = entity;
    this->linkedAttribute = attrId;
    this->initOnly = initOnly;
    this->oldValue = entity->GetAttr(attrId).GetValue();
}
//------------------------------------------------------------------------------
/**
*/
NetworkAttribute::~NetworkAttribute()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
NetworkAttribute::AddNewValue(Timing::Tick timeStamp, const Util::Variant& val)
{
    HistoryEntry newEntry;
    newEntry.timeStamp = timeStamp;
    newEntry.value = val;
    this->history.Enqueue(newEntry);
}

//------------------------------------------------------------------------------
/**
*/
void 
NetworkAttribute::ApplyValue(const Util::Variant& val)
{
    this->gameEntity->SetAttrValue(this->linkedAttribute, val);
    this->oldValue = val;
}
} // namespace MultiplayerFeature
