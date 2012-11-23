//------------------------------------------------------------------------------
//  internalstage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalcameraentity.h"
#include "timing/timer.h"
#include "rendermodules/rt/rtpluginregistry.h"
#include "visibility/visibilitysystems/visibilityquadtree.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/rendershape.h"
#include "lighting/internalgloballightentity.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalStage, 'IGSG', Core::RefCounted);

using namespace Math;
using namespace Util;
using namespace Visibility;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
InternalStage::InternalStage() :
    isAttachedToServer(false),
    curFrameIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalStage::~InternalStage()
{
    // make sure we've been properly cleaned up
    n_assert(!this->IsAttachedToServer());
    n_assert(this->entities.IsEmpty());
    IndexT i;
    for (i = 0; i < InternalGraphicsEntityType::NumTypes; i++)
    {
        n_assert(this->entitiesByType[i].IsEmpty());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::OnAttachToServer()
{
    n_assert(!this->IsAttachedToServer());
    this->isAttachedToServer = true;
        
    // open checker, will open its attached visibility systems
    this->visibilityChecker.Open();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::OnRemoveFromServer()
{
    n_assert(this->IsAttachedToServer());

    // properly cleanup entities
    IndexT entityIndex;
    for (entityIndex = 0; entityIndex < this->entities.Size(); entityIndex++)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = this->entities[entityIndex];
        curEntity->OnRemoveFromStage();
        curEntity->OnDeactivate();
    }
    this->entities.Clear();
    IndexT typeIndex;
    for (typeIndex = 0; typeIndex < InternalGraphicsEntityType::NumTypes; typeIndex++)
    {
        this->entitiesByType[typeIndex].Clear();
    }

    // close visibility checker
    this->visibilityChecker.Close();

    this->isAttachedToServer = false;
}

//------------------------------------------------------------------------------
/**
    Add an entity to the stage. The method OnAttachToStage() will be
    invoked on the entity, and the entity will be inserted into 
    the cell hierarchy of the stage (which in turn call OnAttachToCell()
    on the entity).
*/
void
InternalStage::AttachEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(!entity->IsActive());
    n_assert(!entity->IsAttachedToStage());
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);
    entity->OnActivate();
    entity->OnAttachToStage(this);
    this->visibilityChecker.RegisterEntity(entity);

    // notify render-thread plugins
    RenderModules::RTPluginRegistry::Instance()->OnAttachEntity(entity);
}

//------------------------------------------------------------------------------
/**
    Remove an entity from the stage. This will remove the entity from
    the cell hierarchy of the stage (which invoked OnRemoveFromCell()
    on the entity), and then the method OnRemoveFromStage() will
    be called on the entity.
*/
void
InternalStage::RemoveEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    n_assert(entity->IsActive());
    n_assert(entity->IsAttachedToStage());
    n_assert(entity->GetStage().get() == this);
    n_assert(entity->GetType() < InternalGraphicsEntityType::NumTypes);

    // notify render-thread plugins
    RenderModules::RTPluginRegistry::Instance()->OnRemoveEntity(entity);

    // first remove entity from vischecker, stage and deactivate it
    this->visibilityChecker.UnregisterEntity(entity);
    entity->OnRemoveFromStage();
    entity->OnDeactivate();
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);

    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
InternalStage::RemoveAllEntities()
{
    while (this->entities.Size() > 0)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = this->entities[0];
        this->RemoveEntity(curEntity);
    }
    this->entities.Clear();
    IndexT typeIndex;
    for (typeIndex = 0; typeIndex < InternalGraphicsEntityType::NumTypes; typeIndex++)
    {
        this->entitiesByType[typeIndex].Clear();
    }
}

