#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::DistributionProperty

    Base class for any multiplayer property, a multiplayer property is placed
    between any entity think properties and its executive properties. So the 
    multiplayer property can apply any error correction or interpolation.

    The DistributionProperty can operate in host OR client mode, because of any host migration.
    Furthermore the client code and host code for the same task should be in the same class to 
    keep track of its operations.
  
    (C) 2009 Radon Labs GmbH
*/
#include "game/property.h"
#include "internalmultiplayer/bitreader.h"
#include "multiplayerfeature/objectview.h"

namespace Attr  
{
    DeclareBool(IsMaster, 'ISPE', ReadWrite);
};

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class ObjectView;

class DistributionProperty : public Game::Property
{
	__DeclareClass(DistributionProperty);
public:
    /// constructor
    DistributionProperty();
    /// destructor
    virtual ~DistributionProperty(); 

    /// setup the property's attributes to their default state
    virtual void SetupDefaultAttributes();

    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();

    /// setup callbacks for this property, call by entity in OnActivate()
    virtual void SetupCallbacks();    
    /// called before rendering happens
    virtual void OnRender();     
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);      
    /// on render debug
    virtual void OnRenderDebug();
    
protected:     
    ////////////////////////////////////////////////////////////////////////////// 
    /// overwrite these if needed    
    /// does this property anything if we are the host?
    virtual bool HasMasterOperations() const;
    /// does this property anything if we are the client?
    virtual bool HasSlaveOperations() const;

    /// do any host operations, overwrite in subclass
    virtual void DoMasterOperations();
    /// do any client operations, overwrite in subclass
    virtual void DoSlaveOperations();

    /// add monitored attributes
    virtual void AddMonitoredAttributes();
    /// for create your own object view
    virtual Ptr<MultiplayerFeature::ObjectView> CreateObjectView();
    //////////////////////////////////////////////////////////////////////////////

    /// add one monitor attribute, will be copied to any object view on its creation
    void AddMonitoredAttribute(const Attr::AttrId& attrId, bool initOnly); 

    /// has object view
    bool HasObjectView() const;
    /// has object view
    const Ptr<MultiplayerFeature::ObjectView>& GetObjectView() const;

private:
    /// setup object views, overwrite in subclass for your own object view
    void SetupHostObjectView();
    /// setup object views, overwrite in subclass for your own object view
    bool SetupClientObjectView(ObjectView::ObjectViewId id);       

    /// destroy object views, overwrite in subclass for your own object view cleanup
    void DestroyObjectView();

    struct MonitorAttrEntry
    {
        Attr::AttrId id;
        bool initOnly;
    };
    Util::Array<MonitorAttrEntry> monitoredAttributes;           
    Ptr<MultiplayerFeature::ObjectView> objectView;
};    
__RegisterClass(DistributionProperty);

//------------------------------------------------------------------------------
/**
*/
inline bool 
DistributionProperty::HasMasterOperations() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
DistributionProperty::HasSlaveOperations() const
{
    return false;
} 

//------------------------------------------------------------------------------
/**
*/
inline void 
DistributionProperty::AddMonitoredAttribute(const Attr::AttrId& attrId, bool initOnly)
{
    MonitorAttrEntry newEntry;
    newEntry.id = attrId;
    newEntry.initOnly = initOnly;
    this->monitoredAttributes.Append(newEntry);
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
DistributionProperty::HasObjectView() const
{
    return this->objectView.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<MultiplayerFeature::ObjectView>& 
DistributionProperty::GetObjectView() const
{
    return this->objectView;
}   
}; // namespace Properties
//------------------------------------------------------------------------------
