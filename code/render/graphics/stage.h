#pragma once
//------------------------------------------------------------------------------
/**
    class Graphics::Stage
    
    A client-side proxy of a InternalGraphics::InternalStage.
    The Stage offers a friendly frontend to the
    client thread, and communicates with its server-side 
    Stage object through the GraphicsInterface method. There
    is a 1:1 relationship between the server-side Stage and the
    client-side Stage, thus the Stage may safely store
    read-only information on the client side without synchronizing
    with its InternalStage.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/rtti.h"
#include "util/stringatom.h"
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class ModelEntity;

class Stage : public Core::RefCounted
{
    __DeclareClass(Stage);
public:
    /// constructor
    Stage();
    /// destructor
    virtual ~Stage();

    /// return true if the Stage is valid
    bool IsValid() const;
    /// get name of stage
    const Util::StringAtom& GetName() const;

    /// attach a graphics entity
    void AttachEntity(const Ptr<GraphicsEntity>& entity);
    /// remove a graphics entity 
    void RemoveEntity(const Ptr<GraphicsEntity>& entity);
    /// get an array of all entities attached to the stage
    const Util::Array<Ptr<GraphicsEntity> >& GetEntities() const;
    /// get an array of entities filtered by type
    const Util::Array<Ptr<GraphicsEntity> >& GetEntitiesByType(GraphicsEntityType::Code type) const;
    
private:
    friend class GraphicsServer;

    /// set name of stage
    void SetName(const Util::StringAtom& name);
    /// setup the stage (waits for completion)
    void Setup(const Util::Array<Ptr<Visibility::VisibilitySystemBase> >& visSystems);
    /// discard the stage (waits for completion)
    void Discard();
    /// call this method per-frame
    void OnFrame();

    Util::StringAtom name;
    Ptr<Threading::ObjectRef> objectRef;
    Util::Array<Ptr<GraphicsEntity> > entities;
    Util::Array<Ptr<GraphicsEntity> > entitiesByType[GraphicsEntityType::NumTypes];
    Util::Array<Ptr<GraphicsEntity> > pendingEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Stage::IsValid() const
{
    return this->objectRef.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Stage::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
Stage::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >& 
Stage::GetEntities() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >& 
Stage::GetEntitiesByType(GraphicsEntityType::Code entityType) const
{
    n_assert((entityType >= 0) && (entityType < GraphicsEntityType::NumTypes));
    return this->entitiesByType[entityType];
}

} // namespace Graphics
//------------------------------------------------------------------------------
