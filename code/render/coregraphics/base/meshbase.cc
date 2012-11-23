//------------------------------------------------------------------------------
//  meshbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/meshbase.h"
#include "coregraphics/renderdevice.h"

namespace Base
{
__ImplementClass(Base::MeshBase, 'MSHB', Resources::Resource);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
MeshBase::MeshBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MeshBase::~MeshBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBase::Unload()
{
    if (this->vertexBuffer.isvalid())
    {
        this->vertexBuffer->Unload();
        this->vertexBuffer = 0;
    }
    if (this->indexBuffer.isvalid())
    {
        this->indexBuffer->Unload();
        this->indexBuffer = 0;
    }
    Resource::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void 
MeshBase::ApplyPrimitives(IndexT primGroupIndex)
{
    RenderDevice* renderDevice = RenderDevice::Instance();
    if (this->vertexBuffer.isvalid())
    {
        renderDevice->SetStreamSource(0, this->vertexBuffer, 0);
        renderDevice->SetVertexLayout(this->vertexBuffer->GetVertexLayout());
    }
    if (this->indexBuffer.isvalid())
    {
        renderDevice->SetIndexBuffer(this->indexBuffer);
    }
    renderDevice->SetPrimitiveGroup(this->GetPrimitiveGroupAtIndex(primGroupIndex));
}

} // namespace Base
