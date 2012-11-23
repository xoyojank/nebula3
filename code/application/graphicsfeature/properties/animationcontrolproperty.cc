//------------------------------------------------------------------------------
//  AnimationControlProperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/AnimationControlProperty.h"
#include "graphics/graphicsprotocol.h"
#include "graphicsfeatureunit.h"
#include "basegameprotocol.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::AnimationControlProperty, 'ANCT', GraphicsFeature::GraphicsProperty);

using namespace Util;
using namespace Graphics;
using namespace Math;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
void
AnimationControlProperty::SetupDefaultAttributes()
{
    GraphicsProperty::SetupDefaultAttributes();
    SetupAttr(Attr::IdleAnim);
    SetupAttr(Attr::WalkAnim);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimationControlProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(MoveDirection::Id);
    this->RegisterMessage(MoveStop::Id);
    GraphicsProperty::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimationControlProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(MoveDirection::Id))
    {
        this->Walk();
    }
    else if (msg->CheckId(MoveStop::Id))
    {
        this->Stop();
    }
    else
    {
        GraphicsProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
AnimationControlProperty::Walk()
{
    const Util::String& walkAnim = GetEntity()->GetString(Attr::WalkAnim);
    Ptr<Graphics::AnimPlayClip> animMsg = Graphics::AnimPlayClip::Create();
    animMsg->SetClipName(walkAnim);
    animMsg->SetTrackIndex(MovementTrackIndex);
    animMsg->SetLoopCount(0.0f);
    animMsg->SetFadeInTime(200);
    animMsg->SetFadeOutTime(200);
    this->entity->SendSync(animMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void 
AnimationControlProperty::Stop()
{    
    Ptr<Graphics::AnimStopTrack> msg = Graphics::AnimStopTrack::Create();
    msg->SetTrackIndex(MovementTrackIndex);
    this->entity->SendSync(msg.cast<Messaging::Message>());
}

} // namespace GraphicsFeature