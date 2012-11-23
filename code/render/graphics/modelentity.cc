//------------------------------------------------------------------------------
//  modelentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/modelentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::ModelEntity, 'MDLE', Graphics::GraphicsEntity);

using namespace Util;
using namespace Shared;
using namespace FrameSync;

//------------------------------------------------------------------------------
/**
*/
ModelEntity::ModelEntity() :
    animDrivenMotionTrackingEnabled(false),
    animEventTrackingEnabled(false),
    animEventTrackingOnlyDominatingClip(false),
    jointTrackingEnabled(false)
{
    this->SetType(GraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
ModelEntity::~ModelEntity()
{
    n_assert(!this->sharedData.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::ConfigureAnimDrivenMotionTracking(bool enabled, const StringAtom& jointName)
{
    this->animDrivenMotionTrackingEnabled = enabled;
    this->animDrivenMotionJointName = jointName;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::ConfigureAnimEventTracking(bool enabled, bool onlyDominatingClip)
{
    this->animEventTrackingEnabled = enabled;
    this->animEventTrackingOnlyDominatingClip = onlyDominatingClip;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::ConfigureCharJointTracking(bool enabled, const Array<StringAtom>& jointNames)
{
    this->jointTrackingEnabled = enabled;
    this->trackedJoints = jointNames;
}

//------------------------------------------------------------------------------
/**
    Setup the server-side ModelEntity.
*/
void
ModelEntity::Setup(const Ptr<Stage>& stage_)
{
    n_assert(this->resId.IsValid());

    // call parent class
    GraphicsEntity::Setup(stage_);

    // setup and send an entity creation message
    Ptr<CreateModelEntity> msg = CreateModelEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetResourceId(this->resId);
    msg->SetRootNodePath(this->rootNodePath);
    msg->SetAnimDrivenMotionEnabled(this->animDrivenMotionTrackingEnabled);
    msg->SetAnimDrivenMotionJointName(this->animDrivenMotionJointName);
    msg->SetAnimEventsEnabled(this->animEventTrackingEnabled);
    msg->SetAnimEventsOnlyDominatingClip(this->animEventTrackingOnlyDominatingClip);
    msg->SetCharJointTrackingEnabled(this->jointTrackingEnabled);
    msg->SetTrackedCharJointNames(this->trackedJoints);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::OnSetupSharedData()
{
    this->sharedData = FrameSyncSharedData::Create();
    this->sharedData->OwnerSetup<ModelEntityShared>();
    // do NOT hand to parent class
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::OnDiscardSharedData()
{
    this->sharedData->OwnerDiscard<ModelEntityShared>();
    this->sharedData = 0;
    // do NOT hand to parent class
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::UpdateModelNodeInstanceShaderVariable(const StringAtom& nodeName, const StringAtom& variableSemantic, const Variant& value)
{
    n_assert(this->IsValid());
    Ptr<UpdModelNodeInstanceShaderVariable> msg = UpdModelNodeInstanceShaderVariable::Create();
    msg->SetModelNodeInstanceName(nodeName);
    msg->SetSemantic(variableSemantic);
    msg->SetValue(value);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::UpdateModelNodeInstanceVisibility(const StringAtom& nodeName, bool visible)
{
    n_assert(this->IsValid());
    Ptr<UpdModelNodeInstanceVisibility> msg = UpdModelNodeInstanceVisibility::Create();
    msg->SetModelNodeInstanceName(nodeName);
    msg->SetVisible(visible);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::AddTrackedCharJoint(const StringAtom& jointName)
{
    n_assert(this->IsValid());
    n_assert(InvalidIndex == this->trackedJoints.FindIndex(jointName));

    this->jointTrackingEnabled = true;
    this->trackedJoints.Append(jointName);

    Ptr<Graphics::AddTrackedCharJoint> msg = Graphics::AddTrackedCharJoint::Create();
    msg->SetJointName(jointName);
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
    NOTE: the method returns 0 if the joint is not valid or has not been found!
*/
const Shared::CharJointInfo*
ModelEntity::GetTrackedCharJointInfo(const Util::StringAtom& jointName) const
{
    // note: we assume that relatively few joints will be tracked
    const Shared::ModelEntityShared& shared = this->sharedData->Owner<Shared::ModelEntityShared>();
    if (this->jointTrackingEnabled && shared.charJointDataValid)
    {
        IndexT i;
        for (i = 0; i < shared.charJoints.Size(); i++)
        {
            if (shared.charJoints[i].GetJointName() == jointName)
            {
                return &(shared.charJoints[i]);
            }
        }
    }
    // not found
    return 0;
}

} // namespace Graphics