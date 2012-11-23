//------------------------------------------------------------------------------
//  renderdevicebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/renderdevicebase.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/multiplerendertarget.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/shaderinstance.h"

namespace Base
{
__ImplementClass(Base::RenderDeviceBase, 'RNDB', Core::RefCounted);
__ImplementSingleton(Base::RenderDeviceBase);

using namespace Util;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
RenderDeviceBase::RenderDeviceBase() :
    isOpen(false),
    inNotifyEventHandlers(false),
    inBeginFrame(false),
    inBeginPass(false),
    inBeginBatch(false),
    visualizeMipMaps(false)
{
    __ConstructSingleton;
    _setup_counter(RenderDeviceNumPrimitives);
    _setup_counter(RenderDeviceNumDrawCalls);
    Memory::Clear(this->streamVertexOffsets, sizeof(this->streamVertexOffsets));
}

//------------------------------------------------------------------------------
/**
*/
RenderDeviceBase::~RenderDeviceBase()
{
    n_assert(!this->IsOpen());
    n_assert(this->eventHandlers.IsEmpty());

    _discard_counter(RenderDeviceNumPrimitives);
    _discard_counter(RenderDeviceNumDrawCalls);
    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    This static method can be used to check whether a RenderDevice 
    object can be created on this machine before actually instantiating
    the device object (for instance by checking whether the right Direct3D
    version is installed). Use this method at application startup
    to check if the application should run at all.
*/
bool
RenderDeviceBase::CanCreate()
{
    return false;
}

//------------------------------------------------------------------------------
/**
    Override the default render target (which is normally created in Open())
    with a render target provided by the application, this is normally only
    useful for debugging and testing purposes.
*/
void
RenderDeviceBase::SetOverrideDefaultRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    n_assert(!this->isOpen);
    n_assert(!this->defaultRenderTarget.isvalid());
    this->defaultRenderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderDeviceBase::Open()
{
    n_assert(!this->IsOpen());
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    this->isOpen = true;

    // notify event handlers
    RenderEvent openEvent(RenderEvent::DeviceOpen);
    this->NotifyEventHandlers(openEvent);

    // create default render target (if not overriden by application
    if (!this->defaultRenderTarget.isvalid())
    {
        this->defaultRenderTarget = RenderTarget::Create();
        this->defaultRenderTarget->SetDefaultRenderTarget(true);
        this->defaultRenderTarget->Setup();
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::Close()
{
    n_assert(this->IsOpen());
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);

    // release default render target
    if (this->defaultRenderTarget->IsValid())
    {
        this->defaultRenderTarget->Discard();
    }
    this->defaultRenderTarget = 0;

    // notify event handlers
    RenderEvent closeEvent(RenderEvent::DeviceClose);
    this->NotifyEventHandlers(closeEvent);

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderDeviceBase::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::SetStreamSource(IndexT streamIndex, const Ptr<VertexBuffer>& vb, IndexT offsetVertexIndex)
{
    n_assert((streamIndex >= 0) && (streamIndex < MaxNumVertexStreams));
    this->streamVertexBuffers[streamIndex] = vb;
    this->streamVertexOffsets[streamIndex] = offsetVertexIndex;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<VertexBuffer>&
RenderDeviceBase::GetStreamVertexBuffer(IndexT streamIndex) const
{
    n_assert((streamIndex >= 0) && (streamIndex < MaxNumVertexStreams));
    return this->streamVertexBuffers[streamIndex];
}

//------------------------------------------------------------------------------
/**
*/
IndexT
RenderDeviceBase::GetStreamVertexOffset(IndexT streamIndex) const
{
    n_assert((streamIndex >= 0) && (streamIndex < MaxNumVertexStreams));
    return this->streamVertexOffsets[streamIndex];
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::SetVertexLayout(const Ptr<VertexLayout>& vl)
{
    this->vertexLayout = vl;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<VertexLayout>&
RenderDeviceBase::GetVertexLayout() const
{
    return this->vertexLayout;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::SetIndexBuffer(const Ptr<IndexBuffer>& ib)
{
    this->indexBuffer = ib;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<IndexBuffer>&
RenderDeviceBase::GetIndexBuffer() const
{
    return this->indexBuffer;
}

//------------------------------------------------------------------------------
/**
    Attach an event handler to the render device.
*/
void
RenderDeviceBase::AttachEventHandler(const Ptr<RenderEventHandler>& handler)
{
    n_assert(handler.isvalid());
    n_assert(InvalidIndex == this->eventHandlers.FindIndex(handler));
    n_assert(!this->inNotifyEventHandlers);
    this->eventHandlers.Append(handler);
    handler->OnAttach();
}

//------------------------------------------------------------------------------
/**
    Remove an event handler from the display device.
*/
void
RenderDeviceBase::RemoveEventHandler(const Ptr<RenderEventHandler>& handler)
{
    n_assert(handler.isvalid());
    n_assert(!this->inNotifyEventHandlers);
    IndexT index = this->eventHandlers.FindIndex(handler);
    n_assert(InvalidIndex != index);
    this->eventHandlers.EraseIndex(index);
    handler->OnRemove();
}

//------------------------------------------------------------------------------
/**
    Notify all event handlers about an event.
*/
bool
RenderDeviceBase::NotifyEventHandlers(const RenderEvent& event)
{
    n_assert(!this->inNotifyEventHandlers);
    bool handled = false;
    this->inNotifyEventHandlers = true;
    IndexT i;
    for (i = 0; i < this->eventHandlers.Size(); i++)
    {
        handled |= this->eventHandlers[i]->PutEvent(event);
    }
    this->inNotifyEventHandlers = false;
    return handled;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderDeviceBase::BeginFrame()
{
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    n_assert(!this->indexBuffer.isvalid());
    n_assert(!this->vertexLayout.isvalid());
    IndexT i;
    for (i = 0; i < MaxNumVertexStreams; i++)
    {
        n_assert(!this->streamVertexBuffers[i].isvalid());
    }

    _begin_counter(RenderDeviceNumPrimitives);
    _begin_counter(RenderDeviceNumDrawCalls);

    this->inBeginFrame = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::BeginPass(const Ptr<RenderTarget>& rt, const Ptr<ShaderInstance>& shd)
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    n_assert(!this->passShader.isvalid());
    this->inBeginPass = true;

    // apply pass shader
    this->passShader = shd;
    if (this->passShader.isvalid())
    {
        SizeT numPasses = this->passShader->Begin();
        n_assert(1 == numPasses);
        this->passShader->BeginPass(0);
        this->passShader->Commit();
    }

    // notify render targets
    this->passRenderTarget = rt;
    this->passRenderTarget->BeginPass();    
}

//------------------------------------------------------------------------------
/**
*/
void 
RenderDeviceBase::BeginPass(const Ptr<CoreGraphics::MultipleRenderTarget>& mrt, const Ptr<CoreGraphics::ShaderInstance>& shd)
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginPass);
    n_assert(!this->inBeginBatch);
    n_assert(!this->passShader.isvalid());
    this->inBeginPass = true;

    // apply pass shader
    this->passShader = shd;
    if (this->passShader.isvalid())
    {
        SizeT numPasses = this->passShader->Begin();
        n_assert(1 == numPasses);
        this->passShader->BeginPass(0);
        this->passShader->Commit();
    }

    // notify render targets
    this->multipleRenderTarget = mrt;
    this->multipleRenderTarget->BeginPass();    
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::BeginBatch(BatchType::Code batchType, const Ptr<ShaderInstance>& shd)
{
    n_assert(this->inBeginPass);
    n_assert(!this->inBeginBatch);
    n_assert(!this->batchShader.isvalid());
    n_assert(this->passRenderTarget.isvalid() ||this->multipleRenderTarget.isvalid());
    n_assert(shd.isvalid());
    this->inBeginBatch = true;

    // apply batch shader
    this->batchShader = shd;
    SizeT numPasses = this->batchShader->Begin();
    n_assert(1 == numPasses);
    this->batchShader->BeginPass(0);
    this->batchShader->Commit();

    if (this->passRenderTarget.isvalid())
    {
        // notify render target
        this->passRenderTarget->BeginBatch(batchType);    
    }
    else if (this->multipleRenderTarget.isvalid())
    {
        // notify multiple render target
        this->multipleRenderTarget->BeginBatch(batchType);    
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::EndBatch()
{
    n_assert(this->inBeginBatch);
    n_assert(this->passRenderTarget.isvalid() ||this->multipleRenderTarget.isvalid());
    this->inBeginBatch = false;

    // notify render targets
    if (this->passRenderTarget.isvalid())
    {
        this->passRenderTarget->EndBatch();
    }
    else if (this->multipleRenderTarget.isvalid())
    {
        this->multipleRenderTarget->EndBatch();
    }
    
    // finish batch shader
    this->batchShader->EndPass();
    this->batchShader->End();
    this->batchShader = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::EndPass()
{
    n_assert(this->inBeginPass);
    n_assert(this->passRenderTarget.isvalid() ||this->multipleRenderTarget.isvalid());

    // finish rendering to render target
    if (this->passRenderTarget.isvalid())
    {        
        this->passRenderTarget->EndPass();
        this->passRenderTarget = 0;
    }
    else if (this->multipleRenderTarget.isvalid())
    {
        this->multipleRenderTarget->EndPass();
        this->multipleRenderTarget = 0;
    }

    // finish pass shader
    if (this->passShader.isvalid())
    {
        this->passShader->EndPass();
        this->passShader->End();
        this->passShader = 0;
    }
    this->inBeginPass = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::EndFrame()
{
    n_assert(this->inBeginFrame);

    _end_counter(RenderDeviceNumPrimitives);
    _end_counter(RenderDeviceNumDrawCalls);
    
    this->inBeginFrame = false;
    IndexT i;
    for (i = 0; i < MaxNumVertexStreams; i++)
    {
        this->streamVertexBuffers[i] = 0;
    }
    this->vertexLayout = 0;
    this->indexBuffer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::Present()
{
    n_assert(!this->inBeginFrame);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::Draw()
{
    n_assert(this->inBeginPass);
    // override in subclass!
}

//------------------------------------------------------------------------------
/**
*/
void
RenderDeviceBase::DrawIndexedInstanced(SizeT numInstances)
{
    n_assert(this->inBeginPass);
    // override in subclass!
}

//------------------------------------------------------------------------------
/**
*/
ImageFileFormat::Code
RenderDeviceBase::SaveScreenshot(ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream)
{
    // override in subclass!
    return fmt;    
}

} // namespace Base
