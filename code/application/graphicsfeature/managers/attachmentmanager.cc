//------------------------------------------------------------------------------
//  attachmentmanager.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsfeature/managers/attachmentmanager.h"
#include "graphics/graphicsprotocol.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "graphics/graphicsinterface.h"
#include "game/entity.h"
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/basegameattr/basegameattributes.h"
#include "basegamefeature/basegametiming/gametimesource.h"
#include "graphicsfeature/graphicsfeatureunit.h"

namespace GraphicsFeature
{
__ImplementClass(AttachmentManager, 'ATMA', Game::Manager);
__ImplementSingleton(AttachmentManager);

using namespace Util;
using namespace Math;

// FIXME: why does the AttachmentManager know about InternalGraphics???
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
AttachmentManager::AttachmentManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AttachmentManager::~AttachmentManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::OnFrame()
{
    // check temporary attachments
    this->CheckTemporaryAttachments();
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachment( const Util::StringAtom& jointName,
                                          const Ptr<Game::Entity>& baseEntityPtr,
                                          const Util::StringAtom& entityToAttachResId,
                                          const Math::matrix44& offset,
                                          bool keepLocal,
                                          InternalGraphics::AttachmentServer::AttachmentRotation rotation)
{
    this->AddGraphicsAttachmentTemporary( jointName,
                                          baseEntityPtr,
                                          entityToAttachResId,
                                          offset,
                                          keepLocal,
                                          rotation,
                                          -1);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachment( const Util::StringAtom& jointName, 
                                          const Util::StringAtom& baseEntityId, 
                                          const Util::StringAtom& entityToAttachResId, 
                                          const Math::matrix44& offset,
                                          bool keepLocal,
                                          InternalGraphics::AttachmentServer::AttachmentRotation rotation)
{        
    // find base entity
    this->AddGraphicsAttachmentTemporary( jointName,
        baseEntityId,
        entityToAttachResId,
        offset,
        keepLocal,
        rotation,
        -1);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName, 
                                                   const Util::StringAtom& baseEntityId, 
                                                   const Util::StringAtom& entityToAttachResId, 
                                                   const Math::matrix44& offset,
                                                   bool keepLocal,
                                                   InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                                   Timing::Time duration)
{        
    // find base entity
    const Ptr<Game::Entity> base = BaseGameFeature::EntityManager::Instance()->GetEntityByAttr(Attr::Attribute(Attr::Id, baseEntityId.Value()));
    n_assert(base.isvalid());

    this->AddGraphicsAttachmentTemporary( jointName, 
        base, 
        entityToAttachResId, 
        offset, 
        keepLocal, 
        rotation, 
        duration);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName, 
                                                  const Ptr<Game::Entity>& baseEntityPtr, 
                                                  const Util::StringAtom& entityToAttachResId, 
                                                  const Math::matrix44& offset, 
                                                  bool keepLocal, 
                                                  InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                                  Timing::Time duration)
{
    // first attach as pending
    Attachment newAttachment;
    newAttachment.joint = jointName;
    newAttachment.offset = offset;
    newAttachment.keepLocal = keepLocal;
    newAttachment.rotation = rotation;
    newAttachment.duration = duration;

    Ptr<GetGraphicsEntities> getGfx1 = GetGraphicsEntities::Create();
    baseEntityPtr->SendSync(getGfx1.cast<Messaging::Message>());
    newAttachment.baseEntity = getGfx1->GetEntities()[0];

    // transform
    Math::matrix44 t = Math::matrix44::multiply(newAttachment.baseEntity->GetTransform(), offset);

    // create attachment model entity
    Ptr<Graphics::ModelEntity> attachedEntity = Graphics::ModelEntity::Create();
    attachedEntity->SetResourceId(entityToAttachResId);
    attachedEntity->SetTransform(t);
    attachedEntity->SetVisible(true);
    const Ptr<Graphics::Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    stage->AttachEntity(attachedEntity.cast<Graphics::GraphicsEntity>());
    newAttachment.attachedEntity = attachedEntity;
    
    this->attachments.Append(newAttachment);

    // setup and send BindAttachment message
    this->SendAttachmentMessage(newAttachment);
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::CheckTemporaryAttachments()
{
    // get time
    Timing::Time time = BaseGameFeature::GameTimeSource::Instance()->GetTime();

    // check all
    IndexT index;
    for (index = 0; index < this->attachments.Size(); index++)
    {
        const Attachment& attachment = this->attachments[index];

        // skip if endless
        if (-1 == attachment.duration)
        {
            continue;
        }

        // if time ran out
        if (attachment.startTime + attachment.duration < time)
        {
            // remove that shit
            this->SendDetachmentMessage(attachment);

            // check if remove from stage
            GraphicsFeatureUnit::Instance()->GetDefaultStage()->RemoveEntity(attachment.attachedEntity);
            
            // remove from array
            this->attachments.EraseIndex(index);
            index--;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::SendAttachmentMessage(const Attachment& attachment)
{
    Ptr<Graphics::BindAttachment> msg = Graphics::BindAttachment::Create();
    msg->SetJoint(attachment.joint.Value());
    msg->SetOffset(attachment.offset);
    msg->SetEntityToAttachObjectRef(attachment.attachedEntity->GetObjectRef());
    msg->SetBaseEntityObjectRef(attachment.baseEntity->GetObjectRef());
    msg->SetClearType(InternalGraphics::AttachmentServer::ClearAllIdenticalOnJoint);
    msg->SetKeepLocal(attachment.keepLocal);
    msg->SetRotation(attachment.rotation);
    Graphics::GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AttachmentManager::SendDetachmentMessage(const Attachment &detachment)
{
    Ptr<Graphics::UnbindAttachment> msg = Graphics::UnbindAttachment::Create();
    msg->SetBaseEntityObjectRef(detachment.baseEntity->GetObjectRef());
    msg->SetEntityToAttachObjectRef(detachment.attachedEntity->GetObjectRef());
    msg->SetJoint(detachment.joint.Value());
    msg->SetClearType(InternalGraphics::AttachmentServer::ClearAllIdenticalOnJoint);
    Graphics::GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void 
AttachmentManager::ClearAttachments()
{
    // clean up
    const Ptr<Graphics::Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage(); 
    IndexT i;
    for (i = 0; i < this->attachments.Size(); ++i)
    {
        if (this->attachments[i].attachedEntity.isvalid())
        {
            stage->RemoveEntity(this->attachments[i].attachedEntity.cast<Graphics::GraphicsEntity>());    	
        }        
    }
    this->attachments.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void 
AttachmentManager::ClearAttachmentsOnEntity(const Ptr<Game::Entity>& baseEntity)
{
    // get basegfxentity
    Ptr<GetGraphicsEntities> getGfx1 = GetGraphicsEntities::Create();
    baseEntity->SendSync(getGfx1.cast<Messaging::Message>());
    if (getGfx1->GetEntities().Size() > 0)
    {
        const Ptr<Graphics::GraphicsEntity>& baseGfxEntity = getGfx1->GetEntities()[0].cast<Graphics::GraphicsEntity>();

        // clean up
        const Ptr<Graphics::Stage>& stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();    
        IndexT i;
        for (i = 0; i < this->attachments.Size(); ++i)
        {
            if (this->attachments[i].attachedEntity.isvalid()
                && this->attachments[i].baseEntity == baseGfxEntity)
            {
                this->SendDetachmentMessage(this->attachments[i]);

                stage->RemoveEntity(this->attachments[i].attachedEntity.cast<Graphics::GraphicsEntity>());    	
                this->attachments.EraseIndex(i);
                --i;
            }        
        }
    }    
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Graphics::GraphicsEntity> 
AttachmentManager::GetAttachmentGfxEntity(const Ptr<Graphics::GraphicsEntity>& gfxBaseEntity, const Util::StringAtom& jointName) const
{    
    IndexT i;
    for (i = 0; i < this->attachments.Size(); ++i)
    {
        if (this->attachments[i].attachedEntity.isvalid()
            && this->attachments[i].baseEntity == gfxBaseEntity
            && this->attachments[i].joint == jointName)
        {
            return this->attachments[i].attachedEntity;
        }        
    }

    return Ptr<Graphics::GraphicsEntity>();
}

} // namespace GraphicsFeature
