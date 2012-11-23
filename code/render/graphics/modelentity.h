#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::ModelEntity
    
    Client-side proxy of a InternalGraphics::InternalModelEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "graphics/graphicsprotocol.h"
#include "shared/graphics/modelentityshared.h"
#include "shared/graphics/charjointinfo.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class ModelEntity : public GraphicsEntity
{
    __DeclareClass(ModelEntity);
public:
    /// constructor
    ModelEntity();
    /// destructor
    virtual ~ModelEntity();

    /// set the model's resource id
    void SetResourceId(const Resources::ResourceId& resId);
    /// get the model's resource id
    const Resources::ResourceId& GetResourceId() const;
    /// set optional root node path (allows to setup ModelEntity from a child node in a model node hierarchy)
    void SetRootNodePath(const Util::StringAtom& rootNodePath);
    /// get optional root node path
    const Util::StringAtom& GetRootNodePath() const;
    /// set optional root node offset matrix
    void SetRootNodeOffsetMatrix(const Math::matrix44& offsetMatrix);
    /// get optional root node offset matrix
    const Math::matrix44& GetRootNodeOffsetMatrix() const;

    /// enable anim driven motion tracking
    void ConfigureAnimDrivenMotionTracking(bool enabled, const Util::StringAtom& jointName);
    /// enable anim event tracking
    void ConfigureAnimEventTracking(bool enabled, bool onlyDominatingClip);
    /// enable joint tracking
    void ConfigureCharJointTracking(bool enabled, const Util::Array<Util::StringAtom>& jointNames);

    /// return true if anim driven motion tracking is enabled
    bool IsAnimDrivenMotionTrackingEnabled() const;
    /// get the anim-driven-motion tracking joint name
    const Util::StringAtom& GetAnimDrivenMotionJointName() const;
    /// get the current anim-driven-motion tracking vector
    const Math::vector& GetAnimDrivenMotionVector() const;

    /// return true if anim event tracking is enabled
    bool IsAnimEventTrackingEnabled() const;
    /// return the array of anim events of the current frame
    const Util::Array<Animation::AnimEventInfo>& GetAnimEvents() const;

    /// return true if char joint tracking is enabled
    bool IsCharJointTrackingEnabled() const;
    /// return true if current char joint data is valid in this frame (may change by visibility)
    bool IsCharJointDataValid() const;
    /// dynamically add a tracked character joint
    void AddTrackedCharJoint(const Util::StringAtom& jointName);
    /// access tracked char joint data, return NULL ptr if joint data not valid or not yet available!
    const Shared::CharJointInfo* GetTrackedCharJointInfo(const Util::StringAtom& jointName) const;

    /// update the value of a shader variable on a node instance of the model entity
    void UpdateModelNodeInstanceShaderVariable(const Util::StringAtom& nodeName, const Util::StringAtom& variableSemantic, const Util::Variant& value);
    /// update the visibility of a model node instance node
    void UpdateModelNodeInstanceVisibility(const Util::StringAtom& nodeName, bool visible);

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
    /// called to setup the FrameSyncSharedData object of the entity
    virtual void OnSetupSharedData();
    /// called to discard the FrameSyncSharedData object of the entity
    virtual void OnDiscardSharedData();

    Resources::ResourceId resId;
    Util::StringAtom rootNodePath;
    Math::matrix44 rootNodeOffsetMatrix;

    bool animDrivenMotionTrackingEnabled;
    bool animEventTrackingEnabled;
    bool animEventTrackingOnlyDominatingClip;
    bool jointTrackingEnabled;
    Util::StringAtom animDrivenMotionJointName;
    Util::Array<Util::StringAtom> trackedJoints;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    this->resId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
ModelEntity::GetResourceId() const
{
    return this->resId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetRootNodePath(const Util::StringAtom& p)
{
    this->rootNodePath = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ModelEntity::GetRootNodePath() const
{
    return this->rootNodePath;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetRootNodeOffsetMatrix(const Math::matrix44& m)
{
    this->rootNodeOffsetMatrix = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ModelEntity::GetRootNodeOffsetMatrix() const
{
    return this->rootNodeOffsetMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelEntity::IsAnimDrivenMotionTrackingEnabled() const
{
    return this->animDrivenMotionTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ModelEntity::GetAnimDrivenMotionJointName() const
{
    return this->animDrivenMotionJointName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
ModelEntity::GetAnimDrivenMotionVector() const
{
    return this->sharedData->Owner<Shared::ModelEntityShared>().animDrivenMotionVector;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelEntity::IsAnimEventTrackingEnabled() const
{
    return this->animEventTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Animation::AnimEventInfo>&
ModelEntity::GetAnimEvents() const
{
    return this->sharedData->Owner<Shared::ModelEntityShared>().animEvents;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelEntity::IsCharJointTrackingEnabled() const
{
    return this->jointTrackingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelEntity::IsCharJointDataValid() const
{
    return this->sharedData->Owner<Shared::ModelEntityShared>().charJointDataValid;
}

} // namespace Graphics
//------------------------------------------------------------------------------
