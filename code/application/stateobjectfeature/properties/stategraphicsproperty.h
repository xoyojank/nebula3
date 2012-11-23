#ifndef PROPERTIES_STATEGRAPHICSPROPERTY_H
#define PROPERTIES_STATEGRAPHICSPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class StateObjectFeature::StateGraphicsProperty
  
    A graphics property which can switch between different visual states.
    
    (C) 2008 Radon Labs GmbH
*/
#include "properties/graphicsproperty.h"
#include "util/dictionary.h"
#include "application/stateobjectfeature/properties/stateinfo.h"

//------------------------------------------------------------------------------
namespace StateObjectFeature
{
class StateProperty;

class StateGraphicsProperty : public GraphicsFeature::GraphicsProperty
{
    __DeclareClass(StateGraphicsProperty);
public:
    /// constructor
    StateGraphicsProperty();
    /// destructor
    virtual ~StateGraphicsProperty();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// OnActivate
    virtual void OnActivate();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// called when a SwitchActiveState message is received
    void OnSwitchActiveState(const Util::String& stateName);
    /// update graphics entities transforms
    void UpdateTransforms();
    /// load all entities
    void OnLoadResources(const Util::Dictionary<Util::String, StateInfo>& resources);

    Util::String curState;
};
__RegisterClass(StateGraphicsProperty);

} // namespace StateObjectFeature
//------------------------------------------------------------------------------
#endif
