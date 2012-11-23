#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalModelEntity
    
    Represents a visible graphics object.
    
    (C) 2007 Radon Labs GmbH
*/
#include "internalgraphics/internalgraphicsentity.h"
#include "resources/resourceid.h"
#include "models/managedmodel.h"
#include "models/modelinstance.h"
#include "characters/characterinstance.h"
#include "graphics/graphicsprotocol.h"
#include "shared/graphics/modelentityshared.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalModelEntity : public InternalGraphicsEntity
{
    __DeclareClass(InternalModelEntity);
public:
    /// constructor
    InternalModelEntity();
    /// destructor
    virtual ~InternalModelEntity();
    
    /// set the model's resource id
    void SetResourceId(const Resources::ResourceId& resId);
    /// get the model's resource id
    const Resources::ResourceId& GetResourceId() const;
    /// optional path to root node (allows to instantiate a ModelEntity from part of a ModelNode hierarchy)
    void SetRootNodePath(const Util::StringAtom& rootNodePath);
    /// get root node path, return invalid string atom if root node
    const Util::StringAtom& GetRootNodePath() const;
    /// set optional root node offset matrix
    void SetRootNodeOffsetMatrix(const Math::matrix44& offsetMatrix);
    /// get optional root node offset matrix
    const Math::matrix44& GetRootNodeOffsetMatrix() const;

    /// enable anim driven motion tracking
    void ConfigureAnimDrivenMotionTracking(bool enabled, const Util::StringAtom& jointName);
    /// enable anim event tracking
    void ConfigureAnimEventTracking(bool enabled, bool onlyDominatingClip);
    /// configure joint tracking
    void ConfigureCharJointTracking(bool enabled, const Util::Array<Util::StringAtom>& jointNames);

    /// get the state of the contained managed model resource
    Resources::Resource::State GetModelResourceState() const;
    /// get pointer to model instance (only valid if already loaded)
    const Ptr<Models::ModelInstance>& GetModelInstance() const;

    /// return true if this is a character
    bool HasCharacter() const;
    /// get pointer to character instance
    const Ptr<Characters::CharacterInstance>& GetCharacterInstance() const;
    /// get pointer to character
    const Ptr<Characters::Character>& GetCharacter() const;

    /// return true if anim driven motion tracking is enabled
    bool IsAnimDrivenMotionTrackingEnabled() const;
    /// get anim driven motion joint name
    const Util::StringAtom& GetAnimDrivenMotionJointName() const;
    /// get the current anim-driven-motion tracking vector
    const Math::vector& GetAnimDrivenMotionVector() const;

    /// return true if anim event tracking is enabled
    bool IsAnimEventTrackingEnabled() const;
    /// return the array of anim events of the current frame
    const Util::Array<Animation::AnimEventInfo>& GetAnimEvents() const;

    /// return true if joint tracking is enabled
    bool IsCharJointTrackingEnabled() const;
    /// dynamically add a tracked joint, note: you cannot remove tracked joints once added!
    void AddTrackedCharJoint(const Util::StringAtom& jointName);
    /// return true if current char joint data is valid in this frame (may change by visibility)
    bool IsCharJointDataValid() const;
    /// get char joint information
    const Util::Array<Shared::CharJointInfo>& GetCharJointInfos() const;

    /// handle a message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    friend class InternalView;
    friend class Visibility::VisibilityChecker;

    /// called when entity is created
    virtual void OnActivate();
    /// called before entity is destroyed
    virtual void OnDeactivate();

    /// called to setup the client-portion of the shared data object
    virtual void OnSetupSharedData();
    /// called to discard the client-portion of the shared data object
    virtual void OnDiscardSharedData();
    /// called per frame to reset the shared data object
    virtual void OnResetSharedData();

    /// called from internalview
    virtual void OnResolveVisibility();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called before culling on each(!) graphics entity (visible or not!)
    virtual void OnCullBefore(Timing::Time time, Timing::Time globalTimeFactor, IndexT frameIndex);
    /// called when the entity has been found visible, may be called several times per frame!
    virtual void OnNotifyCullingVisible(const Ptr<InternalGraphicsEntity>& observer, IndexT frameIndex);
    /// called right before rendering
    virtual void OnRenderBefore(IndexT frameIndex);
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    /// validate the ModelInstance
    void ValidateModelInstance();    
    /// called when the entity becomes visible
    virtual void OnShow();
    /// called when the entity becomes invisible
    virtual void OnHide();
    /// handle per-frame anim events
    void HandleCharacterAnimEvents(Timing::Time time);
    /// handle per-frame anim-driven-movement stuff
    void HandleCharacterAnimDrivenMotion();
    /// handle tracked character joints
    void HandleTrackedJoints();

    IndexT updateFrameId;
    Resources::ResourceId resId;
    Util::StringAtom rootNodePath;
    Math::matrix44 rootNodeOffsetMatrix;
    Ptr<Models::ManagedModel> managedModel;
    Ptr<Models::ModelInstance> modelInstance;
    Math::matrix44 rot180Transform;

    // character stuff
    Ptr<Characters::CharacterInstance> charInst;
    Ptr<Characters::Character> character;
    bool nebula2CharacterRotationHack;

    // anim driven motion
    bool animDrivenMotionTrackingEnabled;
    Util::StringAtom animDrivenMotionJointName;
    Math::vector animDrivenMotionVector;

    // anim events
    bool animEventTrackingEnabled;
    bool animEventOnlyDominatingClip;
    Timing::Tick animEventLastTick;

    // character joint tracking
    bool jointTrackingEnabled;
    Util::Dictionary<Util::StringAtom,IndexT> trackedJoints;    // jointName -> key, jointIndex -> value
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    n_assert(!this->IsActive());
    this->resId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
InternalModelEntity::GetResourceId() const
{
    return this->resId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalModelEntity::SetRootNodePath(const Util::StringAtom& p)
{
    this->rootNodePath = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
InternalModelEntity::GetRootNodePath() const
{
    return this->rootNodePath;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalModelEntity::SetRootNodeOffsetMatrix(const Math::matrix44& m)
{
    this->rootNodeOffsetMatrix = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalModelEntity::GetRootNodeOffsetMatrix() const
{
    return this->rootNodeOffsetMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline Resources::Resource::State
InternalModelEntity::GetModelResourceState() const
{
    return this->managedModel->GetState();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Models::ModelInstance>&
InternalModelEntity::GetModelInstance() const
{
    return this->modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::HasCharacter() const
{
    return this->charInst.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Characters::CharacterInstance>&
InternalModelEntity::GetCharacterInstance() const
{
    return this->charInst;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Characters::Character>&
InternalModelEntity::GetCharacter() const
{
    return this->character;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::IsAnimDrivenMotionTrackingEnabled() const
{
    return this->animDrivenMotionTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
InternalModelEntity::GetAnimDrivenMotionJointName() const
{
    return this->animDrivenMotionJointName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
InternalModelEntity::GetAnimDrivenMotionVector() const
{
    return this->sharedData->Client<Shared::ModelEntityShared>().animDrivenMotionVector;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::IsAnimEventTrackingEnabled() const
{
    return this->animEventTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Animation::AnimEventInfo>&
InternalModelEntity::GetAnimEvents() const
{
    return this->sharedData->Client<Shared::ModelEntityShared>().animEvents;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::IsCharJointTrackingEnabled() const
{
    return this->jointTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalModelEntity::IsCharJointDataValid() const
{
    return this->sharedData->Client<Shared::ModelEntityShared>().charJointDataValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Shared::CharJointInfo>&
InternalModelEntity::GetCharJointInfos() const
{
    return this->sharedData->Client<Shared::ModelEntityShared>().charJoints;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------    