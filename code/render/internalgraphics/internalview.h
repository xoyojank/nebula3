#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalView
  
    A graphics View is used to render a Stage through a CameraEntity into a 
    RenderTarget. Any number of views can be associated with the same Stage.
    Views may depend on other views. When a View is rendered, it will
    first ask the Views it depends on to render themselves. Subclasses of
    View may implement their own rendering strategies.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalcameraentity.h"
#include "coregraphics/texture.h"
#include "coregraphics/rendertarget.h"
#include "frame/frameshader.h"
#include "debug/debugtimer.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalView : public Core::RefCounted
{
    __DeclareClass(InternalView);
public:
    /// constructor
    InternalView();
    /// destructor
    virtual ~InternalView();

    /// return true if currently attached to graphics server
    bool IsAttachedToServer() const;
    /// get human-readable name
    const Util::StringAtom& GetName() const;
    /// set the stage this View is associated with
    void SetStage(const Ptr<InternalStage>& stage);
    /// get the stage this View is associated with
    const Ptr<InternalStage>& GetStage() const;
    /// set the CameraEntity this View looks through
    void SetCameraEntity(const Ptr<InternalCameraEntity>& camera);
    /// get the CameraEntity this View looks through
    const Ptr<InternalCameraEntity>& GetCameraEntity() const;
    /// set the render target this view renders to
    void SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& renderTarget);
    /// get the render target this view renders to
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTarget() const;
    /// check if rendertarget is set
    bool HasRenderTarget() const;
    /// set the render target this view renders to
    void SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& renderTarget);
    /// get the render target this view renders to
    const Ptr<CoreGraphics::MultipleRenderTarget>& GetMultipleRenderTarget() const;
    /// set the view's frame shader 
    void SetFrameShader(const Ptr<Frame::FrameShader>& frameShader);
    /// get the view's frame shader
    const Ptr<Frame::FrameShader>& GetFrameShader() const;

    /// add a view which this view depends on
    void AddDependency(const Ptr<InternalView>& view);
    /// get all dependency views
    const Util::Array<Ptr<InternalView> >& GetDependencies() const;

    /// update the visibility links for this view 
    virtual void UpdateVisibilityLinks();
    /// apply camera settings
    void ApplyCameraSettings();
    /// render the view into its render target
    virtual void Render(IndexT frameIndex);
    /// render a debug view of the world
    virtual void RenderDebug();

protected:
    friend class InternalGraphicsServer;

    /// set a human-readable name of the view
    void SetName(const Util::StringAtom& name);
    /// called when attached to graphics server
    virtual void OnAttachToServer();
    /// called when detached from graphics server
    virtual void OnRemoveFromServer();
    /// resolve visible lights
    void ResolveVisibleLights();
    /// resolve visibility for optimal batch rendering
    void ResolveVisibleModelNodeInstances(IndexT frameIndex);

    bool isAttachedToServer;
    Util::StringAtom name;
    Ptr<InternalStage> stage;
    Ptr<InternalCameraEntity> camera;
    Ptr<CoreGraphics::RenderTarget> renderTarget;
    Ptr<CoreGraphics::MultipleRenderTarget> multipleRenderTarget;
    Ptr<Frame::FrameShader> frameShader;
    Util::Array<Ptr<InternalView> > dependencies;    
    
    _declare_timer(resolveVisibleModelNodeInstances);
    _declare_timer(updateShadowBuffers);
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalView::IsAttachedToServer() const
{
    return this->isAttachedToServer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
InternalView::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::SetStage(const Ptr<InternalStage>& s)
{
    this->stage = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalStage>&
InternalView::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalCameraEntity>&
InternalView::GetCameraEntity() const
{
    return this->camera;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->renderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
InternalView::GetRenderTarget() const
{
    return this->renderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& mrt)
{
    this->multipleRenderTarget = mrt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::MultipleRenderTarget>&
InternalView::GetMultipleRenderTarget() const
{
    return this->multipleRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::AddDependency(const Ptr<InternalView>& depView)
{
    this->dependencies.Append(depView);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalView> >&
InternalView::GetDependencies() const
{
    return this->dependencies;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalView::SetFrameShader(const Ptr<Frame::FrameShader>& shd)
{
    this->frameShader = shd;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Frame::FrameShader>&
InternalView::GetFrameShader() const
{
    return this->frameShader;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
