//------------------------------------------------------------------------------
//  internalmodelentityhandler.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "internalgraphics/internalmodelentity.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalstage.h"
#include "framesync/framesynctimer.h"
#include "characters/character.h"
#include "characters/characterinstance.h"
#include "models/nodes/statenodeinstance.h"
#include "renderutil/nodelookuputil.h"
#include "animation/animeventserver.h"
#include "animation/playclipjob.h"

using namespace CoreGraphics;
using namespace Graphics;
using namespace InternalGraphics;
using namespace Animation;
using namespace FrameSync;
using namespace Characters;
using namespace Math;
using namespace CoreAnimation;
using namespace Models;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(CreateModelEntity)
{
    // create a new model entity
    Ptr<InternalModelEntity> modelEntity = InternalModelEntity::Create();
    modelEntity->ConfigureAnimDrivenMotionTracking(msg->GetAnimDrivenMotionEnabled(), msg->GetAnimDrivenMotionJointName());
    modelEntity->ConfigureAnimEventTracking(msg->GetAnimEventsEnabled(), msg->GetAnimEventsOnlyDominatingClip());
    modelEntity->ConfigureCharJointTracking(msg->GetCharJointTrackingEnabled(), msg->GetTrackedCharJointNames());
    modelEntity->SetTransform(msg->GetTransform());
    modelEntity->SetVisible(msg->GetVisible());
    modelEntity->SetResourceId(msg->GetResourceId());
    modelEntity->SetRootNodePath(msg->GetRootNodePath());
    modelEntity->SetRootNodeOffsetMatrix(msg->GetRootNodeOffsetMatrix());
    modelEntity->SetSharedData(msg->GetSharedData());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(modelEntity.cast<InternalGraphicsEntity>());

    // set return value
    msg->GetObjectRef()->Validate<InternalModelEntity>(modelEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, SetVisibility)
{
    obj->SetVisible(msg->GetVisible());

    // also need to hide any attachments
    AttachmentServer::Instance()->SetVisibilityOnAttachments(obj, msg->GetVisible());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, UpdModelNodeInstanceVisibility)
{
    // need to defer handling until model is loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        Ptr<ModelInstance> modelInstance = obj->GetModelInstance();
        n_assert(modelInstance.isvalid());    
        Ptr<ModelNodeInstance> nodeInst = modelInstance->LookupNodeInstance(msg->GetModelNodeInstanceName());
        n_assert(nodeInst.isvalid());
        nodeInst->SetVisible(msg->GetVisible(), obj->GetEntityTime());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, UpdModelNodeInstanceShaderVariable)
{
    Ptr<StateNodeInstance> stateNodeInst = RenderUtil::NodeLookupUtil::LookupStateNodeInstance(obj, msg->GetModelNodeInstanceName());
    if (stateNodeInst.isvalid())
    {
        Ptr<CoreGraphics::ShaderVariableInstance> var;
        if (stateNodeInst->HasShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic())))
        {
            var = stateNodeInst->GetShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));        
        }
        else
        {
            var = stateNodeInst->CreateShaderVariableInstance(ShaderVariable::Semantic(msg->GetSemantic()));
        }
        const Util::Variant& value = msg->GetValue();
        var->SetValue(value);   
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, SetAnimatorTime)
{
    Ptr<AnimatorNodeInstance> nodeInst = RenderUtil::NodeLookupUtil::LookupAnimatorNodeInstance(obj, msg->GetNodeInstanceName());
    if (nodeInst.isvalid())
    {
        nodeInst->OverwriteAnimationTime(msg->GetTime());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AnimPlayClip)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());

        // create and enqueue a new play clip job
        Ptr<PlayClipJob> animJob = PlayClipJob::Create();
        animJob->SetClipName(msg->GetClipName());
        animJob->SetTrackIndex(msg->GetTrackIndex());
        animJob->SetLoopCount(msg->GetLoopCount());
        animJob->SetStartTime(msg->GetStartTime());
        animJob->SetFadeInTime(msg->GetFadeInTime());
        animJob->SetFadeOutTime(msg->GetFadeOutTime());
        animJob->SetTimeOffset(msg->GetTimeOffset());
        animJob->SetTimeFactor(msg->GetTimeFactor());
        animJob->SetBlendWeight(msg->GetBlendWeight());
        animJob->SetExclusiveTag(msg->GetExclusiveTag());
        animJob->SetEnqueueMode(msg->GetEnqueueMode());
        obj->GetCharacterInstance()->AnimController().EnqueueAnimJob(animJob.upcast<AnimJob>());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AnimStopTrack)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->GetCharacterInstance()->AnimController().StopTrack(msg->GetTrackIndex(), msg->GetAllowFadeOut());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AnimStopAllTracks)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->GetCharacterInstance()->AnimController().StopAllTracks(msg->GetAllowFadeOut());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AnimModifyBlendWeight)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        const AnimSequencer& sequencer = obj->GetCharacterInstance()->AnimController().AnimSequencer();
        IndexT trackIndex = msg->GetTrackIndex();
        float blendWeight = msg->GetBlendWeight();
        Util::Array<Ptr<AnimJob> > animJobs = sequencer.GetAnimJobsByTrackIndex(trackIndex);
        IndexT i;
        for (i = 0; i < animJobs.Size(); ++i)
        {
        	animJobs[i]->SetBlendWeight(blendWeight);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AnimModifyTimeFactor)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        const AnimSequencer& sequencer = obj->GetCharacterInstance()->AnimController().AnimSequencer();
        IndexT trackIndex = msg->GetTrackIndex();
        float timeFactor = msg->GetTimeFactor();
        Util::Array<Ptr<AnimJob> > animJobs = sequencer.GetAnimJobsByTrackIndex(trackIndex);
        IndexT i;
        for (i = 0; i < animJobs.Size(); ++i)
        {
            animJobs[i]->SetTimeFactor(msg->GetTimeFactor());
        }
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: this method doesn't really fit with the Animation System's
    philosophy...
