#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestServer
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/bbox.h"
#include "util/string.h"
#include "util/array.h"
#include "io/stream.h"
#include "forest/chunklodnode.h"
#include "internalgraphics/internalcameraentity.h"

//------------------------------------------------------------------------------
namespace Terrain
{

class ChunkLodTree : public Core::RefCounted
{
    __DeclareClass(ChunkLodTree);

public:

    /// constructor
    ChunkLodTree();
    /// destructor
    virtual ~ChunkLodTree();  

	/// open chunk file
	void Setup();
	/// close chunk
	void Discard();
	/// update the quad tree, call this when view matrix changes
	void Update(const Ptr<InternalGraphics::InternalCameraEntity>& camera);
	/// return true if object is valid
	bool IsValid() const;

	/// set terrain scale
	void SetTerrainScale(float s);
	/// get terrain scale
	float GetTerrainScale() const;
	/// set terrain origin (post-scale)
	void SetTerrainOrigin(const Math::vector& orig);
	/// set the filename of a texture quad tree file
	void SetTqtFilename(const char* filename);
	/// get the filename of the texture quad tree file
	const char* GetTqtFilename() const;
	/// set the filename of a texture quad tree file
	void SetChunkFilename(const char* filename);
	/// get the filename of the texture quad tree file
	const char* GetChunkFilename() const;
	/// set max pixel error
	void SetMaxPixelError(float e);
	/// get max pixel error
	float GetMaxPixelError() const;
	/// set max texel size
	void SetMaxTexelSize(float s);
	/// get max texel size
	float GetMaxTexelSize() const;
	/// get terrain origin (post-scale)
	const Math::vector& GetTerrainOrigin() const;
	/// get stream to open chu file
	const Ptr<IO::Stream>& GetStream();
	/// render the terrain
	void Render(const Ptr<InternalGraphics::InternalCameraEntity>& camera);


protected:
	friend class ChunkLodNode;

	/// associate a chunk pointer with its chunk label
	void SetChunkByLabel(int chunkLabel, const Ptr<ChunkLodNode>& chunk);
	/// get a chunk by its label
	const Ptr<ChunkLodNode>& GetChunkByLabel(int chunkLabel);
	/// compute a desired lod level from bounding box and view point
	ushort ComputeLod(const Math::bbox& box, const Math::vector& viewPoint) const;

protected:
	float terrainScale;
	Math::vector terrainOrigin;
	Util::String tqtFilename;
	Util::String chunkFilename;
	//nTextureQuadTree* texQuadTree;
	//nAutoRef<nResourceServer> refResourceServer;
	int chunksAllocated;
	Util::Array<Ptr<ChunkLodNode>> chunks;                  // root node of chunk tree
	int treeDepth;                          // tree depth, from .chu file
	float errorLodMax;                      // from .chu file
	float distanceLodMax;                   // computed from .chu file and set parameters
	float textureDistanceLodMax;            // computed from .chu file and set parameters
	float verticalScale;                    // from .chu file
	float baseChunkDimension;               // x/z size of root chunk
	int chunkCount;
	//nFile* chuFile;                         // open chunk file
	Ptr<IO::Stream> chuStream;
	Util::Array<Ptr<ChunkLodNode>> chunkTable;      // table of chunks, with chunk label as index

	//nDisplayMode2 displayMode;
	//nCamera2 camera;
	float maxPixelError;
	float maxTexelSize;
	bool paramsDirty;

	int numTexturesRendered;
	int numMeshesRendered;
	int numTexturesAllocated;
	int numMeshesAllocated;

	Math::bbox boundingBox;                          // computed bounding box
	bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool 
ChunkLodTree::IsValid() const
{
	return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodTree::SetTerrainOrigin(const Math::vector& orig)
{
	this->terrainOrigin = orig;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector& 
ChunkLodTree::GetTerrainOrigin() const
{
	return this->terrainOrigin;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodTree::SetTerrainScale(float s)
{
	this->terrainScale = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
ChunkLodTree::GetTerrainScale() const
{
	return this->terrainScale;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodTree::SetTqtFilename(const char* filename)
{
	this->tqtFilename = filename;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
ChunkLodTree::GetTqtFilename() const
{
	return this->tqtFilename.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodTree::SetChunkFilename(const char* filename)
{
	this->chunkFilename = filename;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
ChunkLodTree::GetChunkFilename() const
{
	return this->chunkFilename.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ChunkLodTree::SetChunkByLabel(int label, const Ptr<ChunkLodNode>& chunk)
{
	this->chunkTable[label] = chunk;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<ChunkLodNode>&
ChunkLodTree::GetChunkByLabel(int label)
{
	return this->chunkTable[label];
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<IO::Stream>&
ChunkLodTree::GetStream()
{
	n_assert(this->chuStream);
	return this->chuStream;
}

} 