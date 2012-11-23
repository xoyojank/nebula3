//------------------------------------------------------------------------------
//  stage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/stage.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsentitytype.h"
#include "graphics/modelentity.h"

namespace Graphics
{
__ImplementClass(Graphics::Stage, 'GSTG', Core::RefCounted);

using namespace Util;
using namespace Messaging;
using namespace InternalGraphics;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
Stage::Stage()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Stage::~Stage()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::Setup(const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& visSystems)
{
    n_assert(!this->IsValid());
    n_assert(this->name.IsValid());

    // create the object ref (handle of the render-thread-side object)
    this->objectRef = ObjectRef::Create();

    // send a CreateGraphicsStage message and wait for completion 
    Ptr<Graphics::CreateGraphicsStage> msg = Graphics::CreateGraphicsStage::Create();
    msg->SetObjectRef(this->objectRef);
    msg->SetName(this->name);
    msg->SetVisibleSystems(visSystems);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    n_assert(this->objectRef->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::Discard()
{
    n_assert(this->IsValid());

    // send a DiscardGraphicsStage message and wait for completion
    Ptr<DiscardGraphicsStage> msg = DiscardGraphicsStage::Create();
    msg->SetObjectRef(this->objectRef);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    this->objectRef = 0;

    this->entities.Clear();
    IndexT i;
    for (i = 0; i < GraphicsEntityType::NumTypes; i++)
    {
        this->entitiesByType[i].Clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::AttachEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(!entity->IsValid());
    n_assert(entity->GetType() < GraphicsEntityType::NumTypes);

    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);
    entity->Setup(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::RemoveEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(entity->IsValid());
    n_assert(entity->GetStage().get() == this);
    n_assert(entity->GetType() < GraphicsEntityType::NumTypes);

    entity->Discard();

    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
    n_assert2(InvalidIndex == this->entities.FindIndex(entity), "GraphicsEntity was attached at stage twice!");
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::OnFrame()
{
    n_assert(this->IsValid());

    // FIXME: empty (still needed?)
}

} // namespace Graphics