*/
__Handler(InternalModelEntity, AnimIsClipPlaying)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        const AnimSequencer& sequencer = obj->GetCharacterInstance()->AnimController().AnimSequencer();
        Util::Array<Ptr<AnimJob> > animJobs = sequencer.GetAnimJobsByName(msg->GetClipName());
        msg->SetPlaying(animJobs.Size() > 0);
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, ApplySkinList)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        if (obj->GetCharacter()->SkinLibrary().HasSkinList(msg->GetSkinList()))
        {
            obj->GetCharacterInstance()->SkinSet().ApplySkinList(msg->GetSkinList());
        }
        else
        {
            n_printf("ApplySkinList: invalid skin list '%s' on character '%s'!\n",
                msg->GetSkinList().Value(), obj->GetResourceId().Value());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, ShowSkin)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->GetCharacterInstance()->SkinSet().AddSkin(msg->GetSkin());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, HideSkin)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->GetCharacterInstance()->SkinSet().RemoveSkin(msg->GetSkin());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, SetVariation)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->GetCharacterInstance()->SetVariationSetName(msg->GetVariationName());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, AddTrackedCharJoint)
{
    // character related messages can only be handled once character has loaded
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        n_assert(obj->HasCharacter());
        obj->AddTrackedCharJoint(msg->GetJointName());
    }
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, BindAttachment)
{
    Ptr<InternalGraphicsEntity> attachment = msg->GetEntityToAttachObjectRef()->Ref<InternalGraphicsEntity>();
    AttachmentServer::Instance()->AttachEntity(msg->GetOffset(),
                                               msg->GetClearType(),
                                               msg->GetJoint(),
                                               attachment,
                                               obj,
                                               msg->GetKeepLocal(),
                                               msg->GetRotation(),
                                               true);
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, UnbindAttachment)
{
    Ptr<InternalGraphicsEntity> attachment = msg->GetEntityToAttachObjectRef()->Ref<InternalGraphicsEntity>();
    AttachmentServer::Instance()->DetachEntity(msg->GetClearType(),
                                               msg->GetJoint(),
                                               attachment,
                                               obj);
}


//------------------------------------------------------------------------------
/**
*/
__Handler(InternalModelEntity, SwitchAttachmentToNewJoint)
{
    const Ptr<InternalGraphicsEntity>& attachment = msg->GetEntityToAttachObjectRef()->Ref<InternalGraphicsEntity>();
    AttachmentServer::Instance()->SwitchEntity(msg->GetJoint(),
                                               msg->GetNewJoint(),
                                               obj,
                                               attachment);
}

//------------------------------------------------------------------------------
/**
    Special case, AnimEventMessages must be forwarded to the 
    AnimEventServer, but only when the ModelEntity has loaded
    its resources (the character must be valid).
*/
__Handler(InternalModelEntity, BaseAnimEventMessage)
{
    if (!obj->IsValid())
    {
        obj->AddDeferredMessage(msg.cast<Message>());
    }
    else
    {
        Animation::AnimEventServer::Instance()->HandleMessage(msg.cast<Message>());
    }
}

//------------------------------------------------------------------------------
/**
    Dispatcher method (must be positioned after the handler methods to
    prevent automatic instantiation).
*/
__Dispatcher(InternalModelEntity)
{
    __HandleByRTTI(InternalModelEntity, BaseAnimEventMessage);
    __Handle(InternalModelEntity, SetVisibility);
    __Handle(InternalModelEntity, UpdModelNodeInstanceVisibility);
    __Handle(InternalModelEntity, UpdModelNodeInstanceShaderVariable);
    __Handle(InternalModelEntity, AnimPlayClip);
    __Handle(InternalModelEntity, AnimStopTrack);
    __Handle(InternalModelEntity, AnimStopAllTracks);
    __Handle(InternalModelEntity, AnimIsClipPlaying);
    __Handle(InternalModelEntity, AnimModifyBlendWeight);
    __Handle(InternalModelEntity, AnimModifyTimeFactor);
    __Handle(InternalModelEntity, ApplySkinList);
    __Handle(InternalModelEntity, ShowSkin);
    __Handle(InternalModelEntity, HideSkin);
    __Handle(InternalModelEntity, BindAttachment);
    __Handle(InternalModelEntity, UnbindAttachment);
    __Handle(InternalModelEntity, SwitchAttachmentToNewJoint);
    __Handle(InternalModelEntity, SetAnimatorTime);
    __Handle(InternalModelEntity, AddTrackedCharJoint);
    __Handle(InternalModelEntity, SetVariation);
    __HandleUnknown(InternalGraphicsEntity);
}

} // namespace Messaging
