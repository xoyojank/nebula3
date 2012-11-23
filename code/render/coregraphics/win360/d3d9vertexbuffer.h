#pragma once
//------------------------------------------------------------------------------
/** 
    @class Win360::D3D9VertexBuffer
  
    D3D9/Xbox360 implementation of VertexBuffer.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/vertexbufferbase.h"
#include "coregraphics/d3d9/d3d9resource.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9VertexBuffer : public Base::VertexBufferBase, public Direct3D9::D3D9Resource
{
    __DeclareClass(D3D9VertexBuffer);
public:
    /// constructor
    D3D9VertexBuffer();
    /// destructor
    virtual ~D3D9VertexBuffer();

	/// setup the d3d9 object
	void Setup();
	/// discard the d3d9 object
	void Discard();

    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map the vertices for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();

    /// get pointer to d3d9 vertex buffer object
    IDirect3DVertexBuffer9* GetD3D9VertexBuffer() const;

private:
	/// called when d3d9 device is lost
	virtual void OnLostDevice();
	/// called when d3d9 devcie is reset
	virtual void OnResetDevice();

private:
    IDirect3DVertexBuffer9* d3d9VertexBuffer;
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DVertexBuffer9*
D3D9VertexBuffer::GetD3D9VertexBuffer() const
{
    n_assert(0 != this->d3d9VertexBuffer);
    n_assert(0 == this->mapCount);
    return this->d3d9VertexBuffer;
}

} // namespace Win360
//------------------------------------------------------------------------------

