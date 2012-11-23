#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FramePassBase
    
    A frame pass encapsulates all 3d rendering to a render target, organized
    into FrameBatches.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "resources/resourceid.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/multiplerendertarget.h"
#include "coregraphics/shadervariableinstance.h"
#include "frame/framebatch.h"
#include "debug/debugtimer.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FramePassBase : public Core::RefCounted
{
    __DeclareClass(FramePassBase);
public:
    /// constructor
    FramePassBase();
    /// destructor
    virtual ~FramePassBase();
    /// discard the frame pass
    virtual void Discard();
    /// render the pass
    virtual void Render();

    /// set the name of the frame pass
    void SetName(const Resources::ResourceId& resId);
    /// get the name of the frame pass
    const Resources::ResourceId& GetName() const;
    /// set pass shader
    void SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd);
    /// get pass shader
    const Ptr<CoreGraphics::ShaderInstance>& GetShader() const;
    /// set render target
    void SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get render target
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTarget() const;
    /// set multiple render target
    void SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& rt);
    /// get multiple render target
    const Ptr<CoreGraphics::MultipleRenderTarget>& GetMultipleRenderTarget() const;

    /// add a shader variable instance to the frame pass
    void AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var);
    /// get number of shader variables
    SizeT GetNumVariables() const;
    /// get shader variable by index
    const Ptr<CoreGraphics::ShaderVariableInstance>& GetVariableByIndex(IndexT i) const;

    /// add a frame batch to the frame pass
    void AddBatch(const Ptr<FrameBatch>& batch);
    /// get number of frame batches
    SizeT GetNumBatches() const;
    /// get batch by index
    const Ptr<FrameBatch>& GetBatchByIndex(IndexT i) const;

    /// set clear flags (mask of RenderTarget::Clear*)
    void SetClearFlags(uint clearFlags);
    /// get clear flags
    uint GetClearFlags() const;
    /// set clear color for all rendertargets
    void SetClearColor(const Math::float4& c);
    /// get clear color 
    const Math::float4& GetClearColor() const;
    /// set clear depth for all rendertargets
    void SetClearDepth(float d);
    /// get clear depth
    float GetClearDepth() const;
    /// set clear stencil value for all rendertargets
    void SetClearStencil(uchar s);
    /// get clear stencil value
    uchar GetClearStencil() const;

protected:
    Resources::ResourceId name;
    Ptr<CoreGraphics::ShaderInstance> shader;
    Ptr<CoreGraphics::RenderTarget> renderTarget;
    Ptr<CoreGraphics::MultipleRenderTarget> multipleRenderTarget;
    Util::Array<Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariables;
    Util::Array<Ptr<FrameBatch> > batches;
    uint clearFlags;
    Math::float4 clearColor;
    float clearDepth;
    uchar clearStencil;
};

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetName(const Resources::ResourceId& resId)
{
    this->name = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
FramePassBase::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd)
{
    this->shader = shd;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>&
FramePassBase::GetShader() const
{
    return this->shader;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->renderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FramePassBase::GetRenderTarget() const
{
    return this->renderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& rt)
{
    this->multipleRenderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::MultipleRenderTarget>&
FramePassBase::GetMultipleRenderTarget() const
{
    return this->multipleRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var)
{
    this->shaderVariables.Append(var);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FramePassBase::GetNumVariables() const
{
    return this->shaderVariables.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariableInstance>& 
FramePassBase::GetVariableByIndex(IndexT i) const
{
    return this->shaderVariables[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::AddBatch(const Ptr<FrameBatch>& batch)
{
    this->batches.Append(batch);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FramePassBase::GetNumBatches() const
{
    return this->batches.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FrameBatch>&
FramePassBase::GetBatchByIndex(IndexT i) const
{
    return this->batches[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetClearFlags(uint f)
{
    this->clearFlags = f;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
FramePassBase::GetClearFlags() const
{
    return this->clearFlags;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetClearColor(const Math::float4& c)
{
    this->clearColor = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
FramePassBase::GetClearColor() const
{
    return this->clearColor;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetClearDepth(float d)
{
    this->clearDepth = d;
}

//------------------------------------------------------------------------------
/**
*/
inline float
FramePassBase::GetClearDepth() const
{
    return this->clearDepth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePassBase::SetClearStencil(uchar s)
{
    this->clearStencil = s;
}

//------------------------------------------------------------------------------
/**
*/
inline uchar
FramePassBase::GetClearStencil() const
{
    return this->clearStencil;
}

} // namespace Frame
//------------------------------------------------------------------------------

    