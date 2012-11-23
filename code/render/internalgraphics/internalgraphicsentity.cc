//------------------------------------------------------------------------------
//  internalgraphicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "shared/graphics/graphicsentityshared.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalGraphicsEntity, 'GREN', Core::RefCounted);

using namespace Util;
using namespace Math;
using namespace Messaging;
using namespace Shared;

InternalGraphicsEntity::Id InternalGraphicsEntity::UniqueIdCounter = 0;

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntity::InternalGraphicsEntity() :
    transform(matrix44::identity()),
    globalBoxDirty(true),
    links(NumLinkTypes),
    type(InternalGraphicsEntityType::InvalidType),
    isActive(false),
    isValid(false),
    isVisible(true),
    transformChanged(true),
    removeFlag(false),
    updateFrameIndex(InvalidIndex),
    notifyCullingVisibleFrameIndex(InvalidIndex),
    entityTime(0.0),
    timeFactor(1.0),
    clipStatus(ClipStatus::Invalid)
{
    this->id = ++UniqueIdCounter;
}

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntity::~InternalGraphicsEntity()
{
    n_assert(!this->IsActive());
}

//------------------------------------------------------------------------------
/**
    Activate the entity. This method is called when the entity
    is created an attached to the graphics server. During OnActivate()
    the entity should perform any one-time initializations.
*/
void
InternalGraphicsEntity::OnActivate()
{
    n_assert(!this->IsActive());
    this->isActive = true;
    this->isValid = true;
    this->removeFlag = false;
    InternalGraphicsServer::Instance()->RegisterEntity(this);
    this->OnSetupSharedData();
}

//------------------------------------------------------------------------------
/**
    Deactivate the entity, this method is called when the entity
    is removed from the graphics server. Any initialization done
    in OnActivate() should be undone here.
*/
void
InternalGraphicsEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    n_assert(this->deferredMessages.IsEmpty());
    if (this->IsAttachedToStage())
    {           
        this->stage->RemoveEntity(this);
    }
    n_assert(!this->IsAttachedToStage());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    InternalGraphicsServer::Instance()->UnregisterEntity(this);
    this->OnDiscardSharedData();
    this->isActive = false;
}

//------------------------------------------------------------------------------
/**
    This method is called from OnActivate() to setup the shared data
    object of the entity. The method must call the ClientSetup() method
    on the sharedData object with the same template type as the main-thread
    side entity.
*/
void
InternalGraphicsEntity::OnSetupSharedData()
{
    n_assert(this->sharedData.isvalid());
    this->sharedData->ClientSetup<GraphicsEntityShared>();
}

