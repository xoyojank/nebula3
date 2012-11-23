//------------------------------------------------------------------------------
//  forestserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/binaryreader.h"
#include "forest/rt/terrainserver.h"
#include "io/ioserver.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/internalcameraentity.h"
#include "internalgraphics/internalgraphicsserver.h"

namespace Terrain
{
__ImplementClass(Terrain::TerrainServer, 'FOSR', Core::RefCounted);
__ImplementSingleton(TerrainServer);

using namespace Util;
using namespace Math;
using namespace IO;
using namespace InternalGraphics;


//------------------------------------------------------------------------------
/**
*/
TerrainServer::TerrainServer() : 
maxPixelError(5.0f),
maxTexelSize(1.0f)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TerrainServer::~TerrainServer()
{

    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
TerrainServer::Open()
{
	if( !this->chunkLodTree.isvalid() && !this->chunkFilename.IsEmpty() )
	{
		Ptr<ChunkLodTree> tree = ChunkLodTree::Create();
		tree->SetChunkFilename(this->chunkFilename.Get());
		tree->SetTqtFilename(this->texQuadFile.Get());
		tree->Setup();
		this->chunkLodTree = tree;
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
TerrainServer::Close()
{
	this->chunkLodTree->Discard();
	this->chunkLodTree =  0;
}

//------------------------------------------------------------------------------
/**
Called before rendering
*/
void
TerrainServer::OnRenderBefore(IndexT frameId, Timing::Time time)
{
	if( !this->chunkLodTree->IsValid()) return;
	const Ptr<InternalCameraEntity>& camera = InternalGraphicsServer::Instance()->GetDefaultView()->GetCameraEntity();	
	this->chunkLodTree->Update(camera);

	//const Ptr<InternalCameraEntity>& camera = InternalGraphicsServer::Instance()->GetDefaultView()->GetCameraEntity();
	this->chunkLodTree->Render(camera);
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainServer::OnRenderAfter(IndexT frameId, Timing::Time time)
{   
	if( !this->chunkLodTree->IsValid()) return;
}

} // namespace Forest
