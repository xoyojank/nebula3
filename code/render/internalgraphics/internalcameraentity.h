#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalCameraEntity
  
    Represents a camera attached to a graphics stage. Any number of
    cameras can be attached to a stage.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "internalgraphics/internalgraphicsentity.h"
#include "shared/graphics/camerasettings.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalView;

class InternalCameraEntity : public InternalGraphicsEntity
{
    __DeclareClass(InternalCameraEntity);
public:
    /// constructor
    InternalCameraEntity();
    /// destructor
    virtual ~InternalCameraEntity();
    
    /// return true if camera is attached to a View
    bool IsAttachedToView() const;
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

    /// set new camera settings
    void SetCameraSettings(const Shared::CameraSettings& camSettings);
    /// get camera settings
    const Shared::CameraSettings& GetCameraSettings() const;

    /// get projection matrix
    const Math::matrix44& GetProjTransform() const;
    /// get view transform (inverse transform)
    const Math::matrix44& GetViewTransform() const;
    /// get view projection matrix 
    const Math::matrix44& GetViewProjTransform() const;
    /// get view frustum
    const Math::frustum& GetViewFrustum() const;

    /// handle a message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:
    friend class InternalView;

    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called by View when camera is attached to that view
    void OnAttachToView(const Ptr<InternalView>& view);
    /// called by View when camera becomes detached from view
    void OnRemoveFromView(const Ptr<InternalView>& view);
    /// called when transform matrix changed
    virtual void OnTransformChanged();

    Ptr<InternalView> view;
    Shared::CameraSettings camSettings;
};

//------------------------------------------------------------------------------
/**
*/
inline const Shared::CameraSettings&
InternalCameraEntity::GetCameraSettings() const
{
    return this->camSettings;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetProjTransform() const
{
    return this->camSettings.GetProjTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetViewTransform() const
{
    return this->camSettings.GetViewTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalCameraEntity::GetViewProjTransform() const
{
    return this->camSettings.GetViewProjTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::frustum&
InternalCameraEntity::GetViewFrustum() const
{
    return this->camSettings.GetViewFrustum();
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
