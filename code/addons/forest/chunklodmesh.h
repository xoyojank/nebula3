#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestServer
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/stream.h"
#include "coregraphics/mesh.h"
#include "coregraphics/shaderinstance.h"

//------------------------------------------------------------------------------
namespace Terrain
{
class ChunkLodMesh : public Core::RefCounted
{
    __DeclareClass(ChunkLodMesh);

public:

    /// constructor
    ChunkLodMesh();
    /// destructor
    virtual ~ChunkLodMesh();  

	/// set position and size inside .chu file
	void SetFileLocation(int pos);
	/// get position and size inside .chu file
	int GetFileLocation();
	/// set decompression parameters (sx, sy, sz)
	void SetVertexScale(const Math::vector& s);
	/// get decompression parameters 
	const Math::vector& GetVertexScale() const;
	/// set vertex offset
	void SetVertexOffset(const Math::vector& offset);
	/// get vertex offset
	const Math::vector& GetVertexOffset() const;
	/// set alternative shared nFile object pointing to open .chu file
	void SetStream(const Ptr<IO::Stream>& stream);

	/// setup the object
	void Setup();
	/// discard the object
	void Discard();
	/// draw the mesh
	void Draw();

protected:
	int fileChunkPos;           // position of chunk in file
	Math::vector vertexScale;        // vertex decompression parameters
	Math::vector vertexOffset;       // world space offset for vertex decompression
	Math::float4 matDiffuse;          // a material diffuse color for debugging

	//nFile* chuFile;
	bool extChuFile;            // true if file object externally provided 
	Ptr<IO::Stream> stream;
	Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
	Ptr<CoreGraphics::IndexBuffer> indexBuffer;
	CoreGraphics::PrimitiveGroup primGroup;

	Ptr<CoreGraphics::ShaderInstance> shapeShader;
	Ptr<CoreGraphics::ShaderVariable> modelViewProj;
	Ptr<CoreGraphics::ShaderVariable> diffuseColor;
};

//------------------------------------------------------------------------------
/**
    This sets the position of the chunk data inside the .chu file.

    @param  pos     position to read data from
*/
inline
void
ChunkLodMesh::SetFileLocation(int pos)
{
    this->fileChunkPos = pos;
}

//------------------------------------------------------------------------------
/**
    Returns position and size of data chunk inside .chu file.

    @return     position of data in .chu file
*/
inline
int
ChunkLodMesh::GetFileLocation()
{
    return this->fileChunkPos;
}

//------------------------------------------------------------------------------
/**
    Set vertex decompression parameters (sx, sy, sz).
    Terrain vertices are quantized in the terrain file. They need to be
    dequantized to be of any use.

    @param  s       vector3 with the (sx, sy, sz) params
*/
inline
void
ChunkLodMesh::SetVertexScale(const Math::vector& s)
{
    this->vertexScale = s;
}

//------------------------------------------------------------------------------
/**
    Returns the vertex decompression parameters.

    @return     the vertex decompression params
*/
inline
const Math::vector&
ChunkLodMesh::GetVertexScale() const
{
    return this->vertexScale;
}

//------------------------------------------------------------------------------
/**
    Sets the vertex xyz offset.

    Vertices are compressed in the terrain file. These values are 
	used to recreate them. Offset is given in the \a xz plane
	See nLodChunk::read for examples of how to obtain these values.

    @param  offset      a vector3 with the vertex offsets
*/
inline
void
ChunkLodMesh::SetVertexOffset(const Math::vector& offset)
{
    this->vertexOffset = offset;
}

//------------------------------------------------------------------------------
/**
    Returns the vertex xyz offset.
*/
inline
const Math::vector&
ChunkLodMesh::GetVertexOffset() const
{
    return this->vertexOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
ChunkLodMesh::SetStream(const Ptr<IO::Stream>& stream)
{
	this->stream = stream;
}
} 