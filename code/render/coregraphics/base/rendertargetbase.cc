//------------------------------------------------------------------------------
//  rendertargetbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/rendertarget.h"
#include "resources/resourcemanager.h"
#include "coregraphics/displaydevice.h"

namespace Base
{
__ImplementClass(Base::RenderTargetBase, 'RTGB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::RenderTargetBase() :
    batchType(BatchType::InvalidBatchType),
    width(0),
    height(0),
	relWidth(0.0f),
	relHeight(0.0f),
    resolveTextureDimensionsValid(false),
    resolveRectValid(false),
    resolveTextureWidth(0),
    resolveTextureHeight(0),
    resolveRect(0, 0, 0, 0),
    mrtIndex(0),
    clearFlags(0),
#if NEBULA3_DEBUG
    clearColor(1.0f, 0.0f, 1.0f, 0.0f),
#else // on release clear defaultrendertarget to black for nice loadingscreen fading
    clearColor(0.0f, 0.0f, 0.0f, 0.0f),
#endif
    clearDepth(1.0f),
    clearStencil(0),
    antiAliasQuality(AntiAliasQuality::None),
    colorBufferFormat(PixelFormat::X8R8G8B8),
    mipMapsEnabled(false),
    isValid(false),    
    inBeginPass(false),
    inBeginBatch(false),
    hasDepthStencilBuffer(false),
    isDefaultRenderTarget(false),
    resolveCpuAccess(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::~RenderTargetBase()
{
    n_assert(!this->isValid);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Setup()
{
    n_assert(!this->isValid);
	// recompute the relative width and height
	const DisplayMode& displayMode = DisplayDevice::Instance()->GetDisplayMode();
	if (0.0f != this->relWidth)
	{
		this->SetWidth(uint(float(displayMode.GetWidth()) * this->relWidth));
	}
	if (0.0f != this->relHeight)
	{
		this->SetHeight(uint(float(displayMode.GetHeight()) * this->relHeight));
	}
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Discard()
{
    n_assert(this->isValid);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->sharedDepthStencilBufferTarget = 0;
    if (this->resolveTexture.isvalid())
    {
        const Ptr<ResourceManager>& resManager = ResourceManager::Instance();
        if (resManager->HasResource(this->resolveTexture->GetResourceId()))
        {
            resManager->UnregisterUnmanagedResource(this->resolveTexture->GetResourceId());
        }
        this->resolveTexture = 0;
    }
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::AddDepthStencilBuffer()
{
    n_assert(!this->hasDepthStencilBuffer);
    this->hasDepthStencilBuffer = true;
    this->sharedDepthStencilBufferTarget = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::AddSharedDepthStencilBuffer(const Ptr<RenderTarget>& rt)
{
    n_assert(!this->hasDepthStencilBuffer);
    this->hasDepthStencilBuffer = true;
    this->sharedDepthStencilBufferTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginPass()
{
    n_assert(this->isValid);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginPass = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginBatch(BatchType::Code t)
{
    n_assert(this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginBatch = true;
    this->batchType = t;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndBatch()
{
    n_assert(this->inBeginBatch);
    this->inBeginBatch = false; 
    this->batchType = BatchType::InvalidBatchType;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndPass()
{
    n_assert(this->isValid);
    n_assert(this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->inBeginPass = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::GenerateMipLevels()
{
    n_assert(this->mipMapsEnabled);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::ResolveDepthBuffer()
{
    n_error("RenderTargetBase::ResolveDepthBuffer not implemented!");
}

//------------------------------------------------------------------------------
/**
*/
void 
RenderTargetBase::SetMemoryOffset(SizeT size)
{
    this->memoryOffset = size;            
}

//------------------------------------------------------------------------------
/**
*/
SizeT 
RenderTargetBase::GetMemorySize() const
{
    return 0;
}
} // namespace Base