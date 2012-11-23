//-----------------------------------------------------------------------------
//  nmaxmeshimport.cc
//
//  (c)2005 Kim, Hyoun Woo
//-----------------------------------------------------------------------------

#include "import/nmaxmeshimport.h"
#include "kernel/nfileserver2.h"
#include "tools/nmeshbuilder.h"

static
void BuildMesh(int groupIdx, nArray<nMeshBuilder::Group> &group, Mesh &mesh, const nMeshBuilder& meshBuilder);

//-----------------------------------------------------------------------------
/**
*/
nMaxMeshImport::nMaxMeshImport()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxMeshImport::~nMaxMeshImport()
{
}

//-----------------------------------------------------------------------------
/**
    Import the given mesh file.
*/
bool nMaxMeshImport::Import(const char* path, ImpInterface *i)
{
    nMeshBuilder meshBuilder;
    if (!meshBuilder.Load(nFileServer2::Instance(), path))
    {
        n_message("Failed to load %s mesh file.", path);
        return false;
    }

    nArray<nMeshBuilder::Group> group;
    meshBuilder.BuildGroupMap(group);

    TriObject* obj = 0;
    ImpNode* node = 0;

    // create a mesh node for each of the group.
    for (int idx=0; idx<group.Size(); idx++)
    {
        obj = CreateNewTriObject();
        if (!obj)
        {
            return false;
        }

        node = i->CreateNode();
        if (!node)
        {
            delete obj;
            return false;
        }
        node->Reference(obj);

        BuildMesh(idx, group, obj->GetMesh(), meshBuilder);

        Matrix3 tm;
        tm.IdentityMatrix();

        node->SetTransform(0, tm);

        i->AddNodeToScene(node);

        // specify node name: filename + group index <- e.g) mybox0
        nString groupID;
        groupID.AppendInt(idx);

        nString nodename (path);
        nodename = nodename.ExtractFileName();
        nodename.StripExtension();
        nodename += groupID;

        node->SetName(nodename.Get());
    }

    i->RedrawViews();

    return true;
}

//-----------------------------------------------------------------------------
/**
    Set vertices and indices to the created 3DS Max Mesh.

    @note
    made it to be static function not a member function of the nMaxMeshImport 
    class to avoid compile problem of 3dsmax and nebula 
    (n_error and n_message redefinition error is occured in nmaximport_script.cc
     if we include 'nmeshbuilder.h')

    @param groupIdx    group index
    @param group       mesh group for this Mesh.
    @param mesh        3DS Max Mesh. (destination)
    @param mehsBuilder mesh data container which contains loaded mesh info. (source)
    
*/
static
void BuildMesh(int groupIdx, nArray<nMeshBuilder::Group> &group,
               Mesh &mesh, const nMeshBuilder& meshBuilder)
{
    int numFaces = group[groupIdx].GetNumTriangles();

    int numVerts = 0; 
    int vmin, vmax;   // vertex range 

    if (meshBuilder.GetGroupVertexRange(groupIdx, vmin, vmax))
    {
        numVerts = (vmax - vmin) + 1;
    }
    else
        return;

    if (numVerts <= 0)
        return;

    mesh.setNumVerts(numVerts);
    mesh.setNumTVerts(numVerts);
    mesh.setNumFaces(numFaces);
    mesh.setNumTVFaces(numFaces);

    // vertex index of Mesh.
    int vidx = 0;

    // vertex
    for (int i=vmin; i<=vmax; i++, vidx++)
    {
        nMeshBuilder::Vertex& v = meshBuilder.GetVertexAt(i);

        mesh.setVert(vidx, -v.coord.x, v.coord.z, v.coord.y);

        if (v.HasComponent(nMeshBuilder::Vertex::Component::UV0))
        {
            mesh.setTVert(vidx, v.uv[0].x, 1.0f - v.uv[0].y, 0);
        }
    }

    // indices.
    int firstTri = group[groupIdx].GetFirstTriangle();

    for (int i=0; i<numFaces; i++)
    {
        nMeshBuilder::Triangle &tri = meshBuilder.GetTriangleAt(firstTri + i);

        // indices.
        int v0 = tri.vertexIndex[0]-vmin;
        int v1 = tri.vertexIndex[1]-vmin;
        int v2 = tri.vertexIndex[2]-vmin;

        mesh.faces[i].setVerts(v0, v1, v2);
        mesh.tvFace[i].setTVerts(v0, v1, v2);

        mesh.faces[i].flags   = EDGE_ALL;
        mesh.faces[i].smGroup = 0;
    }

    mesh.buildNormals();
    mesh.buildBoundingBox();

    mesh.InvalidateEdgeList();
    mesh.InvalidateGeomCache();
    mesh.InvalidateTopologyCache();
}

