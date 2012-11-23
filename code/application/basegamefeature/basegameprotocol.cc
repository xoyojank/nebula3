//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegameprotocol.h"

namespace BaseGameFeature
{
    __ImplementClass(BaseGameFeature::MoveDirection, 'MOVD', Messaging::Message);
    __ImplementMsgId(MoveDirection);
    __ImplementClass(BaseGameFeature::MoveFollow, 'MOVF', Messaging::Message);
    __ImplementMsgId(MoveFollow);
    __ImplementClass(BaseGameFeature::MoveGoto, 'MOVG', Messaging::Message);
    __ImplementMsgId(MoveGoto);
    __ImplementClass(BaseGameFeature::MoveRotate, 'MMRO', Messaging::Message);
    __ImplementMsgId(MoveRotate);
    __ImplementClass(BaseGameFeature::MoveSetVelocity, 'MMSV', Messaging::Message);
    __ImplementMsgId(MoveSetVelocity);
    __ImplementClass(BaseGameFeature::MoveStop, 'MOVS', Messaging::Message);
    __ImplementMsgId(MoveStop);
    __ImplementClass(BaseGameFeature::MoveTurn, 'MOVT', Messaging::Message);
    __ImplementMsgId(MoveTurn);
    __ImplementClass(BaseGameFeature::UpdateTransform, 'UPTR', Messaging::Message);
    __ImplementMsgId(UpdateTransform);
    __ImplementClass(BaseGameFeature::SetTransform, 'MOST', UpdateTransform);
    __ImplementMsgId(SetTransform);
    __ImplementClass(BaseGameFeature::EnableAmbienceBubble, 'EABU', Messaging::Message);
    __ImplementMsgId(EnableAmbienceBubble);
    __ImplementClass(BaseGameFeature::AttributesUpdated, 'ATUP', Messaging::Message);
    __ImplementMsgId(AttributesUpdated);
} // BaseGameFeature

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
