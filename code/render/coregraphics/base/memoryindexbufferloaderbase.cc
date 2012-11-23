//------------------------------------------------------------------------------
//  memoryindexbufferloaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/memoryindexbufferloaderbase.h"

namespace Base
{
__ImplementClass(Base::MemoryIndexBufferLoaderBase, 'MILB', Resources::ResourceLoader);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
MemoryIndexBufferLoaderBase::MemoryIndexBufferLoaderBase() :
    indexType(IndexType::None),
    numIndices(0),
    indexDataPtr(0),
    indexDataSize(0),
    usage(IndexBuffer::UsageImmutable),
    access(IndexBuffer::AccessNone)
{
	
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the IndexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).

*/
void
MemoryIndexBufferLoaderBase::Setup(IndexType::Code indexType_, 
                                   SizeT numIndices_, 
                                   void* indexDataPtr_, 
                                   SizeT indexDataSize_, 
                                   IndexBuffer::Usage usage_, 
                                   IndexBuffer::Access access_)
{
    n_assert((indexType_ == IndexType::Index16) || (indexType_ == IndexType::Index32));
    n_assert(numIndices_ > 0);
    n_assert(indexDataSize_ == (numIndices_ * IndexType::SizeOf(indexType_)));
    if (IndexBuffer::UsageImmutable == usage_)
    {
        n_assert(0 != indexDataPtr_);
        n_assert(0 < indexDataSize_);
    }
    this->indexType     = indexType_;
    this->numIndices    = numIndices_;
    this->indexDataPtr  = indexDataPtr_;
    this->indexDataSize = indexDataSize_;
    this->usage         = usage_;
    this->access        = access_;
}

} // namespace Base
