#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsEntity
    
    A graphics entity represents an atomic graphics object which can be
    attached to a Stage. Graphics entities come in three flavours:
    
    - ModelEntity: a visible model instance
    - LightEntity: a light source
    - CameraEntity: a camera 
    
    Visibility queries set graphics entities in relation to each other 
    through bidirectional links. A CameraEntity links to all 
    ModelEntities and LightEntities visible through the camera. Since
    visibility links are bidirectional, ModelEntities and LightEntities
    also know through which cameras they are visible. LightEntities
    have links to all ModelEntities they influence, and ModelEntities
    know by which lights they are lit. 
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/bbox.h"
#include "math/clipstatus.h"
#include "timing/time.h"
#include "internalgraphics/internalgraphicsentitytype.h"
#include "messaging/message.h"
#include "messaging/staticmessagehandler.h"
#include "framesync/framesyncshareddata.h"
#include "shared/graphics/graphicsentityshared.h"

namespace Visibility
{
    class VisibilityChecker;
}
//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalStage;
class Cell;

class InternalGraphicsEntity : public Core::RefCounted
{
    __DeclareClass(InternalGraphicsEntity);
public:
    /// visibility link types
    enum LinkType
    {
        CameraLink = 0,
        LightLink,
        
        NumLinkTypes,
    };

    /// a unique id for graphics entities
    typedef IndexT Id;

    /// constructor
    InternalGraphicsEntity();
    /// destructor
    virtual ~InternalGraphicsEntity();

    /// return true if entity is currently active (is between OnActivate()/OnDeactivate()
    bool IsActive() const;
    /// return true if entity is current valid (ready for rendering)
    bool IsValid() const;
    /// get the graphics entity's unique id
    Id GetId() const;
    /// get the entity type
    InternalGraphicsEntityType::Code GetType() const;

    /// set the entity's world space transform
    void SetTransform(const Math::matrix44& m);
    /// get the entity's world space transform
    const Math::matrix44& GetTransform() const;
    
    /// set the entity's visibility
    void SetVisible(bool b);
    /// return true if entity is set to visible
    bool IsVisible() const;

    /// get the stage this entity is attached to
    const Ptr<InternalStage>& GetStage() const;
    /// return true if entity is attached to stage
    bool IsAttachedToStage() const;

    /// get current entity time
    Timing::Time GetEntityTime() const;
    /// get the local space bounding box
    const Math::bbox& GetLocalBoundingBox();
    /// get bounding box in global space
    const Math::bbox& GetGlobalBoundingBox();

    /// clear all visibility links
    void ClearLinks(LinkType linkType);
    /// add visibility link
    void AddLink(LinkType linkType, const Ptr<InternalGraphicsEntity>& entity);
    /// get visibility links by type
    const Util::Array<Ptr<InternalGraphicsEntity> >& GetLinks(LinkType type) const;

    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

    /// mark the entity for removal from the stage at the next possible time
    void MarkRemove();
    /// return true if this entity has been marked for removal
    bool IsMarkedForRemove() const;

    /// handle a message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
    /// add a message for deferred handling once the object becomes valid
    void AddDeferredMessage(const Ptr<Messaging::Message>& msg);

protected:
    friend class InternalStage;
    friend class InternalView;
    friend class InternalGraphicsServer;
    friend class Messaging::StaticMessageHandler;   
    friend class Visibility::VisibilityChecker;

    /// set pointer to shared data object
    void SetSharedData(const Ptr<FrameSync::FrameSyncSharedData>& data);
    /// set entity type, call in constructor of derived class!
    void SetType(InternalGraphicsEntityType::Code t);
    /// set to valid state (when the entity becomes ready for rendering)
    void SetValid(bool b);
    /// update current clip status
    void UpdateClipStatus(Math::ClipStatus::Type c);
    /// update current time
    void UpdateTime(Timing::Time time, Timing::Time timeFactor);
    
    /// called when entity is created
    virtual void OnActivate();
    /// called before entity is destroyed
    virtual void OnDeactivate();

    /// called when attached to Stage
    virtual void OnAttachToStage(const Ptr<InternalStage>& stage);
    /// called when removed from Stage
    virtual void OnRemoveFromStage();

