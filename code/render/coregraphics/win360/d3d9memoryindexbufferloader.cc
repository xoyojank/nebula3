//------------------------------------------------------------------------------
//  d3d9memoryindexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9memoryindexbufferloader.h"
#include "coregraphics/indexbuffer.h"

namespace Win360
{
__ImplementClass(Win360::D3D9MemoryIndexBufferLoader, 'DMIL', Base::MemoryIndexBufferLoaderBase);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 IndexBuffer using the data provided by our
    Setup() method and set our resource object (which must be a
    D3D9IndexBuffer object).
*/
bool
D3D9MemoryIndexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    n_assert(this->indexType != IndexType::None);
    n_assert(this->numIndices > 0);
    if (IndexBuffer::UsageImmutable == this->usage)
    {
        n_assert(this->indexDataSize == (this->numIndices * IndexType::SizeOf(this->indexType)));
        n_assert(0 != this->indexDataPtr);
        n_assert(0 < this->indexDataSize);
    }

	// create a D3D9 index buffer object
	const Ptr<IndexBuffer>& res = this->resource.downcast<IndexBuffer>();
	n_assert(!res->IsLoaded());
	res->SetUsage(this->usage);
	res->SetAccess(this->access);
	res->SetIndexType(this->indexType);
	res->SetNumIndices(this->numIndices);
	res->Setup();
    IDirect3DIndexBuffer9* d3dIndexBuffer = res->GetD3D9IndexBuffer();

	// setup initial data if provided
	if (0 != this->indexDataPtr)
	{
		// copy data to d3d9 index buffer
		void* dstPtr = 0;
		HRESULT hr = d3dIndexBuffer->Lock(0, 0, &dstPtr, D3DLOCK_NOSYSLOCK);
		n_assert(SUCCEEDED(hr));
		n_assert(0 != dstPtr);
		Memory::CopyToGraphicsMemory(this->indexDataPtr, dstPtr, this->indexDataSize);
		hr = d3dIndexBuffer->Unlock();
		n_assert(SUCCEEDED(hr));
	}

    // invalidate setup data (because we don't own our data)
    this->indexDataPtr = 0;
    this->indexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace Win360
