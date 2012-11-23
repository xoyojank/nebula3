//------------------------------------------------------------------------------
//  graphicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsentity.h"
#include "graphics/graphicsinterface.h"
#include "graphics/stage.h"
#include "shared/graphics/graphicsentityshared.h"

namespace Graphics
{
__ImplementClass(Graphics::GraphicsEntity, 'GREP', Core::RefCounted);

using namespace Math;
using namespace Messaging;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::GraphicsEntity() :
    transform(matrix44::identity()),
    type(GraphicsEntityType::InvalidType),
    isVisible(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::~GraphicsEntity()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsEntity::IsValid() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stage>&
GraphicsEntity::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
    Setup the graphics entity. Subclasses must send a specific
    creation message in this method. This method is called from
    StageProxy::AttachEntityProxy().
*/
void
GraphicsEntity::Setup(const Ptr<Stage>& stage_)
{
    n_assert(!this->IsValid());
    n_assert(stage_.isvalid());
    n_assert(!this->objectRef.isvalid());
    this->objectRef = ObjectRef::Create();
    this->stage = stage_;

    // let subclass setup the shared data object
    this->OnSetupSharedData();
}

//------------------------------------------------------------------------------
/**
    Discard the server-side graphics entity. This method is called from
    StageProxy::RemoveEntityProxy(). There's special handling if the server-side
    entity hasn't been created yet, in this case, a pointer to the original create 
    message must be handed over to the render thread.
*/
void
GraphicsEntity::Discard()
{
    n_assert(this->IsValid());
    this->stage = 0;

    // let subclass discard the shared data object
    this->OnDiscardSharedData();

    // create a discard message
    Ptr<DiscardGraphicsEntity> msg = DiscardGraphicsEntity::Create();
    msg->SetObjectRef(this->objectRef);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
    
	// clear the object ref
	this->objectRef = 0;
}

//------------------------------------------------------------------------------
/**
    Setup the shared data object. If subclasses wish to add more members
    to the shared data object, they must subclass from 
    InternalGraphics::InternalGraphicsEntitySharedData, setup a 
    FrameSyncSharedData object with this data type, and NOT CALL the parent
    class method! So the lowest subclass defines the type of the 
    shared data!
*/
void
GraphicsEntity::OnSetupSharedData()
{
    n_assert(!this->sharedData.isvalid());
    this->sharedData = FrameSync::FrameSyncSharedData::Create();
    this->sharedData->OwnerSetup<Shared::GraphicsEntityShared>();
}

//------------------------------------------------------------------------------
/**
    Discard the shared data object. See OnSetupSharedData() for details!
*/
void
GraphicsEntity::OnDiscardSharedData()
{
    n_assert(this->sharedData.isvalid());
    this->sharedData->OwnerDiscard<Shared::GraphicsEntityShared>();
    this->sharedData = 0;
}

//------------------------------------------------------------------------------
/**
    Send a generic GraphicsEntityMessage to the server-side entity.
*/
void
GraphicsEntity::SendMsg(const Ptr<GraphicsEntityMessage>& msg)
{
    n_assert(this->IsValid());
    msg->SetObjectRef(this->objectRef);
    GraphicsInterface::Instance()->SendBatched(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
    This method must be called from the Setup() method of a subclass to
    send off a specific creation message. The message will be stored
    in the proxy to get the entity handle back later when the server-side
    graphics entity has been created.
*/
void
GraphicsEntity::SendCreateMsg(const Ptr<CreateGraphicsEntity>& msg)
{
    n_assert(!this->IsObjectRefValid());
    msg->SetObjectRef(this->objectRef);
    msg->SetSharedData(this->sharedData);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::SetTransform(const matrix44& m)
{
    this->transform = m;
    this->OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
    Called by SetTransform(). This gives subclasses a chance to react
    to changes on the transformation matrix.
*/
void
GraphicsEntity::OnTransformChanged()
{
    // may need to notify server-side entity about transform change
    if (this->IsValid())
    {
        Ptr<Graphics::UpdateTransform> msg = Graphics::UpdateTransform::Create();
        msg->SetTransform(this->transform);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::SetVisible(bool b)
{
    this->isVisible = b;
    if (this->IsValid())
    {
        Ptr<Graphics::SetVisibility> msg = Graphics::SetVisibility::Create();
        msg->SetVisible(b);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

} // namespace Graphics