    /// called to setup the client-portion of the shared data object
    virtual void OnSetupSharedData();
    /// called to discard the client-portion of the shared data object
    virtual void OnDiscardSharedData();
    /// called per frame to reset the shared data object
    virtual void OnResetSharedData();
    
    /// called when the entity becomes visible
    virtual void OnShow();
    /// called when the entity becomes invisible
    virtual void OnHide();

    /// called from Render method in internalview
    virtual void OnResolveVisibility();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called before culling on each(!) graphics entity (visible or not!)
    virtual void OnCullBefore(Timing::Time time, Timing::Time globalTimeFactor, IndexT frameIndex);
    /// called when the entity has been found visible during culling, may be called several times per frame!
    virtual void OnNotifyCullingVisible(const Ptr<InternalGraphicsEntity>& observer, IndexT frameIndex);
    /// called right before rendering
    virtual void OnRenderBefore(IndexT frameIndex);
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    
    /// set the local space bounding box
    void SetLocalBoundingBox(const Math::bbox& b);
    /// update the global bounding box from the transform and local box
    void UpdateGlobalBoundingBox();
    /// handle deferred messages (called by subclasses once resources are loaded)
    void HandleDeferredMessages();

    static Id UniqueIdCounter;
    Id id;
    Math::matrix44 transform;

    Math::bbox localBox;
    Math::bbox globalBox;
    bool globalBoxDirty;

    Ptr<InternalStage> stage;
    Util::FixedArray<Util::Array<Ptr<InternalGraphicsEntity> > > links;
    InternalGraphicsEntityType::Code type;
    bool isActive;
    bool isValid;
    bool isVisible;
    bool transformChanged;
    bool removeFlag;
    IndexT updateFrameIndex;
    IndexT notifyCullingVisibleFrameIndex;
    Timing::Time entityTime;
    float timeFactor;
    Math::ClipStatus::Type clipStatus;
    Util::Array<Ptr<Messaging::Message> > deferredMessages;
    Ptr<FrameSync::FrameSyncSharedData> sharedData;
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetSharedData(const Ptr<FrameSync::FrameSyncSharedData>& data)
{
    this->sharedData = data;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::MarkRemove()
{
    this->removeFlag = true;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsMarkedForRemove() const
{
    return this->removeFlag;
}

//------------------------------------------------------------------------------
/**
*/
inline InternalGraphicsEntity::Id
InternalGraphicsEntity::GetId() const
{
    return this->id;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsActive() const
{
    return this->isActive;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetValid(bool b)
{
    this->isValid = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetType(InternalGraphicsEntityType::Code t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline InternalGraphicsEntityType::Code
InternalGraphicsEntity::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalGraphicsEntity::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsVisible() const
{
    return this->isVisible;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetLocalBoundingBox(const Math::bbox& b)
{
    this->localBox = b;
    this->globalBoxDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
InternalGraphicsEntity::GetLocalBoundingBox()
{
    return this->localBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
InternalGraphicsEntity::GetGlobalBoundingBox()
{
    // and now if something global happened
    if (this->globalBoxDirty)
    {
        this->UpdateGlobalBoundingBox();
    }
    return this->globalBox;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::AddLink(LinkType linkType, const Ptr<InternalGraphicsEntity>& entity)
{
    this->links[linkType].Append(entity);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalGraphicsEntity> >&
InternalGraphicsEntity::GetLinks(LinkType linkType) const
{
    return this->links[linkType];
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::UpdateClipStatus(Math::ClipStatus::Type c)
{
    this->clipStatus = c;
    this->sharedData->Client<Shared::GraphicsEntityShared>().clipStatus = c;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::UpdateTime(Timing::Time t, Timing::Time f)
{
    this->entityTime = t;
    this->timeFactor = float(f);
    this->sharedData->Client<Shared::GraphicsEntityShared>().entityTime = t;
    this->sharedData->Client<Shared::GraphicsEntityShared>().timeFactor = f;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
InternalGraphicsEntity::GetEntityTime() const
{
    return this->entityTime;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
    