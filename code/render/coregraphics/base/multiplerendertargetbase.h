#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::MultipleRenderTargetBase

    Base class for render targets. A render targets wraps up to 4 color buffers
    and an optional depth/stencil buffer into a C++ object. The special
    default render target represents the backbuffer and default depth/stencil
    surface.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coregraphics/rendertarget.h"

//------------------------------------------------------------------------------
namespace Base
{
class MultipleRenderTargetBase : public Core::RefCounted
{
    __DeclareClass(MultipleRenderTargetBase);
public:
    /// max number of rendertargets 
    static const IndexT MaxNumRenderTargets = 4;

    /// constructor
    MultipleRenderTargetBase();
    /// destructor
    virtual ~MultipleRenderTargetBase();
    /// set rendertarget
    void AddRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get rendertarget at index
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTarget(IndexT i) const;
    /// getnumber of rendertargets used
    SizeT GetNumRendertargets() const;

    /// begin rendering to the render target
    void BeginPass();
    /// begin a batch
    void BeginBatch(CoreGraphics::BatchType::Code batchType);
    /// end current batch
    void EndBatch();
    /// end current render pass
    void EndPass(); 
    
    /// set clear color of rendertarget at index
    void SetClearColor(IndexT i, const Math::float4& color);
    /// set clear depth
    void SetClearDepth(float d);
    /// get clear depth
    float GetClearDepth() const;
    /// set clear stencil value
    void SetClearStencil(uchar s);
    /// get clear stencil value
    uchar GetClearStencil() const;

protected:
    friend class RenderDeviceBase;

    Ptr<CoreGraphics::RenderTarget> renderTarget[MaxNumRenderTargets];
    Math::float4 clearColor[MaxNumRenderTargets];
    bool clearDepthStencil;
    float clearDepth;
    uchar clearStencil;
    SizeT numRenderTargets;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>& 
MultipleRenderTargetBase::GetRenderTarget(IndexT i) const
{
    n_assert(i < this->numRenderTargets);
    return this->renderTarget[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT 
MultipleRenderTargetBase::GetNumRendertargets() const
{
    return this->numRenderTargets;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MultipleRenderTargetBase::SetClearDepth(float d)
{
    this->clearDepth = d;
    this->clearDepthStencil = true;
}

//------------------------------------------------------------------------------
/**
*/
inline float
MultipleRenderTargetBase::GetClearDepth() const
{
    return this->clearDepth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MultipleRenderTargetBase::SetClearStencil(uchar s)
{
    this->clearStencil = s;
    this->clearDepthStencil = true;
}

//------------------------------------------------------------------------------
/**
*/
inline uchar
MultipleRenderTargetBase::GetClearStencil() const
{
    return this->clearStencil;
}
} // namespace Base    
//------------------------------------------------------------------------------

