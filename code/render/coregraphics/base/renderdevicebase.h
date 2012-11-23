#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::RenderDeviceBase
  
    The central rendering object of the Nebula3 core graphics system. This
    is basically an encapsulation of the Direct3D device. The render device
    will presents its backbuffer to the display managed by the
    CoreGraphics::DisplayDevice singleton.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coregraphics/rendereventhandler.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/batchtype.h"
#include "coregraphics/imagefileformat.h"
#include "io/stream.h"
#include "debug/debugcounter.h"

namespace CoreGraphics
{
class Texture;
class VertexBuffer;
class IndexBuffer;
class VertexLayout;
class ShaderInstance;
class RenderTarget;
class MultipleRenderTarget;
};

//------------------------------------------------------------------------------
namespace Base
{
class RenderDeviceBase : public Core::RefCounted
{
    __DeclareClass(RenderDeviceBase);
    __DeclareSingleton(RenderDeviceBase);
public:
    /// max number of vertex streams
    static const IndexT MaxNumVertexStreams = 2;

    /// constructor
    RenderDeviceBase();
    /// destructor
    virtual ~RenderDeviceBase();
    /// test if a compatible render device can be created on this machine
    static bool CanCreate();

    /// set an override for the default render target (call before Open()!)
    void SetOverrideDefaultRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);

    /// open the device
    bool Open();
    /// close the device
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// attach a render event handler
    void AttachEventHandler(const Ptr<CoreGraphics::RenderEventHandler>& h);
    /// remove a render event handler
    void RemoveEventHandler(const Ptr<CoreGraphics::RenderEventHandler>& h);

    /// get default render target
    const Ptr<CoreGraphics::RenderTarget>& GetDefaultRenderTarget() const;   
    /// is a pass rendertarget set?
    bool HasPassRenderTarget() const;
    /// get current set pass render target
    const Ptr<CoreGraphics::RenderTarget>& GetPassRenderTarget() const;

    /// begin complete frame
    bool BeginFrame();
    /// begin rendering a frame pass
    void BeginPass(const Ptr<CoreGraphics::RenderTarget>& rt, const Ptr<CoreGraphics::ShaderInstance>& passShader);
    /// begin rendering a frame pass with a multiple rendertarget
    void BeginPass(const Ptr<CoreGraphics::MultipleRenderTarget>& mrt, const Ptr<CoreGraphics::ShaderInstance>& passShader);
    /// begin rendering a batch inside 
    void BeginBatch(CoreGraphics::BatchType::Code batchType, const Ptr<CoreGraphics::ShaderInstance>& batchShader);
    /// set the current vertex stream source
    void SetStreamSource(IndexT streamIndex, const Ptr<CoreGraphics::VertexBuffer>& vb, IndexT offsetVertexIndex);
    /// get currently set vertex buffer
    const Ptr<CoreGraphics::VertexBuffer>& GetStreamVertexBuffer(IndexT streamIndex) const;
    /// get currently set vertex stream offset
    IndexT GetStreamVertexOffset(IndexT streamIndex) const;
    /// set current vertex layout
    void SetVertexLayout(const Ptr<CoreGraphics::VertexLayout>& vl);
    /// get current vertex layout
    const Ptr<CoreGraphics::VertexLayout>& GetVertexLayout() const;
    /// set current index buffer
    void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
    /// get current index buffer
    const Ptr<CoreGraphics::IndexBuffer>& GetIndexBuffer() const;
    /// set current primitive group
    void SetPrimitiveGroup(const CoreGraphics::PrimitiveGroup& pg);
    /// get current primitive group
    const CoreGraphics::PrimitiveGroup& GetPrimitiveGroup() const;
    /// draw current primitives
    void Draw();
    /// draw indexed, instanced primitives
    void DrawIndexedInstanced(SizeT numInstances);
    /// end current batch
    void EndBatch();
    /// end current pass
    void EndPass();
    /// end current frame
    void EndFrame();
    /// check if inside BeginFrame
    bool IsInBeginFrame() const;
    /// present the rendered scene
    void Present();
    /// save a screenshot to the provided stream
    CoreGraphics::ImageFileFormat::Code SaveScreenshot(CoreGraphics::ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream);
    /// get visualization of mipmaps flag	
    bool GetVisualizeMipMaps() const;
    /// set visualization of mipmaps flag	
    void SetVisualizeMipMaps(bool val);

protected:
    /// notify event handlers about an event
    bool NotifyEventHandlers(const CoreGraphics::RenderEvent& e);
    
    Util::Array<Ptr<CoreGraphics::RenderEventHandler> > eventHandlers;
    Ptr<CoreGraphics::RenderTarget> defaultRenderTarget;
    Ptr<CoreGraphics::VertexBuffer> streamVertexBuffers[MaxNumVertexStreams];
    IndexT streamVertexOffsets[MaxNumVertexStreams];
    Ptr<CoreGraphics::VertexLayout> vertexLayout;
    Ptr<CoreGraphics::IndexBuffer> indexBuffer;
    CoreGraphics::PrimitiveGroup primitiveGroup;
    Ptr<CoreGraphics::RenderTarget> passRenderTarget;
    Ptr<CoreGraphics::MultipleRenderTarget> multipleRenderTarget;
    Ptr<CoreGraphics::ShaderInstance> passShader;
    Ptr<CoreGraphics::ShaderInstance> batchShader;
    bool isOpen;
    bool inNotifyEventHandlers;
    bool inBeginFrame;
    bool inBeginPass;
    bool inBeginBatch;
    bool visualizeMipMaps;

    _declare_counter(RenderDeviceNumPrimitives);
    _declare_counter(RenderDeviceNumDrawCalls);
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderDeviceBase::SetPrimitiveGroup(const CoreGraphics::PrimitiveGroup& pg)
{
    this->primitiveGroup = pg;
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::PrimitiveGroup&
RenderDeviceBase::GetPrimitiveGroup() const
{
    return this->primitiveGroup;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
RenderDeviceBase::GetDefaultRenderTarget() const
{
    n_assert(this->defaultRenderTarget.isvalid());
    return this->defaultRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RenderDeviceBase::HasPassRenderTarget() const
{
    return this->passRenderTarget.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
RenderDeviceBase::GetPassRenderTarget() const
{
    n_assert(this->passRenderTarget.isvalid());
    return this->passRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RenderDeviceBase::IsInBeginFrame() const
{
    return this->inBeginFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RenderDeviceBase::GetVisualizeMipMaps() const
{
    return this->visualizeMipMaps;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
RenderDeviceBase::SetVisualizeMipMaps(bool val)
{
    this->visualizeMipMaps = val;
}

} // namespace Base
//------------------------------------------------------------------------------

