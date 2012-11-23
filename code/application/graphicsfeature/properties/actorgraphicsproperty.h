#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::ActorGraphicsProperty
    
    Graphics property for animated characters.
    
    (C) 2008 Radon Labs GmbH
*/
#include "properties/graphicsproperty.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class ActorGraphicsProperty : public GraphicsProperty
{
    __DeclareClass(ActorGraphicsProperty);
public:
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    /// setup graphics entities
    virtual void SetupGraphicsEntities();
};
__RegisterClass(ActorGraphicsProperty);

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
    