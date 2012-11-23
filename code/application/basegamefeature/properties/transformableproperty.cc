//------------------------------------------------------------------------------
//  properties/transformableproperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/transformableproperty.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "basegameprotocol.h"

namespace BaseGameFeature
{
__ImplementClass(BaseGameFeature::TransformableProperty, 'TRPR', Game::Property);

using namespace Game;

//------------------------------------------------------------------------------
/**
*/
TransformableProperty::TransformableProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TransformableProperty::~TransformableProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
TransformableProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    SetupAttr(Attr::Transform);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformableProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(UpdateTransform::Id);
    this->RegisterMessage(SetTransform::Id);
    Game::Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
TransformableProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg != 0);
    
    if (msg->CheckId(UpdateTransform::Id))
    {
        // update the transformation of the game entity
        const Ptr<UpdateTransform>& updateTransform = msg.downcast<UpdateTransform>();
        this->GetEntity()->SetMatrix44(Attr::Transform, updateTransform->GetMatrix());
    }
    else if (msg->CheckId(SetTransform::Id))
    {
        // create a update transform msg, to update the transformation
        const Ptr<SetTransform>& setTransform = msg.downcast<SetTransform>();
        Ptr<UpdateTransform> updateTransform = UpdateTransform::Create();
        updateTransform->SetMatrix(setTransform->GetMatrix());
        this->GetEntity()->SendSync(updateTransform.upcast<Messaging::Message>());
    }
    else
    {
        Game::Property::HandleMessage(msg);
    }
}

}; // namespace Properties
