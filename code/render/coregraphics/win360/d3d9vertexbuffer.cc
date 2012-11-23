//------------------------------------------------------------------------------
//  d3d9vertexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9vertexbuffer.h"
#include "coregraphics/win360/d3d9types.h"
#include "coregraphics/renderdevice.h"

namespace Win360
{
__ImplementClass(Win360::D3D9VertexBuffer, 'D9VB', Base::VertexBufferBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9VertexBuffer::D3D9VertexBuffer() :
    d3d9VertexBuffer(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9VertexBuffer::~D3D9VertexBuffer()
{
    n_assert(0 == this->d3d9VertexBuffer);
    n_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9VertexBuffer::Unload()
{
	this->Discard();
    VertexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
D3D9VertexBuffer::Map(MapType mapType)
{
    n_assert(0 != this->d3d9VertexBuffer);
    DWORD lockFlags = 0;
    switch (mapType)
    {
        case MapRead:
            // deactivated by Georg granted by Floh
            //n_assert(((UsageDynamic == this->usage) || (UsageCpu == this->usage)) && (AccessRead == this->access));
            break;

        case MapWrite:
            n_assert(((UsageDynamic == this->usage) || (UsageCpu == this->usage)) && (AccessWrite == this->access));
            break;

        case MapReadWrite:
            n_assert(((UsageDynamic == this->usage) || (UsageCpu == this->usage)) && (AccessReadWrite == this->access));
            break;

        case MapWriteDiscard:
            n_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            #if __WIN32__
            lockFlags |= D3DLOCK_DISCARD;
            #endif
            break;

        case MapWriteNoOverwrite:
            n_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            lockFlags |= D3DLOCK_NOOVERWRITE;
            break;
    }
    void* ptr = 0;
    HRESULT hr = this->d3d9VertexBuffer->Lock(0, 0, &ptr, lockFlags);
    n_assert(SUCCEEDED(hr));
    this->mapCount++;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9VertexBuffer::Unmap()
{
    n_assert(0 != this->d3d9VertexBuffer);
    n_assert(this->mapCount > 0);
    HRESULT hr = this->d3d9VertexBuffer->Unlock();
    n_assert(SUCCEEDED(hr));
    this->mapCount--;
}

//------------------------------------------------------------------------------
void
D3D9VertexBuffer::Setup()
{
	IDirect3DDevice9* d3d9Device = RenderDevice::Instance()->GetDirect3DDevice();
	n_assert(0 != d3d9Device);
	// create a d3d9 vertex buffer object
	DWORD d3dVertexBufferSize = this->numVertices * this->vertexLayout->GetVertexByteSize();
	D3DPOOL d3dPool     = D3D9Types::AsD3D9Pool(this->usage, this->access);
	DWORD d3dUsage      = D3D9Types::AsD3D9Usage(this->usage, this->access);
	IDirect3DVertexBuffer9* d3dVertexBuffer = 0;
	HRESULT hr = d3d9Device->CreateVertexBuffer(
	                 d3dVertexBufferSize,        // Length
	                 d3dUsage,                   // Usage
	                 0,                          // FVF
	                 d3dPool,                    // Pool
	                 &this->d3d9VertexBuffer,    // ppVertexBuffer
	                 NULL);                      // pSharedHandle
	n_assert(SUCCEEDED(hr));
	n_assert(0 != this->d3d9VertexBuffer);
	// add to handler if it is default pool
	if (D3DPOOL_DEFAULT == d3dPool)
	{
		this->AddToResourceEventHandler();
	}
}

//------------------------------------------------------------------------------
void
D3D9VertexBuffer::Discard()
{
	n_assert(0 == this->mapCount);
	if (0 != this->d3d9VertexBuffer)
	{
		this->d3d9VertexBuffer->Release();
		this->d3d9VertexBuffer = 0;
	}
}

//------------------------------------------------------------------------------
void
D3D9VertexBuffer::OnLostDevice()
{
	if (!this->isLosted)
	{
		this->Discard();
		this->isLosted = true;
	}
}

//------------------------------------------------------------------------------
void
D3D9VertexBuffer::OnResetDevice()
{
	if (this->isLosted)
	{
		this->Setup();
		this->isLosted = false;
	}
}

} // namespace Win360
