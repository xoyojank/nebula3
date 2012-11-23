#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FrameBatch
    
    A frame batch encapsulates the rendering of a batch of ModelNodeInstances.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/batchtype.h"
#include "coregraphics/shaderfeature.h"
#include "models/modelnodetype.h"
#include "frame/lightingmode.h"
#include "frame/sortingmode.h"
#include "debug/debugtimer.h"

#define NEBULA3_FRAME_LOG_ENABLED   (0)
#if NEBULA3_FRAME_LOG_ENABLED
#define FRAME_LOG(...) n_printf(__VA_ARGS__); n_printf("\n")
#else
#define FRAME_LOG(...)
#endif

//------------------------------------------------------------------------------
namespace Frame
{
class FrameBatch : public Core::RefCounted
{
    __DeclareClass(FrameBatch);
public:
    /// constructor
    FrameBatch();
    /// destructor
    virtual ~FrameBatch();
    /// discard the frame batch
    void Discard();
    /// render the batch
    void Render();

    /// set batch shader
    void SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd);
    /// get batch shader
    const Ptr<CoreGraphics::ShaderInstance>& GetShader() const;
    /// set batch type
    void SetType(CoreGraphics::BatchType::Code t);
    /// get batch type
    CoreGraphics::BatchType::Code GetType() const;
    /// set model node filter
    void SetNodeFilter(Models::ModelNodeType::Code f);
    /// get model node filter
    Models::ModelNodeType::Code GetNodeFilter() const;
    /// set lighting mode
    void SetLightingMode(LightingMode::Code c);
    /// get lighting mode
    LightingMode::Code GetLightingMode() const;
    /// set sorting mode
    void SetSortingMode(SortingMode::Code c);
    /// get sorting mode
    SortingMode::Code GetSortingMode() const;
    /// set shader features
    void SetShaderFeatures(CoreGraphics::ShaderFeature::Mask m);
    /// get shader features
    CoreGraphics::ShaderFeature::Mask GetShaderFeatures() const;

    /// add a shader variable instance to the frame batch
    void AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var);
    /// get number of shader variables
    SizeT GetNumVariables() const;
    /// get shader variable by index
    const Ptr<CoreGraphics::ShaderVariableInstance>& GetVariableByIndex(IndexT i) const;

#if NEBULA3_ENABLE_PROFILING
    /// add batch profiler
    void SetBatchDebugTimer(const Util::String& name);
#endif

private:
    /// actual rendering method
    void RenderBatch();

    Ptr<CoreGraphics::ShaderInstance> shader;
    CoreGraphics::BatchType::Code batchType;
    Models::ModelNodeType::Code nodeFilter;
    LightingMode::Code lightingMode;
    SortingMode::Code sortingMode;
    CoreGraphics::ShaderFeature::Mask shaderFeatures;
    Util::Array<Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariables;

    _declare_timer(debugTimer);
};

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetType(CoreGraphics::BatchType::Code t)
{
    this->batchType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::BatchType::Code
FrameBatch::GetType() const
{
    return this->batchType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd)
{
    this->shader = shd;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>&
FrameBatch::GetShader() const
{
    return this->shader;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetNodeFilter(Models::ModelNodeType::Code f)
{
    this->nodeFilter = f;
}

//------------------------------------------------------------------------------
/**
*/
inline Models::ModelNodeType::Code
FrameBatch::GetNodeFilter() const
{
    return this->nodeFilter;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetLightingMode(LightingMode::Code c)
{
    this->lightingMode = c;
}

//------------------------------------------------------------------------------
/**
*/
inline LightingMode::Code
FrameBatch::GetLightingMode() const
{
    return this->lightingMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetSortingMode(SortingMode::Code c)
{
    this->sortingMode = c;
}

//------------------------------------------------------------------------------
/**
*/
inline SortingMode::Code
FrameBatch::GetSortingMode() const
{
    return this->sortingMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var)
{
    this->shaderVariables.Append(var);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameBatch::GetNumVariables() const
{
    return this->shaderVariables.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariableInstance>&
FrameBatch::GetVariableByIndex(IndexT i) const
{
    return this->shaderVariables[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameBatch::SetShaderFeatures(CoreGraphics::ShaderFeature::Mask m)
{
    this->shaderFeatures = m;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::ShaderFeature::Mask
FrameBatch::GetShaderFeatures() const
{
    return this->shaderFeatures;
}

} // namespace Frame
//------------------------------------------------------------------------------

