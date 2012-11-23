//------------------------------------------------------------------------------
//  treebillboardrenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "forest/treebillboardrenderer.h"
#include "coregraphics/vertexcomponent.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "resources/resourceloader.h"

namespace Forest
{
__ImplementClass(Forest::TreeBillboardRenderer, 'FOTR', Core::RefCounted);
__ImplementSingleton(Forest::TreeBillboardRenderer);

using namespace Util;
using namespace Math;
using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
TreeBillboardRenderer::TreeBillboardRenderer() :
    isValid(false),
    inBegin(false),
    treeInstances(1024, 1024),
    vertices(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TreeBillboardRenderer::~TreeBillboardRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardRenderer::Setup()
{
    n_assert(!this->IsValid());
    n_assert(!this->inBegin);
    n_assert(this->treeInstances.IsEmpty());

    this->isValid = true;

    // create dynamic vertex buffer
    Array<VertexComponent> vertexComponents;
    vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float4, 1));   // Particle::position
    vertexComponents.Append(VertexComponent(VertexComponent::Position, 1, VertexComponent::Float4, 1));   // Particle::stretchPosition
    vertexComponents.Append(VertexComponent(VertexComponent::Color, 0, VertexComponent::Float4, 1));      // Particle::color
    vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 1, VertexComponent::Float4, 1));   // Particle::uvMinMax
    vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 2, VertexComponent::Float4, 1));   // x: Particle::rotation, y: Particle::size

    Ptr<MemoryVertexBufferLoader> vbL = MemoryVertexBufferLoader::Create();
    vbL->Setup(vertexComponents, MaxNumVertices, NULL, 0, VertexBuffer::UsageDynamic, VertexBuffer::AccessWrite);

    this->vertexBuffer = VertexBuffer::Create();
    this->vertexBuffer->SetLoader(vbL.upcast<ResourceLoader>());
    this->vertexBuffer->SetAsyncEnabled(false);
    this->vertexBuffer->Load();
    if (!this->vertexBuffer->IsLoaded())
    {
        n_error("TreeBillboardRenderer: Failed to setup tree billboard vertex buffer!");
    }
    this->vertexBuffer->SetLoader(0);
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardRenderer::Discard()
{
    n_assert(this->IsValid());

    this->vertexBuffer->Unload();
    this->vertexBuffer = 0;

    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
    We need to register a TreeBillboardGraphicsEntity which notifies
    us about the currently used directional light source at render time.
*/
void
TreeBillboardRenderer::LoadLevel()
{
    n_assert(this->IsValid());
    //this->graphicsEntity = TreeBillboardGraphicsEntity::Create();
    //MGraphics::Server::Instance()->GetLevel()->AttachEntity(this->graphicsEntity);
}

//------------------------------------------------------------------------------
/**
    Called when the current level is left, we need to unlink our 
    graphics entity here.
*/
void
TreeBillboardRenderer::ExitLevel()
{
    n_assert(this->IsValid());

    // cleanup tree arrays
    this->CleanupTrees();

    // unlink our graphics entity
    //MGraphics::Server::Instance()->GetLevel()->RemoveEntity(this->graphicsEntity);
    //this->graphicsEntity = 0;
}

//------------------------------------------------------------------------------
/**
    Cleanup the tree dictionary. This method must be called from
    TreeServer::CleanupTrees().
*/
void
TreeBillboardRenderer::CleanupTrees()
{
    n_assert(this->IsValid());
    this->treeInstances.Clear();
}

//------------------------------------------------------------------------------
/**
    Call this method once per frame before rendering tree instances.
*/
void
TreeBillboardRenderer::Begin()
{
    n_assert(this->IsValid());
    n_assert(!this->inBegin);

    this->inBegin = true;
    this->treeInstances.Clear();
    this->vertices = this->vertexBuffer->Map(VertexBuffer::MapWriteDiscard);
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardRenderer::AddTreeInstance(const Ptr<TreeInstance>& treeInstance)
{
    n_assert(this->inBegin);

    // get the distance to the viewer for sorting
    const vector& treePos = treeInstance->GetTransform().get_position();
    float distToViewer = vector(treePos - this->viewerPos).length();
    float invDistToViewer = 1.0f / n_max(distToViewer, 1.0f);

    // add to instance array
    this->treeInstances.Append(KeyValuePair<float, TreeInstance*>(invDistToViewer, treeInstance));
}

//------------------------------------------------------------------------------
/**
    Populates the dynamic mesh with the collected tree instance billboards.
*/
void
TreeBillboardRenderer::End()
{
    n_assert(this->inBegin);
    this->inBegin = false;

    // sort the tree instance array by distance to viewer
    this->treeInstances.Sort();

    // populate vertex and index buffers
    this->UpdateDyanmicMesh();
    
    // release vertices
    this->vertexBuffer->Unmap();

    // render the billboards
    this->Render();
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardRenderer::UpdateDyanmicMesh()
{
    if (this->treeInstances.IsEmpty())
    {
        return;
    }

    IndexT treeIndex;
    for (treeIndex = 0; treeIndex < this->treeInstances.Size(); treeIndex++)
    {
        TreeInstance* treeInst = this->treeInstances[treeIndex].Value();
        const Ptr<Tree>& tree = treeInst->GetTree();
    }

    
    //float* vertices = this->refMesh->LockVertices(nMesh2::LockDiscard);
    //ushort* indices = this->refMesh->LockIndices(nMesh2::LockDiscard);
    //float* dstVertexPtr = vertices;
    //ushort* dstIndexPtr = indices;
    //ushort dstVertexIndex = 0;
    //IndexT dstIndexIndex = 0;

    //IndexT treeIndex;
    //for (treeIndex = 0; treeIndex < this->treeInstances.Size(); treeIndex++)
    //{
    //    TreeInstance* treeInst = this->treeInstances[treeIndex].Value();
    //    const Ptr<Tree>& tree = treeInst->GetTree();

    //    // get world transform, lod index and fade alpha value from the tree instance
    //    const matrix44& tform = treeInst->GetTransform();
    //    IndexT lodIndex;
    //    float fadeAlpha, bbScale;
    //    treeInst->GetBillboardData(lodIndex, fadeAlpha, bbScale);
    //    n_assert(InvalidIndex != lodIndex);

    //    // get the right TreeLOD object
    //    const Ptr<TreeLOD>& treeLod = tree->GetLODs()[lodIndex];

    //    // update the per-instance data, so that we can simply
    //    // copy over a chunk of vertices into our dynamic vertex buffer
    //    SizeT srcNumVertices = treeLod->GetNumVertices();
    //    SizeT srcVertexWidth = treeLod->GetVertexWidth();
    //    const float* srcVertexPtr  = treeLod->GetVertices();
    //    SizeT vertexChunkSize = srcNumVertices * srcVertexWidth * sizeof(float);
    //    treeLod->UpdateInstanceData(treeInst->GetTransform(), fadeAlpha, bbScale);
    //            
    //    // copy over the vertex data
    //    n_assert((dstVertexIndex + srcNumVertices) < MaxNumVertices);
    //    Memory::Copy(srcVertexPtr, dstVertexPtr, vertexChunkSize);

    //    // copy over index data, add vertex base index
    //    SizeT srcNumIndices = treeLod->GetNumIndices();
    //    const ushort* srcIndexPtr = treeLod->GetIndices();
    //    IndexT i;
    //    for (i = 0; i < srcNumIndices; i++)
    //    {
    //        *dstIndexPtr = *srcIndexPtr + dstVertexIndex;
    //        dstIndexPtr++;
    //        srcIndexPtr++;
    //    }

    //    // update destination pointers and index trackers
    //    dstVertexPtr += srcNumVertices * srcVertexWidth;
    //    dstVertexIndex += srcNumVertices;
    //    dstIndexIndex  += srcNumIndices;
    //}
    //this->refMesh->UnlockVertices();
    //this->refMesh->UnlockIndices();

    //// setup the shape node of the first tree instance for rendering 
    //// the entire billboard set
    //nShapeNode* shapeNode = this->treeInstances[0].Value()->GetTree()->GetBillboardLOD()->GetBillboardShapeNode();
    //nMeshGroup& meshGroup = this->refMesh->Group(0);
    //meshGroup.SetFirstVertex(0);
    //meshGroup.SetFirstIndex(0);
    //meshGroup.SetNumVertices(dstVertexIndex);
    //meshGroup.SetNumIndices(dstIndexIndex);
    //shapeNode->SetGroupIndex(0);           
    //shapeNode->SetMeshObject(this->refMesh.get());

    //this->renderContext.ClearLinks();
    //this->renderContext.SetRootNode(shapeNode);
    //this->renderContext.SetFrameId(MGraphics::Server::Instance()->GetFrameId());
    //if (this->lightLinkRenderContext)
    //{
    //    this->renderContext.AddLink(this->lightLinkRenderContext);
    //}
    //this->renderContext.SetFlag(nRenderContext::DoOcclusionQuery, false);
    
}

//------------------------------------------------------------------------------
/**
*/
void
TreeBillboardRenderer::Render()
{
}

} // namespace Forest
