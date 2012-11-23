//------------------------------------------------------------------------------
//  frameposteffect.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameposteffect.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shadersemantics.h"

namespace Frame
{
__ImplementClass(Frame::FramePostEffect, 'FPEF', Frame::FramePassBase);

using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePostEffect::FramePostEffect()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePostEffect::~FramePostEffect()
{     
    // empty
}

//------------------------------------------------------------------------------
/**
    This setsup the quad mesh for rendering the fullscreen quad.
*/
void
FramePostEffect::Setup()
{
    n_assert(this->renderTarget.isvalid());
                                                
    // setup the fullscreen quad renderer      
    SizeT w = this->renderTarget->GetWidth();
    SizeT h = this->renderTarget->GetHeight();
    this->drawFullScreenQuad.Setup(w, h);
}

//------------------------------------------------------------------------------
/**
*/
void
FramePostEffect::Discard()
{
    FramePassBase::Discard();
    this->drawFullScreenQuad.Discard();
}

//------------------------------------------------------------------------------
/**
*/
void
FramePostEffect::Render()
{
    n_assert(this->renderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    // update render target
    // @todo: hmm... should be able to define whether a clear is necessary or not!    
    if (this->renderTarget.isvalid())
    {
        // update render targets
        this->renderTarget->SetClearColor(Math::float4(0.0f, 0.0f, 0.0f, 0.0f));
        this->renderTarget->SetClearDepth(COREGRAPHICS_STANDARDCLEARDEPTHVALUE);
        this->renderTarget->SetClearStencil(0);  
    }

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // first render fullscreen quad, then any render batches 
    if (this->renderTarget.isvalid())
    {
        renderDevice->BeginPass(this->renderTarget, this->shader);
    }
    else
    {
        renderDevice->BeginPass(this->multipleRenderTarget, this->shader);
    }     
    this->drawFullScreenQuad.Draw();
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.Size(); batchIndex++)
    {
        this->batches[batchIndex]->Render();
    }
    renderDevice->EndPass();
}

} // namespace Frame
