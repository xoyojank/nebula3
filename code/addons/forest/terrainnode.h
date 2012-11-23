#pragma once
//------------------------------------------------------------------------------
/**
    @class nTerrainNode
    @ingroup NCTerrain2

    @brief Scene graph node for the ncterrain2 package.

    (C) 2003 RadonLabs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Terrain
{
class TerrainNode : public Core::RefCounted
{
    __DeclareClass(TerrainNode);
public:
    /// constructor
    TerrainNode();
    /// destructor
    virtual ~TerrainNode();


};


}

//------------------------------------------------------------------------------

