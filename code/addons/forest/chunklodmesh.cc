#include "stdneb.h"
#include "chunklodmesh.h"
#include "io/binaryreader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "resources/resourceloader.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/win360/d3d9types.h"

namespace Terrain
{
__ImplementClass(ChunkLodMesh,'CLMH',Core::RefCounted);
using namespace IO;
using namespace CoreGraphics;
using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ChunkLodMesh::ChunkLodMesh()
{
}

//------------------------------------------------------------------------------
/**
*/
ChunkLodMesh::~ChunkLodMesh()
{
}

//------------------------------------------------------------------------------
/**
*/
void ChunkLodMesh::Setup()
{
	n_assert(this->stream.isvalid());
	
	Ptr<BinaryReader> reader = BinaryReader::Create();
	reader->SetStream(this->stream);
	if( reader->Open())
	{
		this->stream->Seek(this->fileChunkPos,IO::Stream::Begin);
		ushort numVertices = reader->ReadUShort();
		short* vBuffer = n_new_array(short,numVertices * 4);
		reader->ReadRawData(vBuffer, numVertices * 4 * sizeof(short));

		int numIndices = reader->ReadInt();
		n_assert( numIndices > 0);
		ushort* iBuffer = n_new_array(ushort,numIndices);
		reader->ReadRawData(iBuffer, numIndices * sizeof(ushort));		

		Array<VertexComponent> vertexComponents;
		vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Short4, 1));   // Particle::position

		/// Load VertexBuffer
		Ptr<MemoryVertexBufferLoader> vbL = MemoryVertexBufferLoader::Create();
		vbL->Setup(vertexComponents, numVertices, vBuffer, numVertices * 4 * sizeof(short), VertexBuffer::UsageDynamic, VertexBuffer::AccessWrite);

		this->vertexBuffer = VertexBuffer::Create();
		this->vertexBuffer->SetLoader(vbL.upcast<ResourceLoader>());
		this->vertexBuffer->SetAsyncEnabled(false);
		this->vertexBuffer->Load();
		if (!this->vertexBuffer->IsLoaded())
		{
			n_error("ChunkLodMesh: Failed to setup tree vertex buffer!");
		}
		this->vertexBuffer->SetLoader(0);

		/// Load Index Buffer
		Ptr<MemoryIndexBufferLoader> ibL = MemoryIndexBufferLoader::Create();
		ibL->Setup(IndexType::Index16, numIndices, iBuffer, numIndices * sizeof(ushort), IndexBuffer::UsageDynamic, IndexBuffer::AccessWrite);
		this->indexBuffer = IndexBuffer::Create();
		this->indexBuffer->SetLoader(ibL.upcast<ResourceLoader>());
		this->indexBuffer->SetAsyncEnabled(false);
		this->indexBuffer->Load();
		if (!this->indexBuffer->IsLoaded())
		{
			n_error("ChunkLodMesh: Failed to setup index buffer!");
		}		
		this->indexBuffer->SetLoader(0);

		/// Fixed vertex Buffer
		float* vertices = (float*) this->vertexBuffer->Map(VertexBuffer::MapWrite);
		float* srcPtr = vertices;
		float* dstPtr = vertices;
		for (int i= 0; i< numVertices;i++ )
		{
			*dstPtr++ = (float(*srcPtr++) * this->vertexScale.x()) + this->vertexOffset.x();
			*dstPtr++ = (float(*srcPtr++) * this->vertexScale.y()) + this->vertexOffset.y();
			*dstPtr++ = (float(*srcPtr++) * this->vertexScale.z()) + this->vertexOffset.z();
			srcPtr++;
		}
		this->vertexBuffer->Unmap();

		n_delete_array(vBuffer);
		vBuffer = 0;
		n_delete_array(iBuffer);
		iBuffer = 0;

		// setup a primitive group object
		this->primGroup.SetBaseVertex(0);
		this->primGroup.SetNumVertices(numVertices);
		this->primGroup.SetBaseIndex(0);
		this->primGroup.SetNumIndices(numIndices);
		this->primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);

		// create shape shader instance
		this->shapeShader = ShaderServer::Instance()->CreateShaderInstance(ResourceId("shd:shape"));
		// lookup ModelViewProjection shader variable
		this->modelViewProj = this->shapeShader->GetVariableBySemantic(ShaderVariable::Semantic("ModelViewProjection"));
		this->diffuseColor  = this->shapeShader->GetVariableBySemantic(ShaderVariable::Semantic("MatDiffuse"));

		reader->Close();
	}///end if	
}

//------------------------------------------------------------------------------
/**
*/
void ChunkLodMesh::Discard()
{
	this->vertexBuffer = 0;
	this->indexBuffer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void ChunkLodMesh::Draw()
{	
	this->shapeShader->Begin();
	this->shapeShader->BeginPass(0);

	// resolve model-view-projection matrix and update shader
	TransformDevice* transDev = TransformDevice::Instance();
	this->modelViewProj->SetMatrix(transDev->GetModelViewProjTransform());
	this->diffuseColor->SetFloat4(Math::float4(1.0f,0.0f,0.0f,1.0f));
	this->shapeShader->Commit();

	//D3DPRIMITIVETYPE d3d9PrimType = D3D9Types::AsD3D9PrimitiveType(topology);
	//D3DFORMAT d3d9IndexType = D3DFMT_INDEX16;
	IDirect3DDevice9* d3d9Dev = RenderDevice::Instance()->GetDirect3DDevice();
	d3d9Dev->SetStreamSource(0,this->vertexBuffer->GetD3D9VertexBuffer(),0,4 * sizeof(short));
	d3d9Dev->SetIndices(this->indexBuffer->GetD3D9IndexBuffer());
	HRESULT hr = d3d9Dev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,      // PrimitiveType
		0,										// MinVertexIndex
		0,
		this->primGroup.GetNumVertices(),       // NumVertices
		0,
		this->primGroup.GetNumVertices()-2);	// VertexStreamZeroStride
	n_assert(SUCCEEDED(hr));

	//// draw indexed primitives
	//RenderDevice* renderDevice = RenderDevice::Instance();
	//renderDevice->SetStreamSource(0, this->vertexBuffer, 0);
	//renderDevice->SetIndexBuffer(this->indexBuffer);
	//renderDevice->SetVertexLayout(this->vertexBuffer->GetVertexLayout());
	//renderDevice->SetPrimitiveGroup(this->primGroup);
	//renderDevice->Draw();

	this->shapeShader->EndPass();
	this->shapeShader->End();
}
}