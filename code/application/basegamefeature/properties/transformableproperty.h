#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::TransformableProperty

    Entites with this property can be transformed.
  
    (C) 2007 Radon Labs GmbH
*/
#include "game/property.h"
#include "basegameattr/basegameattributes.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class TransformableProperty : public Game::Property
{
	__DeclareClass(TransformableProperty);
public:
    /// constructor
    TransformableProperty();
    /// destructor
    virtual ~TransformableProperty();
    
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();

    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
};
__RegisterClass(TransformableProperty);
}; // namespace Properties
//------------------------------------------------------------------------------
