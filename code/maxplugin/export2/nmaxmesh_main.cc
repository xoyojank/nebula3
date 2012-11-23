//-----------------------------------------------------------------------------
//  nmaxmesh_main.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmax.h"
#include "export2/nmaxnullview.h"
//#include "util/String.h"
#include "export2/nmaxoptions.h"
//#include "tools/nmeshbuilder.h"

#include "export2/nmaxmesh.h"
#include "export2/nmaxbones.h"
#include "export2/nmaxvertexinfluence.h"
#include "export2/nmaxmaterial.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxutil.h"
#include "export2/nmaxcustattrib.h"
#include "export2/nmaxskinpartitioner.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "kernel/nfileserver2.h"
#include "kernel/nfile.h"
#include "tinyxml/tinyxml.h"
//#include "scene/nskinshapenode.h"
//#include "scene/nshadowskinshapenode.h"
//#include "scene/nshadownode.h"
//#include "scene/nparticleshapenode2.h"
//#include "nature/nswingshapenode.h"
//#include "scene/nattachmentnode.h"
#include "util/string.h"

using namespace Util;

//-----------------------------------------------------------------------------
/**
*/
nMaxMesh::nMaxMesh() : 
    maxNode(0),
    refMaker (NULL),
    mesh(NULL),
    deleteMesh(FALSE),
    beginSkin(false),
    isSkinned(false),
    isPhysique(false),
    meshType (Shape),
    hasCustomAttr(false)
{
    vertexFlag = 0;
}

//-----------------------------------------------------------------------------
/**
*/
nMaxMesh::~nMaxMesh()
{
}

//-----------------------------------------------------------------------------
/**
     Retrieves a Mesh from given INode.

     Call after ReferenceMaker::RenderBegin(0);

     @param inode
     @param needDelete true, if the primitive already has cached mesh.
                       (it should be deleted)
*/
Mesh* nMaxMesh::GetMeshFromRenderMesh(INode* inode, BOOL &needDelete)
{
    n_assert(inode);

    TimeValue animStart = nMaxInterface::Instance()->GetAnimStartTime();

    Object* obj = nMaxUtil::GetBaseObject(inode, animStart);

    GeomObject* geomObj = (GeomObject*)obj;

    nMaxNullView nullView;
    needDelete = false;

    Mesh* mesh = 0;

    // Primitives that already have a mesh cached can just return a pointer to it 
    // (and set needDelete to false).
    mesh = geomObj->GetRenderMesh(animStart, inode, nullView, needDelete);

    return mesh;
}