//------------------------------------------------------------------------------
/**
    Called from OnDeactivate() to discard the shared data object of the
    entity from the client side.
*/
void
InternalGraphicsEntity::OnDiscardSharedData()
{
    n_assert(this->sharedData.isvalid());
    this->sharedData->ClientDiscard<GraphicsEntityShared>();
    this->sharedData = 0;
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
InternalGraphicsEntity::OnResetSharedData()
{
    // only need to update local box, all other members are guaranteed
    // to be updated during the frame
    this->sharedData->Client<GraphicsEntityShared>().localBox = this->localBox;
    this->sharedData->Client<GraphicsEntityShared>().globalBox = this->GetGlobalBoundingBox();
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is attached to a stage. An
    entity may only be attached to one stage at any time, but can be
    attached to different stages during its lifetime. Attachging an entity
    to a stage may be relatively slow because the entity must be inserted
    into the cell hierarchy.
*/
void
InternalGraphicsEntity::OnAttachToStage(const Ptr<InternalStage>& s)
{
    n_assert(this->IsActive());
    n_assert(!this->IsAttachedToStage());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    this->stage = s;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is removed from a stage.
*/
void
InternalGraphicsEntity::OnRemoveFromStage()
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(this->deferredMessages.IsEmpty());
    this->ClearLinks(CameraLink);
    this->ClearLinks(LightLink);
    this->stage = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity 
    changes from invisible to visible state.
*/
void
InternalGraphicsEntity::OnShow()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity
    changes from visible to invisible state.
*/
void
InternalGraphicsEntity::OnHide()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called at the beginning of a frame before culling
    happens on EVERY entity.
*/
void
InternalGraphicsEntity::OnCullBefore(Timing::Time time_, Timing::Time timeFactor_, IndexT frameIndex)
{
    if (this->transformChanged)
    {
        this->transformChanged = false;
        this->stage->NotifyOfEntityTransformChange(this);
    }

    // reset clipstatus to invalid, will be set in OnNotifyCullingVisible
    this->UpdateClipStatus(ClipStatus::Outside);

    // save the given time
    this->UpdateTime(time_, timeFactor_);

    // update the update frame index (to detect multiple calls to OnUpdate()
    this->updateFrameIndex = frameIndex;
}

//------------------------------------------------------------------------------
/**
    This method is called during visibility linking when an observed entity
    is found to be visible by an observer (a camera or a light entity).
    NOTE that this method will be called several times per frame, so it may
    be a good idea to use the graphics server's frame counter to protect
    expensive code from multiple execution.
*/
void
InternalGraphicsEntity::OnNotifyCullingVisible(const Ptr<InternalGraphicsEntity>& observer, IndexT frameIndex)
{
    // reset clipstatus to inside
    this->UpdateClipStatus(ClipStatus::Inside);

    // update the frame index (to detect multiple call to OnNotifyVisible()
    this->notifyCullingVisibleFrameIndex = frameIndex;
}

//------------------------------------------------------------------------------
/**
    This method is called on the entity from InternalView::Render()
    once per frame for every visible entity.
*/
void
InternalGraphicsEntity::OnRenderBefore(IndexT frameIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
    this->globalBoxDirty = true;
    // immediately call transform changed to apply before any attachment is updated
    this->OnTransformChanged();
    this->transformChanged = true;
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the the internalview comes to its Render method. 
    Override this method in a subclass to define the visible objects. Models must
    add the modelinstances here. LightEntities must be added to the LightServer and
    depending on generating shadows to the ShadowServer.
*/
void
InternalGraphicsEntity::OnResolveVisibility()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the transformation matrix has changed
    by a call to SetTransform(). Override this method in a subclass if
    it wants to be informed about changes to the transformation matrix.
*/
void
InternalGraphicsEntity::OnTransformChanged()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to render a debug 
    visualization of itself.
*/
void
InternalGraphicsEntity::OnRenderDebug()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::ClearLinks(LinkType linkType)
{
    this->links[linkType].Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::UpdateGlobalBoundingBox()
{
    n_assert(this->globalBoxDirty);
    this->globalBox = this->localBox;
    this->globalBox.transform(this->transform);
    this->globalBoxDirty = false;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalStage>&
InternalGraphicsEntity::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsEntity::IsAttachedToStage() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
    Compute the clip status between this entity and a bounding box in
    global space. This method must be overwritten in a derived class.
*/
ClipStatus::Type
InternalGraphicsEntity::ComputeClipStatus(const bbox& box)
{
    return ClipStatus::Outside;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsEntity::SetVisible(bool b)
{
    if (b != this->isVisible)
    {
        this->isVisible = b;
        if (this->isVisible)
        {
            this->OnShow();
        }
        else
        {
            this->OnHide();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Message handlers may decide to defer message handling until
    the object has become valid.
*/
void
InternalGraphicsEntity::AddDeferredMessage(const Ptr<Message>& msg)
{
    n_assert(!msg->IsDeferred());
    n_assert(!this->IsValid());
    this->deferredMessages.Append(msg);
    msg->SetDeferred(true);
}

//------------------------------------------------------------------------------
/**
    This method is called once when an object with deferred validation
    (e.g. ModelEntities) become valid (usually after their resources
    have finished loading). Any deferred messages will be processed here.
*/
void
InternalGraphicsEntity::HandleDeferredMessages()
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->deferredMessages.Size(); i++)
    {
        this->HandleMessage(this->deferredMessages[i]);
        this->deferredMessages[i]->SetDeferredHandled(true);
    }
    this->deferredMessages.Clear();
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalGraphicsEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalGraphicsEntity, this, msg);
}

} // namespace InternalGraphics