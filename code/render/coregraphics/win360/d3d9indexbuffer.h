#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9IndexBuffer
  
    D3D9/Xbox360 implementation of index buffer.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/indexbufferbase.h"
#include "coregraphics/d3d9/d3d9resource.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9IndexBuffer : public Base::IndexBufferBase, public Direct3D9::D3D9Resource
{
    __DeclareClass(D3D9IndexBuffer);
public:
    /// constructor
    D3D9IndexBuffer();
    /// destructor
    virtual ~D3D9IndexBuffer();
    
	/// setup the d3d9 object
	void Setup();
	/// discard the d3d9 object
	void Discard();

    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map index buffer for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();

    /// get d3d9 index buffer pointer
    IDirect3DIndexBuffer9* GetD3D9IndexBuffer() const;

private:
	/// called when d3d9 device is lost
	virtual void OnLostDevice();
	/// called when d3d9 devcie is reset
	virtual void OnResetDevice();

private:
    IDirect3DIndexBuffer9* d3d9IndexBuffer;
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DIndexBuffer9*
D3D9IndexBuffer::GetD3D9IndexBuffer() const
{
    n_assert(0 != this->d3d9IndexBuffer);
    n_assert(0 == this->mapCount);
    return this->d3d9IndexBuffer;
}

} // namespace Win360
//------------------------------------------------------------------------------
