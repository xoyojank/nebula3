//------------------------------------------------------------------------------
//  actorgraphicsproperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/actorgraphicsproperty.h"
#include "graphics/graphicsprotocol.h"
#include "graphicsfeatureunit.h"

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::ActorGraphicsProperty, 'AGPR', GraphicsFeature::GraphicsProperty);

using namespace Util;
using namespace Graphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupDefaultAttributes()
{
    GraphicsProperty::SetupDefaultAttributes();
    SetupAttr(Attr::AnimSet);
    SetupAttr(Attr::CharacterSet);
    SetupAttr(Attr::StartAnimation);
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupAcceptedMessages()
{
    GraphicsProperty::SetupAcceptedMessages();
    this->RegisterMessage(Graphics::AnimPlayClip::Id);
    this->RegisterMessage(Graphics::AnimStopTrack::Id);
    this->RegisterMessage(Graphics::AnimStopAllTracks::Id);
    this->RegisterMessage(Graphics::AnimIsClipPlaying::Id);
    this->RegisterMessage(Graphics::AnimModifyBlendWeight::Id);
    this->RegisterMessage(Graphics::AnimModifyTimeFactor::Id);
    this->RegisterMessage(Graphics::ApplySkinList::Id);
    this->RegisterMessage(Graphics::ShowSkin::Id);
    this->RegisterMessage(Graphics::HideSkin::Id);
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->IsA(GraphicsEntityMessage::RTTI))
    {
        this->graphicsEntities[0]->SendMsg(msg.cast<GraphicsEntityMessage>());
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
ActorGraphicsProperty::SetupGraphicsEntities()
{
    const String& resName = this->GetGraphicsResource();
    const Ptr<Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    const matrix44& worldMatrix = this->entity->GetMatrix44(Attr::Transform);
    const String& skinList = this->entity->GetString(Attr::CharacterSet);

    String resId;
    resId.Format("mdl:%s.n3", resName.AsCharPtr());

    Ptr<ModelEntity> modelEntity = ModelEntity::Create();
    modelEntity->SetResourceId(resId);
    modelEntity->SetTransform(worldMatrix);
    stage->AttachEntity(modelEntity.upcast<GraphicsEntity>());
    
    // set start animation to immediately start a valid animation
    const Util::String& startAnimName = GetEntity()->GetString(Attr::StartAnimation);
    Ptr<Graphics::AnimPlayClip> startAnimMsg = Graphics::AnimPlayClip::Create();
    startAnimMsg->SetClipName(startAnimName);
    startAnimMsg->SetTrackIndex(0);
    if (this->entity->GetBool(Attr::LoopStartAnimation))
    {
        startAnimMsg->SetLoopCount(0.0f);
    }
    else
    {
        startAnimMsg->SetLoopCount(1.0f);
    }
    startAnimMsg->SetFadeInTime(200);
    startAnimMsg->SetFadeOutTime(200);
    modelEntity->SendMsg(startAnimMsg.cast<Graphics::GraphicsEntityMessage>());

    this->graphicsEntities.Append(modelEntity);

    Ptr<Graphics::ApplySkinList> applySkinList = Graphics::ApplySkinList::Create();
    applySkinList->SetSkinList(skinList);
    modelEntity->SendMsg(applySkinList.cast<GraphicsEntityMessage>());
}

} // namespace GraphicsFeature