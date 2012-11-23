#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::CameraEntity
    
    Client-side proxy of a InternalGraphics::InternalCameraEntity. NOTE: all getter-methods
    of this class return client-side cached values, not the actual server-side
    values. Thus they may be off by some amount, since the render thread
    may run at a different frame rate then the client thread!
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "graphics/view.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class CameraEntity : public GraphicsEntity
{
    __DeclareClass(CameraEntity);
public:
    /// constructor
    CameraEntity();
    /// destructor
    virtual ~CameraEntity();

    /// return true if this camera is currently attached to a view
    bool IsAttachedToView() const;
    /// get the view this camera is currently attached to
    const Ptr<View>& GetView() const;

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

protected:
    friend class View;

    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
    /// called by stage when entity should discard itself
    virtual void Discard();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called by view when this camera entity should become the view's camera
    void OnAttachToView(const Ptr<View>& view);
    /// called by view when this camera is no longer the view's camera
    void OnRemoveFromView(const Ptr<View>& view);    
    
    Shared::CameraSettings settings;
    Ptr<View> view;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraEntity::IsAttachedToView() const
{
    return this->view.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<View>&
CameraEntity::GetView() const
{
    return this->view;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetProjTransform() const
{
    return this->settings.GetProjTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetViewTransform() const
{
    return this->settings.GetViewTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
CameraEntity::GetViewProjTransform() const
{
    return this->settings.GetViewProjTransform();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::frustum&
CameraEntity::GetViewFrustum() const
{
    return this->settings.GetViewFrustum();
}

//------------------------------------------------------------------------------
/**
*/
inline const Shared::CameraSettings&
CameraEntity::GetCameraSettings() const
{
    return this->settings;
}

} // namespace Graphics
//------------------------------------------------------------------------------
