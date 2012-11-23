#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::NetworkAttribute

    This attribute container is created by the objectview and represents an 
    attribute of a game entity. It delivers some help for tracking, interpolating
    and predicting the attributes in a network based game
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attrid.h"
#include "util/queue.h"
#include "game/entity.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class NetworkAttribute : public Core::RefCounted
{
    __DeclareClass(NetworkAttribute);
public:
    struct HistoryEntry
    {
        Timing::Tick timeStamp;
        Util::Variant value;
    };

    /// constructor
    NetworkAttribute();
    /// init     
    void Init(const Ptr<Game::Entity>& entity, Attr::AttrId attrId, bool initOnly);
    /// destructor
    virtual ~NetworkAttribute();       

    /// get attribute
    const Attr::AttrId& GetLinkedAttributeId() const;
    /// add new value to history
    void AddNewValue(Timing::Tick timeStamp, const Util::Variant& val);
    /// get history
    Util::Queue<NetworkAttribute::HistoryEntry>& GetHistory();
    /// set value in entity
    void ApplyValue(const Util::Variant& val);

    /// get init only state
    bool IsInitOnly() const;

    /// return true if it has changed
    bool HasChanged() const;

private:
    Ptr<Game::Entity> gameEntity;
    Attr::AttrId linkedAttribute;
    bool initOnly;
    // for monitored attribute (host)
    Util::Variant oldValue;
    // for applying new states of attribute (client)        
    Util::Queue<HistoryEntry> history;
};

//------------------------------------------------------------------------------
/**
*/
inline const Attr::AttrId& 
NetworkAttribute::GetLinkedAttributeId() const
{
    return this->linkedAttribute;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
NetworkAttribute::IsInitOnly() const
{
    return this->initOnly;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
NetworkAttribute::HasChanged() const
{
    n_assert(gameEntity.isvalid());
    Util::Variant curValue = gameEntity->GetAttr(this->linkedAttribute).GetValue();
    return (this->oldValue != curValue);
}

//------------------------------------------------------------------------------
/**
*/
inline Util::Queue<NetworkAttribute::HistoryEntry>& 
NetworkAttribute::GetHistory()
{
    return this->history;
}
}; // namespace MultiplayerFeature
//------------------------------------------------------------------------------