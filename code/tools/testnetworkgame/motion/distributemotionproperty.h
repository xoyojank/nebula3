#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::DistributeMotionProperty

    Entites with this property can be transformed.
  
    (C) 2007 Radon Labs GmbH
*/
#include "multiplayerfeature/distributionproperty.h"

namespace Attr
{
    DeclareFloat4(LookDir, 'MVDI', ReadWrite);
    DeclareFloat4(MovementPos, 'MVPO', ReadWrite);
}
//------------------------------------------------------------------------------
namespace Tools
{
class DistributeMotionProperty : public MultiplayerFeature::DistributionProperty
{
	__DeclareClass(DistributeMotionProperty);
public:
    /// constructor
    DistributeMotionProperty();
    /// destructor
    virtual ~DistributeMotionProperty();
    
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();

    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:         
    /// for create your own object view
    virtual Ptr<MultiplayerFeature::ObjectView> CreateObjectView();
    /// does this property anything if we are the host?
    virtual bool HasHostOperations() const;
    /// does this property anything if we are the client?
    virtual bool HasClientOperations() const;
    /// do any host operations, overwrite in subclass
    virtual void DoHostOperations();
    /// do any client operations, overwrite in subclass
    virtual void DoClientOperations();     
    /// add monitored attributes
    virtual void AddMonitoredAttributes();

private: 
    Math::matrix44 oldTransform;
};
__RegisterClass(DistributeMotionProperty);

//------------------------------------------------------------------------------
/**
*/
inline bool 
DistributeMotionProperty::HasHostOperations() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
DistributeMotionProperty::HasClientOperations() const
{
    return true;
}
}; // namespace Properties
//------------------------------------------------------------------------------