//------------------------------------------------------------------------------
/**
    Call the OnCullBefore() method on ALL methods in the stage (no
    matter whether they are visible or not).
*/
void
InternalStage::OnCullBefore(Timing::Time curTime, Timing::Time globalTimeFactor, IndexT frameIndex)
{
    // check if entities have already been updated for this frame
    if (frameIndex != this->curFrameIndex)
    {
        this->curFrameIndex = frameIndex;
        
        // start at end, because we might end up removing entities
        // during the loop!
        IndexT entityIndex;
        for (entityIndex = this->entities.Size() - 1; entityIndex >= 0; entityIndex--)
        {
            const Ptr<InternalGraphicsEntity>& curEntity = this->entities[entityIndex];
            if (curEntity->IsMarkedForRemove())
            {
                Ptr<InternalGraphicsEntity> safePtr = curEntity;
                this->RemoveEntity(safePtr);
            }
            else
            {
                curEntity->OnResetSharedData();
                curEntity->OnCullBefore(curTime, globalTimeFactor, frameIndex); 
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Update visibility links for a given camera. This will create bidirectional
    visibility links between the camera and all other entities (most importantly
    light and model entities) which are visible through this camera. This
    method must be called once for each active camera after UpdateEntities()
    and before UpdateVisibleLightLinks().
*/
void
InternalStage::UpdateCameraLinks(const Ptr<InternalCameraEntity>& cameraEntity)
{
    n_assert(cameraEntity.isvalid());

    // clear old links
    visibilityChecker.ClearVisibilityLinks(InternalGraphicsEntity::CameraLink);

    if (cameraEntity.isvalid())
    {   
        // resolve visible model and light entities from this camera
        uint entityTypeMask = (1 << InternalGraphicsEntityType::Model) | (1 << InternalGraphicsEntityType::Light);
        this->visibilityChecker.PerformVisibilityQuery(this->curFrameIndex, cameraEntity.cast<InternalGraphicsEntity>(), entityTypeMask);        
    }
}

//------------------------------------------------------------------------------
/**
    For each visible light entity, this method will create light links
    between the light entities, and model entities influenced by 
    this light. This method must be called after UpdateCameraLinks() (this
    makes sure that no invisible lights and models will be checked).
*/
void
InternalStage::UpdateLightLinks()
{
    // clear old links
    visibilityChecker.ClearVisibilityLinks(InternalGraphicsEntity::LightLink);
    // for each visible light...
    const Array<Ptr<InternalGraphicsEntity> >& lightEntities = this->entitiesByType[InternalGraphicsEntityType::Light];
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < lightEntities.Size(); lightIndex++)
    {
        const Ptr<InternalGraphicsEntity>& lightEntity = lightEntities[lightIndex];
        if (lightEntity->GetLinks(InternalGraphicsEntity::CameraLink).Size() > 0)
        {
            // find model entities influenced by this light
            this->visibilityChecker.PerformVisibilityQuery(this->curFrameIndex, lightEntity, (1<<InternalGraphicsEntityType::Model));            
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalStage::NotifyOfEntityTransformChange(const Ptr<InternalGraphicsEntity>& entity)
{
    this->visibilityChecker.UpdateVisibilityContext(entity);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalStage::AttachVisibilitySystems(const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& systems)
{
    this->visibilityChecker.AttachVisibilitySystems(systems);
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalStage::OnRenderDebug()
{
    this->visibilityChecker.OnRenderDebug();

    // render light links
    const Array<Ptr<InternalGraphicsEntity> >& lightEntities = this->entitiesByType[InternalGraphicsEntityType::Light];
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < lightEntities.Size(); lightIndex++)
    {
        // skip global light
        if (lightEntities[lightIndex]->IsA(Lighting::InternalGlobalLightEntity::RTTI))
        {
            continue;
        }

        const Util::Array<Ptr<InternalGraphicsEntity> >& linkedEntities = lightEntities[lightIndex]->GetLinks(InternalGraphicsEntity::LightLink);
        if (linkedEntities.Size() > 0)
        {   
            const Math::point& lightPos = lightEntities[lightIndex]->GetTransform().get_position();
            Util::Array<Math::point> lines;        
            IndexT i;
            for (i = 0; i < linkedEntities.Size(); ++i)
            {
                const Math::point& entityPos = linkedEntities[i]->GetTransform().get_position();	
                lines.Append(lightPos);                  
                lines.Append(entityPos);
            }   
        
            RenderShape shape;
            shape.SetupPrimitives(Threading::Thread::GetMyThreadId(),
                matrix44::identity(),
                PrimitiveTopology::LineList,
                lines.Size() / 2,
                &(lines.Front()),
                4,
                point(1,1,0));
            ShapeRenderer::Instance()->AddShape(shape); 
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
Visibility::VisibilityChecker& 
InternalStage::GetVisibilityChecker()
{
    return this->visibilityChecker;
}
} // namespace InternalGraphics