//------------------------------------------------------------------------------
//  d3d9memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayoutserver.h"
#include "coregraphics/win360/d3d9memoryvertexbufferloader.h"
#include "coregraphics/vertexbuffer.h"

namespace Win360
{
__ImplementClass(Win360::D3D9MemoryVertexBufferLoader, 'DMVL', Base::MemoryVertexBufferLoaderBase);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 vertex buffer and vertex declaration object
    from the data provided in the Setup() method and setup our resource
    object (which must be a D3D9VertexBuffer object).
*/
bool
D3D9MemoryVertexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    n_assert(this->numVertices > 0);
    if (VertexBuffer::UsageImmutable == this->usage)
    {
        n_assert(0 != this->vertexDataPtr);
        n_assert(0 < this->vertexDataSize);
    }
    
    // first setup the vertex layout (contains the D3D9 vertex declaration)
    Ptr<VertexLayout> vertexLayout = VertexLayoutServer::Instance()->CreateSharedVertexLayout(this->vertexComponents);
    if (0 != this->vertexDataPtr)
    {
        n_assert((this->numVertices * vertexLayout->GetVertexByteSize()) == this->vertexDataSize);
    }

    // create a d3d9 vertex buffer object
	const Ptr<VertexBuffer>& res = this->resource.downcast<VertexBuffer>();
	n_assert(!res->IsLoaded());
	res->SetUsage(this->usage);
	res->SetAccess(this->access);
	res->SetVertexLayout(vertexLayout);
	res->SetNumVertices(this->numVertices);
	res->Setup();
    IDirect3DVertexBuffer9* d3dVertexBuffer = res->GetD3D9VertexBuffer();

	// setup initial data if provided
	if (0 != this->vertexDataPtr)
	{
		// copy vertex data to vertex buffer
		void* dstPtr = 0;
		HRESULT hr = d3dVertexBuffer->Lock(0, 0, &dstPtr, D3DLOCK_NOSYSLOCK);
		n_assert(SUCCEEDED(hr));
		n_assert(0 != dstPtr);
		Memory::CopyToGraphicsMemory(this->vertexDataPtr, dstPtr, this->vertexDataSize);
		hr = d3dVertexBuffer->Unlock();
		n_assert(SUCCEEDED(hr));
	}

    // invalidate setup data (because we don't own our data)
    this->vertexDataPtr = 0;
    this->vertexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace Win360
