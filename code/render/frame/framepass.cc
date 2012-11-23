//------------------------------------------------------------------------------
//  framepass.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framepass.h"
#include "coregraphics/renderdevice.h"

namespace Frame
{
__ImplementClass(Frame::FramePass, 'FPSS', Frame::FramePassBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePass::FramePass()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePass::~FramePass()
{
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Discard()
{
    FramePassBase::Discard();
    _discard_timer(debugTimer);
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Render()
{
    _start_timer(this->debugTimer);

    n_assert(this->renderTarget.isvalid() ||this->multipleRenderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    if (this->renderTarget.isvalid())
    {
        n_assert(!this->multipleRenderTarget.isvalid());
        // update render targets
        this->renderTarget->SetClearFlags(this->clearFlags);
        this->renderTarget->SetClearColor(this->clearColor);
        this->renderTarget->SetClearDepth(this->clearDepth);
        this->renderTarget->SetClearStencil(this->clearStencil);  
    }  
    else if (this->multipleRenderTarget.isvalid())
    {
        n_assert(!this->renderTarget.isvalid());
        // FIXME!???
    }

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render batches...
    if (this->renderTarget.isvalid())
    {
        renderDevice->BeginPass(this->renderTarget, this->shader);
    }
    else
    {
        renderDevice->BeginPass(this->multipleRenderTarget, this->shader);
    }
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.Size(); batchIndex++)
    {
        FRAME_LOG("    FramePass::Render() %s  batch: %d", this->GetName().AsString().AsCharPtr(), batchIndex);
        this->batches[batchIndex]->Render();
        FRAME_LOG(" ");
    }
    renderDevice->EndPass();

    _stop_timer(this->debugTimer);
}

//------------------------------------------------------------------------------
/**
*/
#if NEBULA3_ENABLE_PROFILING
void 
FramePass::SetFramePassDebugTimer(const Util::String& name)
{
    this->debugTimer = Debug::DebugTimer::Create();
    this->debugTimer->Setup(name);
}
#endif
} // namespace Frame