//-----------------------------------------------------------------------------
/**
    Retrieves a TriObject from a given INode.

    The following shows that how to get a Mesh from retrieved TriObject.

    @param inode the node which we want to retrieve TriObject from
    @param needDelete true, if the retrieved TriObject should be deleted.

    @return pointer to the TriObject. Null, if the given node can not be
            converted to TriObject type.
*/
Mesh* nMaxMesh::GetMeshFromTriObject(INode* inode, BOOL &needDelete)
{
    n_assert(inode);

    TimeValue animStart = nMaxInterface::Instance()->GetAnimStartTime();
    
    Object* obj = nMaxUtil::GetBaseObject(inode, animStart);

    if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
    {
        TriObject* tri = (TriObject*)obj->ConvertToType(animStart, 
                                                Class_ID(TRIOBJ_CLASS_ID, 0));

        needDelete = false;
        // if the pointer to the object is not equal to the retrieved pointer,
        // the TriObject should be deleted after.
        if (obj != tri)
            needDelete = true;

        Mesh* mesh = &(tri->mesh);
        this->triObj = tri;

        return mesh;
    }
    else
    {
        n_maxlog(Error, "Could not get mesh from TriObject of %s", inode->GetName());
        return NULL;
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves mesh from the given node.

    @param inode 3dsmax node
    @return pointer to Mesh, if there's no mesh, return NULL.
*/
Mesh* nMaxMesh::LockMesh(INode* inode)
{
    n_assert(refMaker == NULL);

    refMaker = inode;
    refMaker->RenderBegin(0);

    this->mesh = GetMeshFromRenderMesh(inode, this->deleteMesh);
    //this->mesh = GetMeshFromTriObject(inode, this->deleteMesh);

    return mesh;
}

//-----------------------------------------------------------------------------
/**
    Delete retrieved mesh file is other than original one.
*/
void nMaxMesh::UnlockMesh()
{
    //n_assert(refMaker != NULL);

    if (this->deleteMesh)
    {
        this->mesh->DeleteThis();
        //this->triObj->DeleteThis();
    }

    refMaker->RenderEnd(0);
    refMaker = NULL;

    this->triObj = 0;
    this->mesh = 0;
    this->deleteMesh = false;
}

//-----------------------------------------------------------------------------
/**
    Retrieves mesh options from node custom attribute and create xml elements
    based on it. 

    @return 
*/
bool nMaxMesh::GetCustAttrib(Animatable* obj)
{
    // xml document which created xml elements are linked.
    TiXmlDocument xmlDoc;

    if (obj && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        IDerivedObject* derivedObj = (IDerivedObject*) obj;
        for (int i = 0; i < derivedObj->NumModifiers(); i++)
        {
            Modifier* mod = derivedObj->GetModifier(i);
            if (mod)
            {            
                GetCustAttrib(mod);
            }
        }

        Object* refObj = derivedObj->GetObjRef();
        if (refObj != obj)
        {            
            GetCustAttrib(refObj);
        }

        BaseObject* baseObj = derivedObj->FindBaseObject();
        if (baseObj != obj && baseObj != refObj)
        {            
            GetCustAttrib(baseObj);
        }
    }

    if (!obj)
        return false;

    // convert node custom attributes to xml data.
    nMaxCustAttrib custAttrib;
    if (!custAttrib.Convert(obj, xmlDoc))
    {
        n_maxlog(High, "The node has no custom attributes.");
        return false;
    }

//#ifdef _DEBUG
//    xmlDoc.SaveFile("c:\\custattrb.xml");
//#endif

    this->hasCustomAttr = true;

    TiXmlHandle xmlHandle(&xmlDoc);

    TiXmlElement* e;

    // parameter name of mesh types
    const char* mtParamName = "MeshTypes";

    e = xmlHandle.FirstChild(mtParamName).Element();
    if (e)
    {
        TiXmlElement* child;

        // get the mesh type
        child = xmlHandle.FirstChild(mtParamName).FirstChild("meshtype").Child("", 0).Element();
        if (child)
        {
            int type;
            child->Attribute("value", &type);
        
            // radiobutton of max script ui start with '1'.
            switch(type)
            {
            case 1:
                this->meshType = Shape;
                break;
            case 2:
                this->meshType = Swing;
                break;
            case 3:
                this->meshType = Shadow;
                break;
            case 4:
                this->meshType = Collision;
                break;
            case 5:
                this->meshType = Particle2;
                break;
            case 6:
                this->meshType = Sky;
                break;
            case 0:
            default:
                this->meshType = None;
                break;
            }
        }
    }

    // parameter name of vertex options
    const char* voParamName = "VertexOptions";

    e = xmlHandle.FirstChild(voParamName).Element();
    if (e)
    {
        // retrieves mesh vertex options from xml.
        int flag;

        this->vertexFlag = 0;

        TiXmlElement* child;
        //for (e; e; e = e->NextSiblingElement())
        {
            child = xmlHandle.FirstChild(voParamName).FirstChild("normal").Child("", 0).Element();
            if (child)
            {
                child->Attribute("value", &flag);
                if (flag)
                    this->vertexFlag |= VertexNormal;
            }

            child = xmlHandle.FirstChild(voParamName).FirstChild("color").Child("", 0).Element();
            if (child)
            {
                child->Attribute("value", &flag);
                if (flag)
                    this->vertexFlag |= VertexColor;
            }

            child = xmlHandle.FirstChild(voParamName).FirstChild("uvs").Child("", 0).Element();
            if (child)
            {
                child->Attribute("value", &flag);
                if (flag)
                    this->vertexFlag |= VertexUvs;
            }

            child = xmlHandle.FirstChild(voParamName).FirstChild("tangent").Child("", 0).Element();
            if (child)
            {
                child->Attribute("value", &flag);
                if (flag)
                    this->vertexFlag |= VertexTangent;
            }
        }
    }

    // parameter block name for picked sky element to link.
    const char* skyElemParamName = "SkyLinkedElem";

    e = xmlHandle.FirstChild(skyElemParamName).Element();
    if (e)
    {
        TiXmlElement* child;
        child = xmlHandle.FirstChild(voParamName).FirstChild("linkedElem").Child("", 0).Element();
        if (child)
        {
            const char* linkedElemName = child->Attribute("value");
            this->skyLinkElemName = linkedElemName;
        }
    }

    // parameter block name for mesh directory setting.
    const char* dirParamName = "MeshDirSetting";

    e = xmlHandle.FirstChild(dirParamName).Element();
    if (e)
    {
        // find parameter with the given its name.
        TiXmlElement* child;
        child = xmlHandle.FirstChild(dirParamName).FirstChild("meshDir").Child("", 0).Element();
        if (child)
        {
            const char* path = child->Attribute("value");
            if (path)
            {
                this->meshPath = path;

                // The directory parameter has "" for default string. It is absolutely necessary in Max6.
                // Without that, the exporter is not usable as the panels that have those controls in them don't work.
                //this->meshPath = nFileServer2::Instance()->ManglePath(this->meshPath.IsEmpty() ? nMaxOptions::Instance()->GetMeshesAssign() : this->meshPath);
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
/**
    Create proper nebula scene node object.

    @param inode 3dsmax geometry node for the mesh.
    @param nodename A name for this node, proper name is passed if the the mesh
                    has multi material.

    @return created nebula scene node object.
*/
Models::ModelNode* nMaxMesh::CreateShapeNode(INode* inode, String &nodename)
{
	Models::ModelNode* sceneNode = 0;

    // check that the given node is skinned mesh or static mesh.
    bool skinnedMesh = false;
    if (this->IsSkinned() || this->IsPhysique())
        skinnedMesh = true;
    else
        skinnedMesh = false;

    if (!skinnedMesh)
    {
        // we do not have any skinned mesh in the scene (only static meshes are exist)
        //if (this->meshType == Shadow)
        //    sceneNode =  (nShadowNode*)CreateNebulaObject("nshadownode", nodename.Get());
        //else
        if (this->meshType == Shape)
			sceneNode =  ShapeNode::Create();//(nShapeNode*)CreateNebulaObject("nshapenode", nodename.Get());
        //else
        //if (this->meshType == Swing)
        //    sceneNode = (nSwingShapeNode*)CreateNebulaObject("nswingshapenode", nodename.Get());
        //else
        //if (this->meshType == Particle2)
        //    sceneNode = (nParticleShapeNode2*)CreateNebulaObject("nparticleshapenode2", nodename.Get());
        else
        {
            sceneNode = 0; // collision mesh
        }
    }
    else
    {
        // the given node is skinned mesh.
        //if (this->meshType == Shadow)
        //    sceneNode = (nShadowSkinShapeNode*)CreateNebulaObject("nshadowskinshapenode", nodename.Get());
        //else
        //if (this->meshType == Shape)
        //    sceneNode = (nShapeNode*)CreateNebulaObject("nskinshapenode", nodename.Get());
        //else
        //{
        //    sceneNode = 0; // collision mesh
        //}
    }

    return sceneNode;
}

//-----------------------------------------------------------------------------
/**
    Get mesh and material data from the given node.

    -14-Mar-07  Fixed to handle null material. Thank Cho Jun Heung for the patch.

    @param inode
    @param globalMeshBuilder mesh builder to merge each of mesh data into it.
    @param useIndivisualMesh save mesh data per 3dsmax geometry node if it is true.

    @return return created nebula object. 
*/
Models::ModelNode* nMaxMesh::Export(INode* inode)
{
    this->maxNode = inode;

	Models::ModelNode* createdNode = 0;

    n_maxlog(High, "Start to retrieve custom attributes from the node %s.", inode->GetName());

    // get custom attributes of the node if it exist.
    Object* obj = nMaxUtil::GetBaseObject(inode, 0);
    if (GetCustAttrib(obj))
    {
        n_maxlog(Medium, "Base Object of %s node has custom attributes.", inode->GetName());
    }

    if (GetCustAttrib(inode))
    {
        n_maxlog(Medium, "%s node has custom attributes.", inode->GetName());
    }

    // this is needed for an object which one is what any modifier is applied.
    if(inode->GetObjectRef() != obj)
    {
        if (GetCustAttrib(inode->GetObjectRef()))
        {
            n_maxlog(Medium, "ObjectRef of %s node has custom attributes.", inode->GetName());
        }
    }

    n_maxlog(High, "End to retrieve custom attributes from the node %s.", inode->GetName());

    // check for that this mesh is skinned(or physique). 
    // if it does, do some initializations to acess max skin(or physique) interface 
    // to extract vertex weight.
    //bool skinnedMesh = false;
    //skinnedMesh = BeginSkin(inode);
    BeginSkin(inode);

    // Get the number of materials to check this mesh uses single material or multi material.
    // The reason to use GetNumMaterial instead of using nMaxMaterial::GetType() is that
    // there's multi-sub material type which only has one sub material.
    // In this case, we process this mesh as same way of a single material mesh.
    int numMaterials = nMaxUtil::GetNumMaterials(inode);
    if (numMaterials == 1)
    {
        // we have single material.
        int baseGroupIndex = GetMesh(inode, &this->localMeshBuilder, -1, 1);

        if (baseGroupIndex >= 0)
        {
            String nodename(inode->GetName());

            this->nodeName = inode->GetName();

            // In case of the shadow node, add '_shadow' postfix.
            if (this->meshType == Shadow)
                nodename += "_shadow";

            createdNode = this->CreateShapeNode(inode, nodename);
            this->sceneNodeArray.Append(createdNode);


            // save group index for skin partitioning and mesh fragments.
            if (createdNode)
            {
                SetSkinAnimator(inode, (nShapeNode*)createdNode, numMaterials);

                // specifies local bounding box.
                bbox localBox;
                localBox = this->localMeshBuilder.GetGroupBBox(baseGroupIndex);

                // scale transformation for local mesh bounding box.
                float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();
                matrix44 matScale;
                matScale.scale(vector3(geomScale, geomScale, geomScale));
                localBox.transform(matScale);

                //((nShapeNode*)createdNode)->SetLocalBox(localBox);
            }

            // build material
            if (this->meshType == Shape || this->meshType == Swing || this->meshType == Particle2)
                GetMaterial(inode, (nShapeNode*)createdNode, 0);

            if (createdNode)
            {
                // save mesh file and specifies it to the shape node.
                SetMeshFile(createdNode);
            }
        }
    }
    else
    {
        // we have multi materials

        // Create nTransform node for a mesh has multi material.
        // Guess that, a multi material mesh has some other child mesh in its hierarchy.
        // The multi material mesh to be splited by its number of the material and create
        // nShapeNode(or nSkinShapeNode) as that number. 
        // And so, where its child mesh to be located? We put its child under nTransformNode.
        // e.g) 
        //    nTransformNode parent // parent mesh
        //        nShapeNode material0
        //        nShapeNode material1
        //        nTransformNode child0 //  child mesh
        //            nShapeNode child_material0
        //            nShapeNode child_material1
        nTransformNode* transformNode = 0;

        String transformNodeName(inode->GetName());
        //FIXME: in case of the shadow node, add '_shadow' postfix.
        if (this->meshType == Shadow)
            transformNodeName += "_shadow";

        transformNode = (nTransformNode*)CreateNebulaObject("ntransformnode", transformNodeName.Get());

        bbox3 parentLocalBox;

        for (int matIdx=0; matIdx<numMaterials; matIdx++)
        {
            // build mesh.
            int numVertices = this->localMeshBuilder.GetNumVertices();
            int baseGroupIndex = GetMesh(inode, &this->localMeshBuilder, matIdx, numMaterials);

            // if mesh with matIdx has no faces, ignore it at all.
            if (baseGroupIndex < 0)
                continue;

            // we have null material in the slot of the material editor.
            if( numVertices == this->localMeshBuilder.GetNumVertices())
            {
                //HACK: this case might happen when we removed a material from slot.
                //      By the way, even inspite of the the material was removed, total number of 
                //      materials are not changed. I have NO idea that is a bug or intended.
                continue;
            }

            // create node name. we append material index to the end of the node name.
            String nodename;
            nodename += inode->GetName();

            if (this->meshType == Shadow)
                nodename += "_shadow";

            nodename += "_";
            nodename.AppendInt(matIdx);

            createdNode = this->CreateShapeNode(inode, nodename);
            this->sceneNodeArray.Append(createdNode);

            // save group index for skin partitioning and mesh fragments.
            if (createdNode)
            {
                SetSkinAnimator(inode, (nShapeNode*)createdNode, numMaterials);

                // specifies local bounding box.
                bbox3 localBox;
                localBox = this->localMeshBuilder.GetGroupBBox(baseGroupIndex);

                // scale transformation for local mesh bounding box.
                float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();
                matrix44 matScale;
                matScale.scale(vector3(geomScale, geomScale, geomScale));
                localBox.transform(matScale);

                ((nShapeNode*)createdNode)->SetLocalBox(localBox);

                parentLocalBox.extend(localBox);
            }

            // build material.
            if (this->meshType == Shape || this->meshType == Swing)
                GetMaterial(inode, (nShapeNode*)createdNode, matIdx);

            // save mesh file and specifies it to the shape node.
            SetMeshFile(createdNode);
            
            if (createdNode)
            {
                // set cwd to the parent to put the shape in the same hierarchy.
                // (cause all these shapes belong to same mesh)
                nKernelServer::Instance()->PopCwd();
            }
        }

        // scale transformation for local mesh bounding box.
        float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();
        matrix44 matScale;
        matScale.scale(vector3(geomScale, geomScale, geomScale));
        parentLocalBox.transform(matScale);

        // specifies local bouding box of multi-sub transform node's.
        transformNode->SetLocalBox(parentLocalBox);
        
        //nKernelServer::Instance()->PopCwd();

        createdNode = transformNode;
    }

    if (this->IsSkinned() || this->IsPhysique())
    {
        EndSkin();
    }

    return createdNode;
}

//-----------------------------------------------------------------------------
/**
    Check the given mesh uses per vertex alpha or not.
*/
bool nMaxMesh::UsePerVertexAlpha(Mesh* mesh)
{
    return (mesh->mapSupport(MAP_ALPHA) ? true : false);
}

//-----------------------------------------------------------------------------
/**
    Get mesh from given 3dsmax node.

    @param inode 3dsmax's node.
    @param meshBuilder mesh builder which to be used to save mesh.
    @param matidx material index
    @param numMats number of the material of given node
    @param baseGroup
*/
int nMaxMesh::GetMesh(INode* inode, nMeshBuilder* meshBuilder, const int matIdx, const int numMats, bool worldCoords)
{
    n_assert(meshBuilder);

    Mesh* mesh = this->LockMesh(inode);
    if (!mesh)
    {
        n_maxlog(-1, "The node '%s' does not have mesh.", inode->GetName());
        return -1;
    }

    int baseVertexIndex = meshBuilder->GetNumVertices();
    int baseTriangleIndex = meshBuilder->GetNumTriangles();

    int baseGroupIndex;
    nArray<nMeshBuilder::Group> groupMap;
    meshBuilder->BuildGroupMap(groupMap);
    baseGroupIndex = groupMap.Size();

    int iVertCount = 0;

    //TODO: check if we need vertex normal.
    // build vertex normal of the given mesh
    mesh->buildNormals();
    mesh->checkNormals(TRUE);

    this->pivMat.IdentityMatrix();
    if (!IsPhysique() && !IsSkinned() && !worldCoords)
    {
        pivMat = (inode->GetObjectTM(0)*Inverse(inode->GetNodeTM(0))) * pivMat;
    }
    else
    {
        pivMat = inode->GetObjectTM(0);
    }

    int i;
    int numFaces = mesh->getNumFaces();

    int v1, v2, v3;

    bool hasFaces = false;

    for (i=0; i<numFaces; i++)
    {
        if (!HasSameMaterial(mesh, i, matIdx, numMats))
            continue;

        hasFaces = true;
        Face& face = mesh->faces[i];

        if (HasNegativeScale(inode))
        {
            v1 = 0; v2 = 2; v3 = 1;
        }
        else
        {
            v1 = 0; v2 = 1; v3 = 2;
        }

        nMeshBuilder::Vertex vertex1, vertex2, vertex3;

        vertex1 = GetVertex(mesh, face, i, v1);
        vertex2 = GetVertex(mesh, face, i, v2);
        vertex3 = GetVertex(mesh, face, i, v3);

        meshBuilder->AddVertex(vertex1);
        meshBuilder->AddVertex(vertex2);
        meshBuilder->AddVertex(vertex3);

        nMeshBuilder::Triangle triangle;
        triangle.SetVertexIndices(baseVertexIndex + iVertCount,
                                  baseVertexIndex + iVertCount + 1,
                                  baseVertexIndex + iVertCount + 2 );

        if (IsPhysique() || IsSkinned())
            triangle.SetUsageFlags(nMesh2::WriteOnce | nMesh2::NeedsVertexShader);
        else
            triangle.SetUsageFlags(nMesh2::WriteOnce);

        triangle.SetGroupId(baseGroupIndex);

        if (matIdx >= 0)
            triangle.SetMaterialId(matIdx);

        meshBuilder->AddTriangle(triangle);

        iVertCount += 3;
    }

    this->UnlockMesh();


#ifdef _DEBUG
    n_maxlog(Medium, "Number of vertices of %s node are %d", inode->GetName(), 
                                                             meshBuilder->GetNumVertices());
#endif
    //return true;
    return hasFaces ? baseGroupIndex : -1;
}

//-----------------------------------------------------------------------------
/**
    Get per vertex component from 3dsmax's Mesh and specifies it to 
    mesh builder's vertex.

    @return mesh builder's vertex.
*/
nMeshBuilder::Vertex nMaxMesh::GetVertex(Mesh* mesh, Face& face, int faceNo, int vIdx)
{
    nMeshBuilder::Vertex vertex;

    // face.v[vIdx] : get the actual index of the vertex of the Mesh vertex array.
    vector3 pos = GetVertexPosition(mesh, face.v[vIdx]);
    vertex.SetCoord(pos);

    nMaxOptions* option = nMaxOptions::Instance();

	// FIXME: we don't use MAX's custom attributes for filtering each of vertex components
    //if( this->hasCustomAttr )
    //{
    //    // vertex normal.
    //    if (this->ExportNormals())
    //    {
    //        vector3 norm = GetVertexNormal(mesh, face, faceNo, vIdx);
    //        vertex.SetNormal(norm);
    //    }

    //    // vertex color.
    //    if (this->ExportColors())
    //    {
    //        vector4 col = GetVertexColor(mesh, faceNo, vIdx);
    //        vertex.SetColor(col);
    //    }

    //    // vertex uvs.
    //    if (this->ExportUvs())
    //    {
    //        //FIXME: is 'm' identical to map channel?
    //        int layer = 0;
    //        for (int m=1; m<MAX_MESHMAPS-1; m++)
    //        {
    //            if (mesh->mapSupport(m))
    //            {
    //                vector2 uvs = GetVertexUv(mesh, faceNo, vIdx, m);
    //                vertex.SetUv(layer++, uvs);
    //            }
    //        }
    //    }
    //}
    //else
    {
        // vertex normal.
        if (option->ExportNormals())
        {
            vector3 norm = GetVertexNormal(mesh, face, faceNo, vIdx);
            vertex.SetNormal(norm);
        }

        // vertex color.
        if (option->ExportColors())
        {
            vector4 col = GetVertexColor(mesh, faceNo, vIdx);
            vertex.SetColor(col);
        }

        // vertex uvs.
        if (option->ExportUvs())
        {
            //FIXME: is 'm' identical to map channel?
            int layer = 0;
            for (int m=1; m<MAX_MESHMAPS-1; m++)
            {
                if (mesh->mapSupport(m))
                {
                    vector2 uvs = GetVertexUv(mesh, faceNo, vIdx, m);
					uvs *= 8192.0f;
                    vertex.SetUv(layer++, uvs);
                }
            }
        }
    }
    
    if (IsPhysique() || IsSkinned())
    {
        vector4 joints, weights;

        this->GetVertexWeight(face.v[vIdx], joints, weights);

        vertex.SetJointIndices(joints);
        vertex.SetWeights(weights);
    }
    return vertex;
}

//-----------------------------------------------------------------------------
/**
    Get vertex position.
*/
vector3 nMaxMesh::GetVertexPosition(Mesh* mesh, int index)
{
    vector3 pos;

    Point3& v = mesh->verts[index] * pivMat;

    pos.set(-v.x, v.z, v.y);

    return pos;
}

//-----------------------------------------------------------------------------
/**
    Get vertex normal.
*/
vector3 nMaxMesh::GetVertexNormal(Mesh* mesh, Face& face, int faceNo, int vtxIdx)
{
    vector3 normal;
    Point3& vn = GetVertexNormal(mesh, faceNo, mesh->getRVertPtr(face.getVert(vtxIdx)));
    normal.set(-vn.x, vn.z, vn.y);

    return normal;
}

//-----------------------------------------------------------------------------
/**
   Get vertex normal.
*/
Point3 nMaxMesh::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
    Face* f = &mesh->faces[faceNo];
    DWORD smGroup = f->smGroup;
    int numNormals;
    Point3 vertexNormal;

    // Is normal specified
    // SPECIFIED_NORMAL is not currently used, but may be used in future versions.
    if (rv->rFlags & SPECIFIED_NORMAL)
    {
        vertexNormal = rv->rn.getNormal();
    }
    // If normal is not specified it's only available if the face belongs
    // to a smoothing group
    else 
    if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) 
    {
        // If there is only one vertex is found in the rn member.
        if (numNormals == 1) 
        {
            vertexNormal = rv->rn.getNormal();
        }
        else
        {
            // If two or more vertices are there you need to step through them
            // and find the vertex with the same smoothing group as the current face.
            // You will find multiple normals in the ern member.
            for (int i = 0; i < numNormals; i++)
            {
                if (rv->ern[i].getSmGroup() & smGroup)
                {
                    vertexNormal = rv->ern[i].getNormal();
                }
            }
        }
    }
    else
    {
        // Get the normal from the Face if no smoothing groups are there
        vertexNormal = mesh->getFaceNormal(faceNo);
    }

    return vertexNormal;
}

//-----------------------------------------------------------------------------
/**
    Get vertex color.
*/
vector4 nMaxMesh::GetVertexColor(Mesh* mesh, int faceNo, int vtxIdx)
{
    AColor color;

    if (mesh->getNumVertCol())
    {
        Point3 col = mesh->vertCol[mesh->vcFace[faceNo].t[vtxIdx]];

        color.r = col.x; color.g = col.y; color.b = col.z; color.a = 1.0f;
    }
    else
    {
        color.White();
    }

    if (UsePerVertexAlpha(mesh))
    {
        UVVert* uvVerts = mesh->mapVerts(MAP_ALPHA);
        if (uvVerts)
        {
            TVFace* tvFace = &mesh->mapFaces(MAP_ALPHA)[faceNo];
            color.a = uvVerts[tvFace->t[vtxIdx]].x;
        }
    }

    vector4 vertexCol;
    vertexCol.set(color.r, color.g, color.b, color.a);

    return vertexCol;
}

//-----------------------------------------------------------------------------
/**
    Get vertex uv.
*/
vector2 nMaxMesh::GetVertexUv(Mesh* mesh, int faceNo, int vtxIdx, int m)
{
    vector2 uvs;

    int numTV = mesh->getNumMapVerts(m);
    if (numTV)
    {
        int a = mesh->mapFaces(m)[faceNo].t[vtxIdx];

        Point3 pt1 = mesh->mapVerts(m)[a];

        uvs.set(pt1.x, 1.0f - pt1.y);
    }
    else
    {
        switch(vtxIdx)
        {
        case 0:
            uvs.set(0.0f, 0.0f);
            break;
        case 1:
            uvs.set(1.0f, 0.0f);
            break;
        case 2:
            uvs.set(0.0f, 1.0f);
            break;
        }
    }

    return uvs;
}

//-----------------------------------------------------------------------------
/**
    Extract vertex weight value from given vertex

    @param vertexIdx vertex index.
*/
void nMaxMesh::GetVertexWeight(int vertexIdx, vector4 &jointIndices, vector4 &weights)
{
    nMaxVertexInfluence vertexInfluence;

    if (IsPhysique())
    {
        IPhyVertexExport* vertexExport = GetVertexInterface(vertexIdx);
        if (vertexExport)
        {
            int vertexType = vertexExport->GetVertexType();

            switch(vertexType)
            {
            case RIGID_BLENDED_TYPE:
                {
                    IPhyBlendedRigidVertex* blended;
                    blended = static_cast<IPhyBlendedRigidVertex*>(vertexExport);

                    for (int i=0; i<blended->GetNumberNodes(); i++)
                    {
                        INode* bone = blended->GetNode(i);
                        float weight = blended->GetWeight(i);

                        if (bone)
                        {
                            String boneName = nMaxUtil::CorrectName(String(bone->GetName()));

                            int boneID = nMaxBoneManager::Instance()->FindBoneIDByName(boneName);

                            if (boneID >= 0)
                            {
                                vertexInfluence.AddBoneInfluence(boneID, weight);
                            }
                            else
                            {
                                n_maxlog(Error, "Bone Influence Error: bone %s", bone->GetName());
                            }
                        }
                        else
                        {
                            n_maxlog(High, "No bone influence for this vertex: bone '%s' vertex Index [%d]", 
                                bone->GetName(), vertexIdx);
                        }
                    }
                }
                break;
            case RIGID_TYPE:
                {
                    IPhyRigidVertex* rigid;
                    rigid = static_cast<IPhyRigidVertex*>(vertexExport);

                    INode* bone  = rigid->GetNode();
                    float weight = 1.0f;

                    if (bone)
                    {
                        String boneName = nMaxUtil::CorrectName(String(bone->GetName()));
                        int boneID = nMaxBoneManager::Instance()->FindBoneIDByName(boneName);

                        if (boneID >= 0)
                        {
                            vertexInfluence.AddBoneInfluence(boneID, weight);
                        }
                        else
                        {
                            n_maxlog(Error, "Bone Influence Error: bone %s", bone->GetName());
                        }
                    }
                    else
                    {
                        n_maxlog(High, "No bone influence for this vertex: bone '%s' vertex Index [%d]", 
                            bone->GetName(), vertexIdx);
                    }
                }
                break;
            }//end of switch

            ReleaseVertexInterface(vertexExport);
            vertexExport = 0;

        }
        else
        {
            n_maxlog(Error, "Invalid IPhyVertexExport.");
            return;
        }
    }
    else
    if (IsSkinned())
    {
        ISkinContextData* skinContext = GetSkinContextData();

        const int numBones = skinContext->GetNumAssignedBones(vertexIdx);
        for (int i=0; i<numBones; i++)
        {
            // retrieves the index of the bone that affecting the vertex.
            int boneIdx = skinContext->GetAssignedBone(vertexIdx, i);
            if (boneIdx < 0)
            {
                // bone index should not be less than zero.
                boneIdx = 0;
                continue;
            }

            INode* bone = GetSkin()->GetBone(boneIdx);
            float weight = skinContext->GetBoneWeight(vertexIdx, i);

            if (bone)
            {
                String boneName = nMaxUtil::CorrectName(String(bone->GetName()));

                int boneID = nMaxBoneManager::Instance()->FindBoneIDByName(boneName);

                if (boneID >= 0)
                {
                    vertexInfluence.AddBoneInfluence(boneID, weight);
                }
                else
                {
                    n_maxlog(Error, "Bone Influence Error: bone ID[%d]", boneID);
                }
            }
            else
            {
                n_maxlog(High, "No bone influence for this vertex: bone [%s] vertex Index [%d]", 
                    bone->GetName(), vertexIdx);
            }
        }
    }
    else
    {
        // should not reach here!
        n_maxlog(Error, "Failed to get the physique or skin context for the given node %s", 
            this->maxNode->GetName());
    }

//#if CURRENT_VERSION > 3 // CS version : 313 in 3dsmax7
//    // export floating bones
//    IPhyFloatingVertex* floatVertex;
//    floatVertex = static_cast<IPhyFloatingVertex*>(GetVertexInterface(vertexIdx));
//
//    if (floatVertex)
//    {
//        for(int i=0; i<floatVertex->GetNumberNodes(); i++)
//        {
//            INode* bone = floatVertex->GetNode(i);
//            float totalWeight;
//            float weight = floatVertex->GetWeight(i, totalWeight);
//
//            if (bone)
//            {
//                String boneName = nMaxUtil::CorrectName(String(bone->GetName()));
//                int boneID = nMaxBoneManager::Instance()->FindBoneIDByName(boneName);
//
//                if (boneID >= 0)
//                {
//                    vertexInfluence.AddBoneInfluence(boneID, weight);
//                }
//                else
//                {
//                    n_maxlog(Error, "Bone Influence Error: bone ID[%d]", boneID);
//                }
//            }
//            else
//            {
//                n_maxlog(High, "No bone influence for this vertex: bone '%s' vertex Index [%d]", 
//                bone->GetName(), vertexIdx);
//            }
//        }
//    }
//#endif

    // calculate bone influences for most important four bones.
    float weightThreshold = nMaxOptions::Instance()->GetWeightThreshold();
    vertexInfluence.AdjustBonesInfluence(weightThreshold);

    // specify weight and joint indices.
    jointIndices = vertexInfluence.GetIndicesVector();
    weights = vertexInfluence.GetWeightsVector();
}

//-----------------------------------------------------------------------------
/**
    Get material from given node.

    @param inode 3dsmax node to retrieve material.
    @param matID -1, if the given node has single material.
*/
void nMaxMesh::GetMaterial(INode* inode, nShapeNode* shapeNode, int matID)
{
    n_assert(shapeNode);

    nMaxMaterial material;

    material.Export(inode->GetMtl(), shapeNode, matID);
}

//-----------------------------------------------------------------------------
/**
    Determine given mesh's face has same material.

    @param mesh point to the Mesh
    @param faceIdx face index
    @param matIdx material index
    @param numMats number of material of the given Mesh.
*/
bool nMaxMesh::HasSameMaterial(Mesh* mesh, int faceIdx, const int matIdx, const int numMats)
{
    // Retrieves the zero based material index of the 'i-th' face.
    MtlID matID = mesh->getFaceMtlIndex(faceIdx) % numMats;

    return ((matIdx == -1 && numMats == 1) || matID == matIdx);
}

//-----------------------------------------------------------------------------
/**
    Negatively scaled node has inverted order of vertices. (counter clockwise)

    For example, consider exporting mirrored objects. 
    The mirrored objects has negative scale factor and the vertices of that 
    should be exported with counter clockwise order not to be inverted.

    @param m matrix of a node.
    @return true if the given matrix has negative scaling.
*/
bool nMaxMesh::HasNegativeScale(Matrix3& m)
{
    Point3 cp = CrossProd(m.GetRow(0),m.GetRow(1));
    if (DotProd(cp, m.GetRow(2)) < 0.0f)
        return true;

    return false;
}

//-----------------------------------------------------------------------------
/**
    'parity' of the matrix is set if one axis of the matrix is scaled negatively.

    @return return parity of the given node's world space transform.
*/
bool nMaxMesh::HasNegativeScale(INode* inode)
{
    n_assert(inode);
    return (inode->GetNodeTM(0).Parity() ? true : false);
}

//-----------------------------------------------------------------------------
/**
    Retrieves used map channels from given mesh.

    @param mesh pointer to the given Mesh.
    @return array of used map channel, size of the array is the number of map 
            channel which used by this the mesh.
*/
nArray<int> nMaxMesh::GetUsedMapChannels(Mesh* mesh)
{
    nArray<int> mapChannelArray;

    // m = 0 : used for vertex color.
    for (int m=1; m<MAX_MESHMAPS-1; m++) 
    {
        if (mesh->mapSupport(m)) 
        {
            int numTVx = mesh->getNumMapVerts(m);
            if (numTVx) 
            {
                mapChannelArray.Append(m);
            }
        }
    }

    return mapChannelArray;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxMesh::SetSkinAnimator(INode* inode, nSceneNode* createdNode, int numMaterials)
{
    n_assert(inode);
    n_assert(createdNode);
    if (this->IsSkinned() || this->IsPhysique())
    {
        int skelIndex = nMaxBoneManager::Instance()->GetSkelForNode(inode);
        if (this->meshType == Shape)
        {
            nSkinShapeNode *skinShapeNode = static_cast<nSkinShapeNode*>(createdNode);
            if (numMaterials > 1)
                skinShapeNode->SetSkinAnimator(nMaxBoneManager::Instance()->GetSkinAnimatorName("../../skinanimator", skelIndex).Get());
            else
                skinShapeNode->SetSkinAnimator(nMaxBoneManager::Instance()->GetSkinAnimatorName("../skinanimator", skelIndex).Get());
        }
        else
        if (this->meshType == Shadow)
        {
            nShadowSkinShapeNode *shadowSkinShapeNode = static_cast<nShadowSkinShapeNode*>(createdNode);

            if (numMaterials > 1)
                shadowSkinShapeNode->SetSkinAnimator(nMaxBoneManager::Instance()->GetSkinAnimatorName("../../skinanimator", skelIndex).Get());
            else
                shadowSkinShapeNode->SetSkinAnimator(nMaxBoneManager::Instance()->GetSkinAnimatorName("../skinanimator", skelIndex).Get());
        }
    }
    else
    {
        nMaxBoneManager::Instance()->BuildAttachmentNode(inode, createdNode, numMaterials);
    }
}

//-----------------------------------------------------------------------------
/**
    Specifies base group index of the all created shape or shadow nodes.

    @note
    Only the first base group index is passed for in-argument. 
    Last group indexes are calculated based on this in-argument.

    @param baseGroupIndex The first group index of this local mesh builder.

*/
void nMaxMesh::SetBaseGroupIndex(int baseGroupIndex)
{
    if (this->IsSkinned() || this->IsPhysique())
    {
        if (this->meshType == Shape)
        {
            // we don't need this for shadow skin shape node 
            // cause nShadowNode dees not need to be partitioned.
            int numShapeNode = this->sceneNodeArray.Size();
            for (int i=0; i<numShapeNode; i++)
            {
                nSceneNode* createdNode = this->sceneNodeArray[i];

                nSkinShapeNode *skinShapeNode = static_cast<nSkinShapeNode*>(createdNode);
                nMaxSkinMeshData groupMesh;
                groupMesh.node = skinShapeNode;
                groupMesh.groupIndex = baseGroupIndex + i;
                this->skinmeshArray.Append(groupMesh);
            }
        }
    }
    else
    {
        // nShapeNode and nShodowShapeNode exist in different location in class hierarchy.
        if (this->meshType == Shape)
        {
            int numShapeNode = this->sceneNodeArray.Size();
            for (int i=0; i<numShapeNode; i++)
            {
                nSceneNode* createdNode = this->sceneNodeArray[i];
                nShapeNode* shapeNode = static_cast<nShapeNode*>(createdNode);
                shapeNode->SetGroupIndex(baseGroupIndex + i);
            }
        }
        else
        if (this->meshType == Shadow)
        {
#if 0
            n_assert(this->sceneNodeArray.Size() == 1);
            nSceneNode* createdNode = this->sceneNodeArray[0];
            nShadowNode *shadowNode = static_cast<nShadowNode*>(createdNode);
            shadowNode->SetGroupIndex(baseGroupIndex);
#else
            int numShadowNode = this->sceneNodeArray.Size();
            for (int i=0; i<numShadowNode; i++)
            {
                nSceneNode* createdNode = this->sceneNodeArray[i];
                nShadowNode *shadowNode = static_cast<nShadowNode*>(createdNode);
                shadowNode->SetGroupIndex(baseGroupIndex + i);
            }
#endif
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Specify mesh filename to the shape node.
    If each shape node uses its own mesh file, it set its node name for its mesh
    filename otherwise uses save name.

    - 05-Mar-05 kims modified to export shadow node, 
                     The face that nshadownode and nshapenode is not derived 
                     from the same parent, ugly type casting was needed. Better idea?
*/
void nMaxMesh::SetMeshFile(nSceneNode* createdNode)
{
    if (createdNode)
    {
        if( nMaxOptions::Instance()->UseSameDir() )
        {
            this->meshPath = nMaxOptions::Instance()->GetMeshExportPath();
        }

        // specify shape node's name.
        String meshname, meshFileName;

        // specify mesh filename.
        meshname = nMaxOptions::Instance()->GetSaveFileName();

        // retrieve shape node name and append postfix by its type.
        meshFileName += nMaxUtil::RelacePathToAssign(nMaxUtil::Mesh, this->meshPath, meshname);

        // append postfix to the mesh file name.
        meshFileName += nMaxMesh::AppendMeshPostfixByType(this->meshType, 
            ((this->IsSkinned() || this->IsPhysique()) ? true : false));
        
        // add file extension (.n3d2 or .nvx2)
        meshFileName += nMaxOptions::Instance()->GetMeshFileType();

        // specify mesh file name.
        if (this->meshType == Shadow)
        {
            if (this->IsSkinned() || this->IsPhysique())
            {
                nShadowSkinShapeNode* shadowSkinNode = static_cast<nShadowSkinShapeNode*>(createdNode);
                shadowSkinNode->SetMesh(meshFileName.Get());
            }
            else
            {
                nShadowNode* shadowNode = static_cast<nShadowNode*>(createdNode);
                shadowNode->SetMesh(meshFileName.Get());
            }
        }
        else
        {
            nShapeNode* shapeNode = static_cast<nShapeNode*>(createdNode);
            shapeNode->SetMesh(meshFileName.Get());
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Build tangents for the given mesh.

    Triangle normals, tangents and binormals will be built for the mesh. If vertex
    are not present, they are built from the triangle normals.

    Be sure that the given mesh has uv0 texture coordinate.

    @param meshBuilder input to build its tangents.
    @return true, if it success.
*/
bool nMaxMesh::BuildMeshTangentNormals(nMeshBuilder &meshBuilder)
{
    const nMeshBuilder::Vertex& v = meshBuilder.GetVertexAt(0);

    bool buildTangentNormals = false;
    if( HasCustomAttribute() )
    {
        buildTangentNormals = ExportNormals() || ExportTangents();
    }
    else
    {
        buildTangentNormals = nMaxOptions::Instance()->ExportNormals() || nMaxOptions::Instance()->ExportTangents();
    }

    if (buildTangentNormals)
    {
        // build triangle normals, vertex normals and tangents.
        n_maxlog(Low, "Build tangents and normals...");

        if (false == v.HasComponent(nMeshBuilder::Vertex::UV0))
        {
            n_maxlog(Error, "The tangents require a valid uv-mapping in texcoord layer 0.");
            return false;
        }
        n_maxlog(Low, "  - Build triangle normals, tangents, and binormals...");
        meshBuilder.BuildTriangleNormals();

        if (false == v.HasComponent(nMeshBuilder::Vertex::NORMAL))
        {
            // build vertex normals by averaging triangle normals.
            n_maxlog(Low, "  - Build vertex normals...");
            meshBuilder.BuildVertexNormals();
        }
        if (nMaxOptions::Instance()->ExportTangents())
        {
            n_maxlog(Low, "  - Build vertex tangents...");
            // XXX: One day, we may want to make this configurable so that 
            // blendshape export can pass false.
            meshBuilder.BuildVertexTangents(true);
        }
        n_maxlog(Low, "Building mesh tangents and normals done.");
    }
    return true;
}

//-----------------------------------------------------------------------------
/**
    Check the validation of the given mesh builder and put the error log to
    log file if any of it exist.

    @param meshBuilder the mesh builder which to check the geometry errors.
    @param filename .n3d2 (or .nvx2) mesh file name which to be saved.
*/
void nMaxMesh::CheckGeometryErrors(nMeshBuilder& meshBuilder, String& meshName)
{
    nArray<String> geomErrorMsgArray;
    geomErrorMsgArray = meshBuilder.CheckForGeometryError();
    if (geomErrorMsgArray.Size())
    {
        String errlogfilename;
        errlogfilename = meshName;
        errlogfilename.StripExtension();
        errlogfilename += ".error";

        n_maxlog(Warning, "Warning: The exported mesh file '%s' has geometry errors.", meshName.Get());
        n_maxlog(Warning, "    - See the file '%s' for the details.", errlogfilename.Get());

        nFile* errFile = nFileServer2::Instance()->NewFileObject();
        if (errFile->Open(errlogfilename.Get(), "w"))
        {
            // put the geometry error message to log dialog.
            for (int i=0; i<geomErrorMsgArray.Size(); i++)
            {
                errFile->PutS(geomErrorMsgArray[i]);
            }

            errFile->Close();
            errFile->Release();
        }
        else
        {
            n_maxlog(Error, "Error: Failed to open error log file '%s for the geometry errrs.", 
                errlogfilename.Get());
            errFile->Release();
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Append postfix to mesh file name by its type.

    The following shows exist kind of postfix:
        - static mesh (nShapeNode) : no postfix
        - skinned mesh (nSkinShapeNode) : "_skinned"
        - static mesh shadowd (nShadowShapeNode) : "_shadow"
        - skinned mesh shadow (nShadowSkinShapeNode) : "_skinnedshadow"

    TODO: make it possible to specify mesh postfix explicitly. (in mesh option panel?)

*/
String nMaxMesh::AppendMeshPostfixByType(Type type, bool isSkinned)
{
    String postfix = "";

    switch(type)
    {
    case Shape:
        {
            if (isSkinned)
                postfix = nMaxOptions::Instance()->GetSkinnedMeshSuffixt();
        }
        break;
    case Shadow:
        {
            if (isSkinned)
                postfix = nMaxOptions::Instance()->GetSkinnedShadowMeshSuffix();
            else
                postfix = nMaxOptions::Instance()->GetShadowMeshSuffix();
        }
        break;
    case Swing:
        {
            postfix = nMaxOptions::Instance()->GetSwingMeshSuffix();
        }
        break;
    case Collision:
        {
            postfix = nMaxOptions::Instance()->GetCollisionMeshSuffix();
        }
        break;
    case Particle2:
        {
            postfix = nMaxOptions::Instance()->GetParticle2MeshSuffix();
        }
        break;
    default:
        return postfix;
    }

    return postfix;
}
