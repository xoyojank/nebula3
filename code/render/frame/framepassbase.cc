//------------------------------------------------------------------------------
//  framepassbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framepassbase.h"
#include "coregraphics/renderdevice.h"

namespace Frame
{
__ImplementClass(Frame::FramePassBase, 'FPSB', Core::RefCounted);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePassBase::FramePassBase() :
    clearFlags(0),
    clearDepth(1.0f),
    clearStencil(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePassBase::~FramePassBase()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(!this->renderTarget.isvalid());
    n_assert(!this->multipleRenderTarget.isvalid());
    n_assert(this->shaderVariables.IsEmpty());  
    n_assert(this->batches.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FramePassBase::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    if (this->renderTarget.isvalid())
    {
        if (this->renderTarget->IsValid())
        {
            this->renderTarget->Discard();
        }
        this->renderTarget = 0;
    }
    if (this->multipleRenderTarget.isvalid())
    {
        this->multipleRenderTarget = 0;
    }
    this->shaderVariables.Clear(); 

    IndexT i;
    for (i = 0; i < this->batches.Size(); i++)
    {
        this->batches[i]->Discard();
    }
    this->batches.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
FramePassBase::Render()
{
    // implement in base class    
}

} // namespace Frame
