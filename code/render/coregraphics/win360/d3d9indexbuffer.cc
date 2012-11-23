//------------------------------------------------------------------------------
//  d3d9indexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9indexbuffer.h"
#include "coregraphics/win360/d3d9types.h"
#include "coregraphics/renderdevice.h"

namespace Win360
{
__ImplementClass(Win360::D3D9IndexBuffer, 'D9IB', Base::IndexBufferBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9IndexBuffer::D3D9IndexBuffer() :
    d3d9IndexBuffer(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9IndexBuffer::~D3D9IndexBuffer()
{
    n_assert(0 == this->d3d9IndexBuffer);
    n_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9IndexBuffer::Unload()
{
	this->Discard();
    IndexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
D3D9IndexBuffer::Map(MapType mapType)
{
    n_assert(0 != this->d3d9IndexBuffer);
    DWORD lockFlags = 0;
    switch (mapType)
    {
        case MapRead:
            n_assert(((UsageDynamic == this->usage) || (UsageCpu == this->usage)) && (AccessRead == this->access));
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
    HRESULT hr = this->d3d9IndexBuffer->Lock(0, 0, &ptr, lockFlags);
    n_assert(SUCCEEDED(hr));
    this->mapCount++;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9IndexBuffer::Unmap()
{
    n_assert(0 != this->d3d9IndexBuffer);
    n_assert(this->mapCount > 0);
    HRESULT hr = this->d3d9IndexBuffer->Unlock();
    n_assert(SUCCEEDED(hr));
    this->mapCount--;
}

//------------------------------------------------------------------------------
void
D3D9IndexBuffer::OnLostDevice()
{
	if (!this->isLosted)
	{
		this->Discard();
		this->isLosted = true;
	}
}

//------------------------------------------------------------------------------
void
D3D9IndexBuffer::OnResetDevice()
{
	if (this->isLosted)
	{
		this->Setup();
		this->isLosted = false;
	}
}

//------------------------------------------------------------------------------
void
D3D9IndexBuffer::Setup()
{
	IDirect3DDevice9* d3d9Device = RenderDevice::Instance()->GetDirect3DDevice();
	n_assert(0 != d3d9Device);
	// create a D3D9 index buffer object
	DWORD d3dIndexBufferSize = this->numIndices * IndexType::SizeOf(this->indexType);
	D3DPOOL d3dPool     = D3D9Types::AsD3D9Pool(this->usage, this->access);
	DWORD d3dUsage      = D3D9Types::AsD3D9Usage(this->usage, this->access);
	D3DFORMAT d3dFormat = D3D9Types::IndexTypeAsD3D9Format(this->indexType);
	HRESULT hr = d3d9Device->CreateIndexBuffer(
	                 d3dIndexBufferSize,      // Length,
	                 d3dUsage,                // Usage
	                 d3dFormat,               // Format
	                 d3dPool,                 // Pool
	                 &this->d3d9IndexBuffer,  // ppIndexBuffer
	                 NULL);                   // pSharedHandle
	n_assert(SUCCEEDED(hr));
	n_assert(0 != this->d3d9IndexBuffer);
	// add to handler if it is default pool
	if (D3DPOOL_DEFAULT == d3dPool)
	{
		this->AddToResourceEventHandler();
	}
}

//------------------------------------------------------------------------------
void
D3D9IndexBuffer::Discard()
{
	n_assert(0 == this->mapCount);
	if (0 != this->d3d9IndexBuffer)
	{
		this->d3d9IndexBuffer->Release();
		this->d3d9IndexBuffer = 0;
	}
}

} // namespace CoreGraphics
