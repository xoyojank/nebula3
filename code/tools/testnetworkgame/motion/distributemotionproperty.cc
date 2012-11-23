//------------------------------------------------------------------------------
//  properties/DistributeMotionProperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testnetworkgame/motion/distributemotionproperty.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "basegameprotocol.h"
#include "multiplayerfeature/distributionmanager.h"     
#include "testnetworkgame/motion/motionobjectview.h"
#include "network/distributionprotocol.h"
#include "math/polar.h"

namespace Attr
{
    DefineFloat4(LookDir, 'MVDI', ReadWrite);
    DefineFloat4(MovementPos, 'MVPO', ReadWrite);
}
namespace Tools
{
__ImplementClass(Tools::DistributeMotionProperty, 'DIMO', MultiplayerFeature::DistributionProperty);

using namespace Util;
using namespace Math;
using namespace Game;
using namespace BaseGameFeature;
using namespace MultiplayerFeature;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
DistributeMotionProperty::DistributeMotionProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DistributeMotionProperty::~DistributeMotionProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
DistributeMotionProperty::SetupDefaultAttributes()
{
    this->GetEntity()->AddFloat4(Attr::LookDir);
    this->GetEntity()->AddFloat4(Attr::MovementPos);
    DistributionProperty::SetupDefaultAttributes();
}

//------------------------------------------------------------------------------
/**
*/
void
DistributeMotionProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(UpdateTransform::Id);
    this->RegisterMessage(SetTransform::Id);
    this->RegisterMessage(MoveDirection::Id);
    this->RegisterMessage(MoveStop::Id);
    this->RegisterMessage(MultiplayerFeature::PlayerMoveDirection::Id);
    this->RegisterMessage(MultiplayerFeature::PlayerMoveStop::Id);
    DistributionProperty::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
DistributeMotionProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg != 0);

    DistributionManager::Mode mode = DistributionManager::Instance()->GetCurrentMode();
    if (mode == DistributionManager::Host)
    {         
        // as client remote object
        if (msg->CheckId(UpdateTransform::Id))
        {
            // track transformation
            const Ptr<UpdateTransform>& updateTransform = msg.downcast<UpdateTransform>();
            const Math::matrix44& trans = updateTransform->GetMatrix();
            Math::vector movementDir = trans.get_zaxis();
            this->GetEntity()->SetFloat4(Attr::LookDir, trans.get_zaxis());
            this->GetEntity()->SetFloat4(Attr::MovementPos, trans.get_position());
            this->oldTransform = trans;
        }
        else if (msg->CheckId(SetTransform::Id))
        {
            // track transformation
            const Ptr<SetTransform>& setTransform = msg.downcast<SetTransform>();
            const Math::matrix44& trans = setTransform->GetMatrix();
            this->GetEntity()->SetFloat4(Attr::MovementPos, trans.get_position());//vector::nullvec());
            this->oldTransform = trans;
        }
        else if (msg->CheckId(MultiplayerFeature::PlayerMoveDirection::Id))
        {
            const Ptr<MultiplayerFeature::PlayerMoveDirection>& netMsg = msg.cast<MultiplayerFeature::PlayerMoveDirection>();
            Ptr<MoveDirection> moveMsg = MoveDirection::Create();
            moveMsg->SetDirection(netMsg->GetDirection());
            this->GetEntity()->SendSync(moveMsg.cast<Message>());
        }
        else if (msg->CheckId(MultiplayerFeature::PlayerMoveStop::Id))
        {
            const Ptr<MultiplayerFeature::PlayerMoveStop>& netMsg = msg.cast<MultiplayerFeature::PlayerMoveStop>();
            Ptr<MoveStop> stopMsg = MoveStop::Create();
            this->GetEntity()->SendSync(stopMsg.cast<Message>());
        }
    }
    else
    {
        // as client player
        if (msg->CheckId(MoveDirection::Id))
        {
            // put player action
            if (this->HasObjectView())
            {
                Ptr<MultiplayerFeature::PlayerMoveDirection> netMsg = MultiplayerFeature::PlayerMoveDirection::Create();
                netMsg->SetDirection(msg.cast<MoveDirection>()->GetDirection());
                this->GetObjectView()->PutNetworkEvent(netMsg.cast<Multiplayer::NetworkEventBase>());
            }
        }
        else if (msg->CheckId(MoveStop::Id))
        {
            // put player action
            if (this->HasObjectView())
            {
                Ptr<MultiplayerFeature::PlayerMoveStop> netMsg = MultiplayerFeature::PlayerMoveStop::Create();                
                this->GetObjectView()->PutNetworkEvent(netMsg.cast<Multiplayer::NetworkEventBase>());
            }
        }
    }
    DistributionProperty::HandleMessage(msg);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributeMotionProperty::DoHostOperations()
{
    // compute velocity
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributeMotionProperty::DoClientOperations()
{
    // send settransform      
    Ptr<UpdateTransform> msg = UpdateTransform::Create();
    Math::point oldPos = this->GetEntity()->GetMatrix44(Attr::Transform).get_position();
    Math::point newPos = this->GetEntity()->GetFloat4(Attr::MovementPos);
    matrix44 transform = matrix44::translation(newPos);
    polar dirAngle(this->GetEntity()->GetFloat4(Attr::LookDir));
    matrix44 rotTransform = matrix44::rotationy(dirAngle.rho);
    transform = matrix44::multiply(rotTransform, transform);
    msg->SetMatrix(transform);     
    this->GetEntity()->SendSync(msg.upcast<Message>());
}     

//------------------------------------------------------------------------------
/**
*/
void 
DistributeMotionProperty::AddMonitoredAttributes()
{
    this->AddMonitoredAttribute(Attr::LookDir, false);
    this->AddMonitoredAttribute(Attr::MovementPos, false);    
}
         
//------------------------------------------------------------------------------
/**
*/
Ptr<MultiplayerFeature::ObjectView> 
DistributeMotionProperty::CreateObjectView()
{
    Ptr<MotionObjectView> obView = MotionObjectView::Create();      
    return obView.cast<ObjectView>();
}

}; // namespace Properties
