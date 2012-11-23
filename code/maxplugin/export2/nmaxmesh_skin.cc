//-----------------------------------------------------------------------------
//  nmaxmesh_skin.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"
#include "export2/nmaxutil.h"
//#include "util/nstring.h"
#include <util/string.h>
#include "export2/nmaxoptions.h"
//#include "tools/nmeshbuilder.h"

#include "export2/nmaxmesh.h"
//#include "scene/nskinshapenode.h"
//#include "tools/nskinpartitioner.h"

//-----------------------------------------------------------------------------
/**
    @return True, if the given 3dsmax geometry node has skin modifier.
*/
bool nMaxMesh::IsSkinned()
{
    return isSkinned;
}
//-----------------------------------------------------------------------------
/**
    @return True, if the given 3dsmax geometry node has physique modifier.
*/
bool nMaxMesh::IsPhysique()
{
    return isPhysique;
}
//-----------------------------------------------------------------------------
/**
    Retrieves vertex interface of the vertex which has given vertex iD.
*/
IPhyVertexExport* nMaxMesh::GetVertexInterface(int vertexId)
{
    return (IPhyVertexExport*)phyContextExport->GetVertexInterface(vertexId);
}
//-----------------------------------------------------------------------------
/**
    Release retrieved vertex interface.
*/
void nMaxMesh::ReleaseVertexInterface(IPhyVertexExport* phyVertexExport)
{
    phyContextExport->ReleaseVertexInterface(phyVertexExport);
}
//-----------------------------------------------------------------------------
/**
    Retrieves interface of skin context data.
*/
ISkinContextData* nMaxMesh::GetSkinContextData()
{
    return skinContext;
}
//-----------------------------------------------------------------------------
/**
    Retrieves skin interface.
*/
ISkin* nMaxMesh::GetSkin()
{
    return skin;
}
//-----------------------------------------------------------------------------
/**
    Determine if the given mesh has physique or skin modifier.

    @note
        DO NOT FORGET TO CALL EndSkin()

    @param node 3dsmax's node.
    @return true, if the given mesh has physique or skin modifier
*/
bool nMaxMesh::BeginSkin(INode* node)
{
    n_assert(node);
    n_assert(beginSkin == false);

    beginSkin = true;   

    // Get object from given node.
    Object *obj = node->GetObjectRef();
    if (!obj)
    {
        return false;
    }

    // find physique first
    modifier = nMaxUtil::FindModifier(obj, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
    if (modifier)
    {
        // create a physique export interface for the given physique modifier.
        phyExport = (IPhysiqueExport*)modifier->GetInterface(I_PHYINTERFACE);
        if (NULL == phyExport)
        {
            return false;
        }

        //phyExport->SetInitialPose(true);

        // create a ModContext Export Interface for the given node of the physique modifier.
        phyContextExport = (IPhyContextExport*)phyExport->GetContextInterface(node);
        if (NULL == phyContextExport)
        {
            return false;
        }

        // we convert all vertices to rigid
        phyContextExport->ConvertToRigid(TRUE);
        phyContextExport->AllowBlending(TRUE);

        Matrix3 initTM;
        int result = phyExport->GetInitNodeTM(node, initTM);
        if (result != MATRIX_RETURNED)
        {
            initTM = node->GetObjectTM(0);
        }

        isPhysique = true;
        isSkinned = false;

        return true;
    }
    else
    {
        // no physique, so we try to find skin modifier.
        modifier = nMaxUtil::FindModifier(obj, SKIN_CLASSID);
        if (!modifier)
        {
            return false;
        }

        skin = (ISkin*)modifier->GetInterface(I_SKIN);
        if (NULL == skin)
        {
            return false;
        }

        skinContext = skin->GetContextInterface(node);

        isPhysique = false;
        isSkinned = true;

        return true;
    }

    object = obj;

    // not physique and skin
    return false;
}

//-----------------------------------------------------------------------------
/**
    Uninitialize things for any physique or skin modifier.
*/
void nMaxMesh::EndSkin()
{
    n_assert(beginSkin == true);

    if (isPhysique)
    {
        phyExport->ReleaseContextInterface(phyContextExport);
        modifier->ReleaseInterface(I_PHYINTERFACE, phyExport);
    }

    beginSkin = false;  
    modifier = 0;
    phyExport = 0;
    phyContextExport = 0;
}
