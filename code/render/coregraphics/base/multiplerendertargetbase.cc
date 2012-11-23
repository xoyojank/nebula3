//------------------------------------------------------------------------------
//  multiplerendertargetbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/multiplerendertargetbase.h"

namespace Base
{
__ImplementClass(Base::MultipleRenderTargetBase, 'MRTB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
MultipleRenderTargetBase::MultipleRenderTargetBase():
    clearDepthStencil(false),
    clearDepth(1.0f),
    clearStencil(0),
    numRenderTargets(0)
{
    IndexT i;
    for (i = 0; i < MaxNumRenderTargets; i++)
    {
        this->clearColor[i].set(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

//------------------------------------------------------------------------------
/**
*/
MultipleRenderTargetBase::~MultipleRenderTargetBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::AddRenderTarget(const Ptr<RenderTarget>& rt)
{
    n_assert(rt.isvalid());
    n_assert(this->numRenderTargets < MaxNumRenderTargets);
    this->renderTarget[this->numRenderTargets] = rt;
    this->renderTarget[this->numRenderTargets]->SetMRTIndex(this->numRenderTargets);
    this->numRenderTargets++;
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::SetClearColor(IndexT i, const Math::float4& color)
{
    n_assert(i < this->numRenderTargets);
    this->clearColor[i] = color;
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::BeginPass()
{
    if (this->clearDepthStencil)
    {
        uint clearFlags = RenderTarget::ClearDepth | RenderTarget::ClearStencil; 
        this->renderTarget[0]->SetClearFlags(clearFlags);
        this->renderTarget[0]->SetClearDepth(this->GetClearDepth());
        this->renderTarget[0]->SetClearStencil(this->GetClearStencil());
    }
    
    IndexT i;
    for (i = 0; i < this->numRenderTargets; i++)
    {
        uint clearFlags = this->renderTarget[i]->GetClearFlags();
        clearFlags |= RenderTarget::ClearColor; 
        this->renderTarget[i]->SetClearFlags(clearFlags);
        this->renderTarget[i]->SetClearColor(this->clearColor[i]);
        this->renderTarget[i]->BeginPass();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::BeginBatch(CoreGraphics::BatchType::Code batchType)
{
    IndexT i;
    for (i = 0; i < this->numRenderTargets; i++)
    {
        this->renderTarget[i]->BeginBatch(batchType);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::EndBatch()
{
    IndexT i;
    for (i = 0; i < this->numRenderTargets; i++)
    {
        this->renderTarget[i]->EndBatch();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MultipleRenderTargetBase::EndPass()
{
    IndexT i;
    for (i = 0; i < this->numRenderTargets; i++)
    {
        this->renderTarget[i]->EndPass();
    }
}
} // namespace Base