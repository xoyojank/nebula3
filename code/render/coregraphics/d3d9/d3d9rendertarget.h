#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9RenderTarget
  
    D3D9 implementation of RenderTarget.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/rendertargetbase.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/d3d9/d3d9resource.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9RenderTarget : public Base::RenderTargetBase, public D3D9Resource
{
    __DeclareClass(D3D9RenderTarget);
public:
    /// constructor
    D3D9RenderTarget();
    /// destructor
    virtual ~D3D9RenderTarget();
    
    /// setup the render target object
    void Setup();
    /// discard the render target object
    void Discard();
    /// begin a render pass
    void BeginPass();
    /// end current render pass
    void EndPass();
    /// generate mipmap levels
    void GenerateMipLevels();

private:
	/// called when d3d9 device is lost
	virtual void OnLostDevice();
	/// called when d3d9 device is reset
	virtual void OnResetDevice();

    /// setup compatible multisample type
    void SetupMultiSampleType();
                                                      
private:
    friend class D3D9RenderDevice;
                                                      
    Ptr<CoreGraphics::ShaderVariable> sharedPixelSize; 
    Ptr<CoreGraphics::ShaderVariable> sharedHalfPixelSize;
    IDirect3DSurface9* d3d9RenderTarget;
    IDirect3DSurface9* d3d9DepthStencil;
    IDirect3DTexture9* d3d9ResolveTexture;
    IDirect3DTexture9* d3d9CPUResolveTexture;
    D3DMULTISAMPLE_TYPE d3d9MultiSampleType;
    DWORD d3d9MultiSampleQuality;
    D3DFORMAT d3d9ColorBufferFormat;
    bool needsResolve;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
