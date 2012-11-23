//-----------------------------------------------------------------------------
//  nmaxskinpartitioner.h
//
//  (C)2005 Kim, Hyoun Woo.
//-----------------------------------------------------------------------------
#ifndef N_MAXSKINPARTITIONER_H
#define N_MAXSKINPARTITIONER_H
//-----------------------------------------------------------------------------
#include "util/narray.h"

class nMaxMesh;
class nMeshBuilder;
class nSkinShapeNode;

//-----------------------------------------------------------------------------
/**
    @class nMaxMeshFragment
    @ingroup NebulaMaxExport2Contrib

    @brief
*/
class nMaxMeshFragment
{
public:
    struct Fragment
    {
        /// group map index in meshbuilder.
        int          groupMapIndex;
        /// joint array of a joint palette.
        nArray<int>  bonePaletteArray;
    };

    /// skinned shape node which to be specified this fragment and joints.
    nSkinShapeNode*  node;
    /// array for fragments.
    nArray<Fragment> fragmentArray;
};

//-----------------------------------------------------------------------------
/**
    @class nMaxMeshFragment
    @ingroup NebulaMaxExport2Contrib

    @brief Do partitioning for skinned meshes and specifies its fragments and
           joint palette.
*/
class nMaxSkinPartitioner
{
public:
    nMaxSkinPartitioner();
    virtual ~nMaxSkinPartitioner();

    void Partitioning(nArray<nMaxMesh*> &srcMeshArray, nMeshBuilder &srcMeshBuilder);

protected:
    void BuildMeshFragments(nArray<nMaxMeshFragment> &meshFragmentArray);

};
//-----------------------------------------------------------------------------
#endif
