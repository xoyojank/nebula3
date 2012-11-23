//-----------------------------------------------------------------------------
//  nmaxskinpartitioner.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxskinpartitioner.h"
#include "tools/nmeshbuilder.h"
#include "export2/nmaxmesh.h"
#include "export2/nmaxoptions.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "scene/nskinshapenode.h"

#include "tools/nskinpartitioner.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxSkinPartitioner::nMaxSkinPartitioner()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxSkinPartitioner::~nMaxSkinPartitioner()
{
}

//-----------------------------------------------------------------------------
/**
    Partitioning the given meshes and specifies its fragments to each 
    skinned shape nodes.

    @param srcMeshArray Array which contains exported meshes.
    @param srcMeshBuilder Mesh builder which contains exported meshes mesh data.
*/
void nMaxSkinPartitioner::Partitioning(nArray<nMaxMesh*> &srcMeshArray, 
                                       nMeshBuilder &srcMeshBuilder)
{
    // array to store partitioned mesh and its fragments.
    nArray<nMaxMeshFragment> meshFragmentArray;

    nMeshBuilder tmpMeshBuilder;
    nSkinPartitioner skinPartitioner;

    int maxJointPaletteSize = nMaxOptions::Instance()->GetMaxJointPaletteSize();

    // do skin partitioning.
    if (skinPartitioner.PartitionMesh(srcMeshBuilder, tmpMeshBuilder, maxJointPaletteSize))
    {
        n_maxlog(Medium, "The number of partitions: %d", skinPartitioner.GetNumPartitions());

        for (int i=0; i<srcMeshArray.Size(); i++)
        {
            nMaxMesh* mesh = srcMeshArray[i];

            for (int j=0; j<mesh->GetNumGroupMeshes(); j++)
            {
                const nMaxSkinMeshData& groupMesh = mesh->GetGroupMesh(j);

                nSkinShapeNode* node = groupMesh.node;//groupMesh.GetNode(); 
                n_assert(node);

                int groupIndex = groupMesh.groupIndex;//groupMesh.GetGroupIndex();

                // create mesh fragments per shape.
                nMaxMeshFragment meshFragment;
                meshFragment.node = node;

                if (groupIndex >= 0)
                {
                    const nArray<int>& groupMapArray = skinPartitioner.GetGroupMappingArray();

                    for ( int k=0; k<groupMapArray.Size(); k++ )
                    {
                        if ( groupMapArray[k] == groupIndex )
                        {                          
                            nArray<int> bonePaletteArray = skinPartitioner.GetJointPalette(k);

                            if (bonePaletteArray.Size() > 0)
                            {
                                // k - fragment group index.
                                // bonePaletteArray - fragment bone palette array.

                                // create fragment of the mesh.
                                nMaxMeshFragment::Fragment frg;

                                frg.groupMapIndex    = k;
                                frg.bonePaletteArray = bonePaletteArray;

                                // append created mesh fragment.
                                meshFragment.fragmentArray.Append(frg);
                            }
                        }
                    }
                }

                meshFragmentArray.Append(meshFragment);
            }
        }
    }

    //this->globalMeshBuilder = tmpMeshBuilder;
    srcMeshBuilder = tmpMeshBuilder;

    // build skin shape node's fragments.
    BuildMeshFragments(meshFragmentArray);
}

//-----------------------------------------------------------------------------
/**
    Specifies fragments and joint palette of the skin mesh based on 
    the given nMaxMeshFragment

    @param meshFragmentArray Array which contains skinned shape node and its 
    mesh fragments of previously partitioned mesh.
*/
void nMaxSkinPartitioner::BuildMeshFragments(nArray<nMaxMeshFragment>& meshFragmentArray)
{
    // build skin shape node's fragments.
    for (int i=0; i<meshFragmentArray.Size(); i++)
    {
        nMaxMeshFragment& meshFragment = meshFragmentArray[i];

        int numFragments = meshFragment.fragmentArray.Size();
        if (numFragments > 0)
        {
            nSkinShapeNode* node = meshFragment.node;

            node->BeginFragments(numFragments);

            for (int j=0; j<numFragments; j++)
            {
                nMaxMeshFragment::Fragment& frag = meshFragment.fragmentArray[j];

                node->SetFragGroupIndex(j, frag.groupMapIndex);

                int numJointPaletteSize = frag.bonePaletteArray.Size();
                node->BeginJointPalette(j, numJointPaletteSize);

                for (int k=0; k<numJointPaletteSize; k++)
                {
                    node->SetJointIndex(j, k, frag.bonePaletteArray[k]);
                }

                node->EndJointPalette(j);
            }

            node->EndFragments();
        }// end of if
    }//end of for
}
