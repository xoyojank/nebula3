//------------------------------------------------------------------------------
//  internalmodelentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalmodelentity.h"
#include "models/modelserver.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "characters/characternode.h"
#include "characters/characternodeinstance.h"
#include "characters/characterserver.h"
#include "animation/animeventserver.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalModelEntity, 'IMDE', InternalGraphics::InternalGraphicsEntity);

using namespace Threading;
using namespace CoreGraphics;
using namespace Math;
using namespace Models;
using namespace Resources;
using namespace Characters;
using namespace Animation;
using namespace Messaging;
using namespace Util;
using namespace Shared;

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::InternalModelEntity() :
    updateFrameId(InvalidIndex),
    rootNodeOffsetMatrix(matrix44::identity()),
    rot180Transform(matrix44::identity()),
    nebula2CharacterRotationHack(true),
    animDrivenMotionTrackingEnabled(false),
    animDrivenMotionVector(vector::nullvec()),
    animEventTrackingEnabled(false),
    animEventOnlyDominatingClip(true),
    animEventLastTick(-1),
    jointTrackingEnabled(false)
{
    this->SetType(InternalGraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::~InternalModelEntity()
{
    n_assert(!this->modelInstance.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
InternalModelEntity::ConfigureAnimDrivenMotionTracking(bool enabled, const StringAtom& jointName)
{
    n_assert(!this->IsValid());
    this->animDrivenMotionTrackingEnabled = enabled;
    this->animDrivenMotionJointName = jointName;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalModelEntity::ConfigureAnimEventTracking(bool enabled, bool onlyDominatingClip)
{
    n_assert(!this->IsValid());
    this->animEventTrackingEnabled = enabled;
    this->animEventOnlyDominatingClip = onlyDominatingClip;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalModelEntity::ConfigureCharJointTracking(bool enabled, const Array<StringAtom>& jointNames)
{
    n_assert(!this->IsValid());
    this->jointTrackingEnabled = enabled;
    if (enabled)
    {
        n_assert(jointNames.Size() > 0);
        n_assert(this->trackedJoints.IsEmpty());
        this->trackedJoints.Reserve(jointNames.Size());
        IndexT i;
        for (i = 0; i < jointNames.Size(); i++)
        {
            this->trackedJoints.Add(jointNames[i], InvalidIndex);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InternalModelEntity::AddTrackedCharJoint(const StringAtom& jointName)
{
    n_assert(!this->trackedJoints.Contains(jointName));
    this->trackedJoints.Add(jointName, InvalidIndex);
    this->jointTrackingEnabled = true;
}

//------------------------------------------------------------------------------
/**
    This will initialize the managed model inside the ModelEntity.
*/
void
InternalModelEntity::OnActivate()
{
    n_assert(!this->IsActive());
    n_assert(this->resId.IsValid());
    n_assert(!this->charInst.isvalid());
    n_assert(!this->character.isvalid());
    InternalGraphicsEntity::OnActivate();
    
    // note: we will remain invalid until at least our model has loaded
    this->SetValid(false);
    if (!this->managedModel.isvalid())
    {
        this->managedModel = ModelServer::Instance()->LoadManagedModel(this->resId);
    }  
    this->ValidateModelInstance();

#if NEBULA3_DEBUG
    //n_printf("InternalModelEntity::OnActivate(): %s\n", this->resId.Value());
#endif
}

//------------------------------------------------------------------------------
/**
    Cleanup our managed model, and ModelInstance if it is already 
    initialized.
*/
void
InternalModelEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    n_assert(this->managedModel.isvalid());

    // clear character instance reference (if valid)
    this->charInst = 0;
    this->character = 0;

    // discard our model instance (if exists)
    if (this->modelInstance.isvalid())
    {        
        // hmm... this doesn't look very elegant...
        this->modelInstance->GetModel()->DiscardInstance(this->modelInstance);
        this->modelInstance = 0;
    }

    // discard our managed model
    if (ModelServer::Instance()->HasManagedModel(this->managedModel->GetResourceId()))
    {
        ModelServer::Instance()->DiscardManagedModel(this->managedModel);
    }    
    this->managedModel = 0;

    // up to parent class
    InternalGraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    This method is called from OnActivate() to setup the shared data
    object of the entity. The method must call the ClientSetup() method
    on the sharedData object with the same template type as the main-thread
    side entity.
*/
void
InternalModelEntity::OnSetupSharedData()
{
    if (!this->sharedData.isvalid())
    {
        // this is unusual, but may happen with the attachment system, we are only
        // existing on the render-thread side, and have no proxy on the main thread
        #if NEBULA3_DEBUG
            // n_printf("InternalModelEntity::OnSetupSharedData(): WARNING no shared data object set on ModelEntity(%s)!\n", this->resId.AsString().AsCharPtr());
        #endif

        this->sharedData = FrameSync::FrameSyncSharedData::Create();
    }
    this->sharedData->ClientSetup<ModelEntityShared>();
    // do NOT hand to parent class!
}

//------------------------------------------------------------------------------
/**
    Called from OnDeactivate() to discard the shared data object of the
    entity from the client side.
*/
void
InternalModelEntity::OnDiscardSharedData()
{
    this->sharedData->ClientDiscard<ModelEntityShared>();
    // do NOT hand to parent class!
}

//------------------------------------------------------------------------------
/**
    This method is called once per frame before OnCullBefore() to 
    initialize the shared data object with suitable data (which may be
    overwritten with uptodate-data later in the frame). This is necessary
    because the SharedData object is double buffered, and thus if an update
    if missed for one frame invalid data from the previous frame may "leak"
    into the next frame.
*/
void
InternalModelEntity::OnResetSharedData()
{
    ModelEntityShared& data = this->sharedData->Client<ModelEntityShared>();
    data.animDrivenMotionVector = vector::nullvec();
    data.animEvents.Clear();

    // initialize the tracked joint data with the bind pose joint positions,
    // the actual position will only be updated if the character is currently visible!
    matrix44 globalMatrix = this->rot180Transform;
    data.charJointDataValid = false;
    data.charJoints.Clear();
    if (this->charInst.isvalid() && this->jointTrackingEnabled)
    {
        data.charJointDataValid = true;

        IndexT i;
        for (i = 0; i < this->trackedJoints.Size(); i++)
        {
            // first validate joint indices in trackedJoints array which are
            // not valid yet (new joints may have been added dynamically)
            const StringAtom& jointName = this->trackedJoints.KeyAtIndex(i);
            IndexT jointIndex;
            if (this->trackedJoints.ValueAtIndex(i) == InvalidIndex)
            {
                jointIndex = this->character->Skeleton().GetJointIndexByName(jointName);
                if (InvalidIndex == jointIndex)
                {
                    n_error("InternalModelEntity::HandleTrackedJoints(): invalid joint name '%s'!\n", jointName.Value());
                }
                this->trackedJoints.ValueAtIndex(i) = jointIndex;
            }
            else
            {
                jointIndex = this->trackedJoints.ValueAtIndex(i);
            }

            // update joint matrices to pose matrix
            const matrix44& localPoseMatrix = this->character->Skeleton().GetJoint(jointIndex).GetPoseMatrix();
            matrix44 globalPoseMatrix = matrix44::multiply(localPoseMatrix, this->rot180Transform);
            CharJointInfo jointInfo(jointName, localPoseMatrix, globalPoseMatrix);
            data.charJoints.Append(jointInfo);
        }
    }
    InternalGraphicsEntity::OnResetSharedData();
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the the internalview comes to its Render method. 
    Add light entities to the LightServer or to the ShadowServer.
*/
void
InternalModelEntity::OnResolveVisibility()
{
    n_assert(this->modelInstance.isvalid());
    VisResolver::Instance()->AttachVisibleModelInstancePlayerCamera(this->modelInstance);
}

//------------------------------------------------------------------------------
/**
    In OnTransformChanged() we need to update the transformation
    of our ModelInstance (if it has already been initialised).
*/
void
InternalModelEntity::OnTransformChanged()
{
    if (this->modelInstance.isvalid())
    {
        if (this->HasCharacter() && this->nebula2CharacterRotationHack)
        {
            // AAARGH implement the age old 180 degree hack for Nebula characters
            this->rot180Transform = matrix44::multiply(matrix44::rotationy(n_deg2rad(180.0f)), this->GetTransform());
            this->modelInstance->SetTransform(this->rot180Transform);
        }
        else
        {
            this->modelInstance->SetTransform(this->GetTransform());
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method is called each frame on each entity (visible or not),
    so keep this method cheap!
*/
void
InternalModelEntity::OnCullBefore(Timing::Time time_, Timing::Time timeFactor_, IndexT frameIndex)
{
    // only perform the update once per frame
    if (this->updateFrameId != frameIndex)
    {
        // first check if our model has been loaded already, if yes
        // our model instance must be initialized 
        if (!this->IsValid())
        {
            this->ValidateModelInstance();
        }  
    }

    // important: call parent class!
    InternalGraphicsEntity::OnCullBefore(time_, timeFactor_, frameIndex);
}

//------------------------------------------------------------------------------
/**
    This method is called once (per-view) before the entity is rendered,
    and only for visible entities. Please note that this method
    may be called several times per frame!
*/
void
InternalModelEntity::OnNotifyCullingVisible(const Ptr<InternalGraphicsEntity>& observer, IndexT frameIndex)
{
    // only execute once per frame
    if (this->notifyCullingVisibleFrameIndex != frameIndex)
    {
        if (this->IsVisible())
        {
            // call back our model-instance
            if (this->modelInstance.isvalid())
            {
                this->modelInstance->OnNotifyCullingVisible(frameIndex, this->entityTime);
            }
        }

        // call parent-class
        InternalGraphicsEntity::OnNotifyCullingVisible(observer, frameIndex);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called on the entity from InternalView::Render()
    once per frame for every visible entity.
*/
void
InternalModelEntity::OnRenderBefore(IndexT frameIndex)
{    
    // if our model instance is valid, let it update itself
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->OnRenderBefore(frameIndex, this->entityTime);
    }

    if (this->charInst.isvalid())
    {
        this->charInst->WaitUpdateDone();
        this->HandleCharacterAnimDrivenMotion();
        this->HandleCharacterAnimEvents(this->entityTime);
        this->HandleTrackedJoints();
    }

    InternalGraphicsEntity::OnRenderBefore(frameIndex);
}

//------------------------------------------------------------------------------
/**
    This creates and initializes our ModelInstance if needed. Since Model 
    loading happens asynchronously this may happen at any time after
    the ModelEntity is activated.
*/
void
InternalModelEntity::ValidateModelInstance()
{
    n_assert(!this->IsValid());
    if (!this->modelInstance.isvalid())
    {
        if (this->managedModel->GetState() == Resource::Loaded)
        {
            const Ptr<Model> model = this->managedModel->GetModel();
            n_assert(model->IsLoaded());
            
            // only set the entity to valid if the complete 
            // model node is completely loaded (no placeholders)
            // FIXME: this should be better adjustable...
            // e.g. meshes must be loaded, but textures may
            // still be loading?
            if (model->CheckPendingResources())
            {
                this->SetValid(true);
                
                // setup model and create a new model instance
                this->SetLocalBoundingBox(model->GetBoundingBox());
                // set transform changed, to update global bounding box
                this->transformChanged = true;

                if (this->rootNodePath.IsValid())
                {
                    this->modelInstance = model->CreatePartialInstance(this->rootNodePath, this->rootNodeOffsetMatrix);
                }
                else
                {
                    this->modelInstance = model->CreateInstance();
                }
                this->modelInstance->SetTransform(this->GetTransform());
                this->modelInstance->SetModelEntity(this);
                if (this->IsVisible())
                {
                    this->OnShow();
                }
                else
                {
                    this->OnHide();
                }

                // check if the model instance contains a character
                const Ptr<ModelNodeInstance>& rootNodeInst = this->modelInstance->GetRootNodeInstance();
                if (rootNodeInst->IsA(CharacterNodeInstance::RTTI))
                {
                    // FIXME FIXME FIXME!!! HACK HACK HACK!!!!
                    this->OnTransformChanged();
                        
                    this->charInst = rootNodeInst.downcast<CharacterNodeInstance>()->GetCharacterInstance();
                    this->character = rootNodeInst->GetModelNode().downcast<CharacterNode>()->GetCharacter();

                    // setup anim driven motion if requested
                    if (this->animDrivenMotionTrackingEnabled)
                    {
                        IndexT jointIndex = this->character->Skeleton().GetJointIndexByName(this->animDrivenMotionJointName);
                        this->charInst->AnimController().SetupAnimDrivenMotion(jointIndex);
                    }
                }
                // handle deferred messages
                this->HandleDeferredMessages();
            }
        }
        else if (this->managedModel->GetState() == Resource::Failed)
        {
            n_error("Failed to load model entity '%s'!", this->resId.Value());
        }
    }
}

//------------------------------------------------------------------------------
/**
    Handle character anim event stuff, always call this method, even
    if anim event tracking is not enabled.
*/
void
InternalModelEntity::HandleCharacterAnimEvents(Timing::Time time)
{
    n_assert(this->charInst.isvalid());
    if (this->animEventTrackingEnabled)
    {
        // first frame time exception
        Timing::Tick timeInTicks = Timing::SecondsToTicks(time);
        if (-1 == this->animEventLastTick)
        {
            this->animEventLastTick = timeInTicks;
        }

        // now get animevents from last time till current time
        Timing::Tick startTime = this->animEventLastTick;
        Timing::Tick endTime = timeInTicks;
        Array<AnimEventInfo>& animEventInfos = this->sharedData->Client<ModelEntityShared>().animEvents;
        animEventInfos = this->charInst->AnimController().AnimSequencer().EmitAnimEvents(startTime, endTime, this->animEventOnlyDominatingClip, "");
    
        // handle events in render thread, and publish through shared data
        if (animEventInfos.Size() > 0)
        { 
            // populate entity events with entity id
            IndexT i;
            for (i = 0; i < animEventInfos.Size(); i++)
            {
                animEventInfos[i].SetEntityId(this->GetId());
            }

            // perform render-thread-event handling
            Animation::AnimEventServer::Instance()->HandleAnimEvents(animEventInfos);
        }
        this->animEventLastTick = timeInTicks;
    }
}

//------------------------------------------------------------------------------
/**
    Handle character anim driven motion feedback. This is called once
    per frame for visible characters.
*/
void
InternalModelEntity::HandleCharacterAnimDrivenMotion()
{
    n_assert(this->charInst.isvalid());
    if (this->animDrivenMotionTrackingEnabled)
    {
        n_assert(this->charInst->AnimController().IsAnimDrivenMotionEnabled());
        Math::vector vec = this->charInst->AnimController().GetAnimDrivenMotionVector();
        vec = float4::multiply(vec, float4(this->timeFactor, this->timeFactor, this->timeFactor, 1.0f));
        vec = matrix44::transform(vec, this->GetTransform());
        this->sharedData->Client<ModelEntityShared>().animDrivenMotionVector = vec;
    }
}

//------------------------------------------------------------------------------
/**
    Handle the tracked character joints if joint tracking is enabled.
    NOTE: it's important that this method is called after the character's
    asynchronous update jobs have finished!
*/
void
InternalModelEntity::HandleTrackedJoints()
{
    n_assert(this->charInst.isvalid());
    if (this->jointTrackingEnabled)
    {
        // update tracked joint array in shared data, the array has
        // already been initialised with the pose matrix in OnResetSharedData!
        ModelEntityShared& shared = this->sharedData->Client<ModelEntityShared>();
        n_assert(shared.charJoints.Size() == this->trackedJoints.Size());
        IndexT i;
        for (i = 0; i < this->trackedJoints.Size(); i++)
        {
            IndexT jointIndex = this->trackedJoints.ValueAtIndex(i);
            n_assert(InvalidIndex != jointIndex);
            const matrix44& localMatrix = this->charInst->Skeleton().GetJointMatrix(jointIndex);
            matrix44 globalMatrix = matrix44::multiply(localMatrix, this->rot180Transform);
            shared.charJoints[i].SetLocalMatrix(localMatrix);
            shared.charJoints[i].SetGlobalMatrix(globalMatrix);
        }
        shared.charJointDataValid = true;
    }
}

//------------------------------------------------------------------------------
/**
    Render our debug visualization (the bounding box).
*/
void
InternalModelEntity::OnRenderDebug()
{
    // render bounding box
    float4 color(1.0f, 0.0f, 0.0f, 0.15f);
    //ShapeRenderer::Instance()->AddShape(RenderShape(Thread::GetMyThreadId(), RenderShape::Box, this->GetGlobalBoundingBox().to_matrix44(), color));
    color.set(1.0f, 0.1f, 0.1f, 0.8f);
    ShapeRenderer::Instance()->AddWireFrameBox(this->GetGlobalBoundingBox(), color, Thread::GetMyThreadId());

    // let model instance render its debug stuff
    this->modelInstance->RenderDebug();
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity 
    changes from invisible to visible state.
*/
void
InternalModelEntity::OnShow()
{
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->OnShow(this->entityTime);
    }
    InternalGraphicsEntity::OnShow();
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity
    changes from visible to invisible state.
*/
void
InternalModelEntity::OnHide()
{
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->OnHide(this->entityTime);
    }
    InternalGraphicsEntity::OnHide();
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalModelEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalModelEntity, this, msg);
}

} // namespace InternalGraphics
