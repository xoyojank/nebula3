//------------------------------------------------------------------------------
// terrainnode.cc
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "forest/terrainnode.h"

namespace Terrain
{
__ImplementClass(Terrain::TerrainNode, 'TERR', Core::RefCounted);
//------------------------------------------------------------------------------
/**
*/
TerrainNode::TerrainNode() :
maxPixelError(5.0f),
maxTexelSize(1.0f)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
TerrainNode::~TerrainNode()
{
}
}