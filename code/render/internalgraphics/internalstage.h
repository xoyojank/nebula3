#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalStage
    
    A graphics stage groups graphics entities (models, cameras and lights)
    together for rendering. The main job of a Stage is to speed up
    visibility queries between the attached graphics entities. Different
    visibility query strategies are implemented by Stage subclasses. 
    Nebula3 comes with a set of generic Stage subclasses for different
    purposes, but applications are free to derive their own subclasses
    which implement visibility query mechanisms tailored to the application.
    
    Visibility queries exist in the following variations:
    
    - Camera->Light: this finds all light entities visible from a
      given camera
    - Camera->Model: this finds all model entities visible from a
      given camera
    - Light->Model: this finds all model entities which are lit
      by a given light source
    
    Those visibility queries establish so-called visibility links
    between graphics entities which are then used by the lower
    level rendering subsystems to speed up rendering.

    To render the content of a stage, at least one View object is 
    needed. A View object binds renders a stage through a camera entity
    into a render target. Any number of View objects can exist in
    parallel, and may be bound to any Stage. Furthermore, dependencies
    between View objects may be defined (so that a View object will
    first ask the View objects it depends on to render themselves).

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalgraphicsentitytype.h"
#include "timing/time.h"                   
#include "visibility/visibilitychecker.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalCameraEntity;

class InternalStage : public Core::RefCounted
{
    __DeclareClass(InternalStage);
public:
    /// constructor
    InternalStage();
    /// destructor
    virtual ~InternalStage();

    /// return true if currently attached to graphics server
    bool IsAttachedToServer() const;
    /// get human readable name of the stage
    const Util::StringAtom& GetName() const;
    
    /// attach an entity to the stage
    virtual void AttachEntity(const Ptr<InternalGraphicsEntity>& graphicsEntity);
    /// remove an entity from the stage
    virtual void RemoveEntity(const Ptr<InternalGraphicsEntity>& entity);
    /// notify of an transform change
    virtual void NotifyOfEntityTransformChange(const Ptr<InternalGraphicsEntity>& entity);
    /// get an array of all entities attached to the stage
    const Util::Array<Ptr<InternalGraphicsEntity> >& GetEntities() const;
    /// get entities by type
    const Util::Array<Ptr<InternalGraphicsEntity> >& GetEntitiesByType(InternalGraphicsEntityType::Code type) const;
    /// remove all entities
    void RemoveAllEntities();

    /// call OnCullBefore in entities in the stage
    virtual void OnCullBefore(Timing::Time curTime, Timing::Time globalTimeFactor, IndexT frameIndex);
    /// update camera links for a given camera
    virtual void UpdateCameraLinks(const Ptr<InternalCameraEntity>& cameraEntity);
    /// update light links for all visible lights (after updating camera links!)
    virtual void UpdateLightLinks();

    /// attach visibility systems to checker
    void AttachVisibilitySystems(const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& systems);
    /// get visibility checker
    Visibility::VisibilityChecker& GetVisibilityChecker();
    /// on render debug
    void OnRenderDebug();

protected:
    friend class InternalGraphicsServer;

    /// set a human readable name on the stage
    void SetName(const Util::StringAtom& name);
    /// called when the stage is attached to graphics server
    virtual void OnAttachToServer();
    /// called when the stage is detached from graphics server
    virtual void OnRemoveFromServer();

    bool isAttachedToServer;
    IndexT curFrameIndex;
    Util::StringAtom name;
    Visibility::VisibilityChecker visibilityChecker;    
    
    Util::Array<Ptr<InternalGraphicsEntity> > entities;
    Util::Array<Ptr<InternalGraphicsEntity> > entitiesByType[InternalGraphicsEntityType::NumTypes];
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalStage::IsAttachedToServer() const
{
    return this->isAttachedToServer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalStage::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
InternalStage::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >&
InternalStage::GetEntities() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >&
InternalStage::GetEntitiesByType(InternalGraphicsEntityType::Code entityType) const
{
    n_assert((entityType >= 0) && (entityType < InternalGraphicsEntityType::NumTypes));
    return this->entitiesByType[entityType];
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
