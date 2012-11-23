//------------------------------------------------------------------------------
//  view.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/view.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"
#include "graphics/cameraentity.h"
#include "internalgraphics/internalview.h"

namespace Graphics
{
__ImplementClass(Graphics::View, 'GVIW', Core::RefCounted);

using namespace Messaging;
using namespace Util;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
View::View() :
    viewClass(&InternalGraphics::InternalView::RTTI),
    isDefaultView(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
View::~View()
{
    n_assert(!this->IsValid());
    n_assert(!this->cameraEntity.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
View::Setup()
{
    n_assert(0 != this->viewClass);
    n_assert(!this->IsValid());
    n_assert(this->name.IsValid());
    n_assert(this->stageName.IsValid());
    n_assert(this->frameShaderName.IsValid());

    // setup the shared object ref
    this->objectRef = ObjectRef::Create();

    // send a CreateGraphicsView message and wait for completion
    Ptr<Graphics::CreateGraphicsView> msg = Graphics::CreateGraphicsView::Create();
    msg->SetViewClass(this->viewClass);
    msg->SetName(this->name);
    msg->SetStageName(this->stageName);
    msg->SetFrameShaderName(this->frameShaderName);
    msg->SetDefaultView(this->isDefaultView);
    msg->SetObjectRef(this->objectRef);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    n_assert(this->objectRef->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
View::Discard()
{
    n_assert(this->IsValid());

    // properly detach camera entity if set
    if (this->cameraEntity.isvalid())
    {
        this->cameraEntity->OnRemoveFromView(this);
        this->cameraEntity = 0;
    }

    // send a DiscardGraphicsView message and wait for completion
    Ptr<Graphics::DiscardGraphicsView> msg = Graphics::DiscardGraphicsView::Create();
    msg->SetObjectRef(this->objectRef);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    this->objectRef = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
View::SetCameraEntity(const Ptr<CameraEntity>& entity)
{
    if (this->cameraEntity.isvalid())
    {
        this->cameraEntity->OnRemoveFromView(this);
        this->cameraEntity = 0;
    }
    if (entity.isvalid())
    {
        this->cameraEntity = entity;
        this->cameraEntity->OnAttachToView(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<CameraEntity>&
View::GetCameraEntity() const
{
    return this->cameraEntity;
}

//------------------------------------------------------------------------------
/**
*/
void
View::SetDefaultView(bool b)
{
    this->isDefaultView = b;

    if(true == this->isDefaultView && this->IsValid())
    {
        // send a SetDefaultGraphicsView message and wait for completion
        Ptr<Graphics::SetDefaultGraphicsView> msg = Graphics::SetDefaultGraphicsView::Create();
        msg->SetObjectRef(this->objectRef);
        GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    }
}

} // namespace Graphics
