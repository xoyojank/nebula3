#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::GraphicsEntity
    
    Client-side proxy of a InternalGraphics::InternalGraphicsEntity.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "messaging/message.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsentitytype.h"
#include "shared/graphics/graphicsentityshared.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage;

class GraphicsEntity : public Core::RefCounted
{
    __DeclareClass(GraphicsEntity);
public:
    /// constructor
    GraphicsEntity();
    /// destructor
    virtual ~GraphicsEntity();
        
    /// return true if entity is valid (is attached to a stage)
    bool IsValid() const;
    /// get the entity type
    GraphicsEntityType::Code GetType() const;
    /// set the entity's world space transform
    void SetTransform(const Math::matrix44& m);
    /// get the entity's world space transform
    const Math::matrix44& GetTransform() const;
    /// set the entity's visibility
    void SetVisible(bool b);
    /// return true if entity is set to visible
    bool IsVisible() const;
    /// get the stage proxy this entity is attached to
    const Ptr<Stage>& GetStage() const;
    /// get current clip status
    Math::ClipStatus::Type GetClipStatus() const;
    /// get the local bounding box
    const Math::bbox& GetLocalBoundingBox() const;
    /// get the global bounding box
    const Math::bbox& GetGlobalBoundingBox() const;
    /// get current entity time
    Timing::Time GetEntityTime() const;

    /// send a message to the server-side graphics entity
    void SendMsg(const Ptr<GraphicsEntityMessage>& msg);
    /// test if the entity's object ref is valid
    bool IsObjectRefValid() const;
    /// get the entity handle
    const Ptr<Threading::ObjectRef>& GetObjectRef() const;

protected:
    friend class Stage;

    /// set graphics entity type, called from constructor of subclass
    void SetType(GraphicsEntityType::Code t);
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
    /// called by stage when entity should discard itself
    virtual void Discard();
    /// called to setup the FrameSyncSharedData object of the entity
    virtual void OnSetupSharedData();
    /// called to discard the FrameSyncSharedData object of the entity
    virtual void OnDiscardSharedData();
    /// send off a specific create message from the subclass
    void SendCreateMsg(const Ptr<CreateGraphicsEntity>& msg);
    /// called when transform matrix changed
    virtual void OnTransformChanged();

    Math::matrix44 transform;
    Math::bbox localBoundingBox;
    Ptr<Stage> stage;
    GraphicsEntityType::Code type;
    Ptr<Threading::ObjectRef> objectRef;
    Ptr<FrameSync::FrameSyncSharedData> sharedData;
    bool isVisible;
}; 

//------------------------------------------------------------------------------
/**
*/
inline Math::ClipStatus::Type
GraphicsEntity::GetClipStatus() const
{
    return this->sharedData->Owner<Shared::GraphicsEntityShared>().clipStatus;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
GraphicsEntity::GetLocalBoundingBox() const
{
    return this->sharedData->Owner<Shared::GraphicsEntityShared>().localBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
GraphicsEntity::GetGlobalBoundingBox() const
{
    return this->sharedData->Owner<Shared::GraphicsEntityShared>().globalBox;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
GraphicsEntity::GetEntityTime() const
{
    return this->sharedData->Owner<Shared::GraphicsEntityShared>().entityTime;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
GraphicsEntity::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsEntity::IsVisible() const
{
    return this->isVisible;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
GraphicsEntity::IsObjectRefValid() const
{
    return this->objectRef->IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Threading::ObjectRef>&
GraphicsEntity::GetObjectRef() const
{
    return this->objectRef;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GraphicsEntity::SetType(GraphicsEntityType::Code t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline GraphicsEntityType::Code
GraphicsEntity::GetType() const
{
    return this->type;
}

} // namespace Graphics
//------------------------------------------------------------------------------

    
