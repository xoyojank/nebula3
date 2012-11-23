#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestServer
        
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "timing/time.h"
#include "timing/timer.h"
#include "forest/chunklodtree.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace Terrain
{
class TerrainServer : public Core::RefCounted
{
    __DeclareClass(TerrainServer);
    __DeclareSingleton(TerrainServer);

public:

    /// constructor
    TerrainServer();
    /// destructor
    virtual ~TerrainServer();  
    
    /// open the posteffect server
    void Open();
    /// close the posteffect server
    void Close();

    /// get the current time (for smooth LOD transitions)
    Timing::Time GetTime() const;

    /// Called before rendering
    virtual void OnRenderBefore(IndexT frameId, Timing::Time time);
    /// called after rendering entities
    virtual void OnRenderAfter(IndexT frameId, Timing::Time time);

	/// set filename of .chu file
	void SetChunkFile(const char* name);
	/// get filename of .chu file
	const char* GetChunkFile() const;
	/// set texture quad file
	void SetTexQuadFile(const char* name);
	/// get texture quad file
	const char* GetTexQuadFile() const;
	/// set maximum pixel error
	void SetMaxPixelError(float e);
	/// get maximum pixel error
	float GetMaxPixelError() const;
	/// set maximum texel error
	void SetMaxTexelSize(float s);
	/// get maximum texel error
	float GetMaxTexelSize() const;
	/// set terrain scale
	void SetTerrainScale(float s);
	/// get terrain scale
	float GetTerrainScale() const;
	/// set terrain origin
	void SetTerrainOrigin(const Math::vector& orig);
	/// get terrain origin
	const Math::vector& GetTerrainOrigin() const;

protected:
	Util::String chunkFilename;
	Util::String texQuadFile;
	Ptr<ChunkLodTree> chunkLodTree;
	float maxPixelError;
	float maxTexelSize;
	float terrainScale;
	Math::vector terrainOrigin;

protected:
    Timing::Timer timer;
};

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
TerrainServer::GetTime() const
{
    return this->timer.GetTime();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetTerrainScale(float s)
{
	this->terrainScale = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
TerrainServer::GetTerrainScale() const
{
	return this->terrainScale;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetTerrainOrigin(const Math::vector& orig)
{
	this->terrainOrigin = orig;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetChunkFile(const char* name)
{
	this->chunkFilename = name;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
TerrainServer::GetChunkFile() const
{
	return this->chunkFilename.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetTexQuadFile(const char* name)
{
	this->texQuadFile = name;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
TerrainServer::GetTexQuadFile() const
{
	return this->texQuadFile.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetMaxPixelError(float e)
{
	this->maxPixelError = e;
	//if (this->refChunkLodTree.isvalid())
	//{
	//    this->refChunkLodTree->SetMaxPixelError(e);
	//}
}

//------------------------------------------------------------------------------
/**
*/
inline
float
TerrainServer::GetMaxPixelError() const
{
	return this->maxPixelError;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TerrainServer::SetMaxTexelSize(float s)
{
	this->maxTexelSize = s;
	//if (this->refChunkLodTree.isvalid())
	//{
	//    this->refChunkLodTree->SetMaxTexelSize(s);
	//}
}

//------------------------------------------------------------------------------
/**
*/
inline
float
TerrainServer::GetMaxTexelSize() const
{
	return this->maxTexelSize;
}

} 