//-----------------------------------------------------------------------------
//  nmaxutil.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "stdneb.h"
#include "export2/nmaxutil.h"
#include "export2/nmaxoptions.h"

//#include "kernel/ntypes.h"
//#include "kernel/nfileserver2.h"
//#include "util/String.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "util/string.h"

//#include "tools/nmeshbuilder.h"

#include <Tlhelp32.h>
using namespace Util;

//-----------------------------------------------------------------------------
/**
    Retrieves Object from the given node.
    It checkes the returned object is derived object and if it is, 
    tries to get its referenced object until it is not a derived object.
*/
Object* nMaxUtil::GetBaseObject(INode* inode, TimeValue time)
{
    ObjectState objState = inode->EvalWorldState(time);
    Object* obj = objState.obj;

    if (obj)
    {
        SClass_ID sID;
        sID = obj->SuperClassID();
        while( sID == GEN_DERIVOB_CLASS_ID )
        {
            obj = ((IDerivedObject*)obj)->GetObjRef();
            sID = obj->SuperClassID();
        }
    }

    return obj;
}

//---------------------------------------------------------------------------
/*
static
Modifier* nMaxUtil::FindMorpherModifier(Object *pObj)
{
    int i;

    if (!pObj)
        return(NULL);

    IDerivedObject *pDerived = NULL;
    if (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        pDerived = (IDerivedObject *) pObj;

        for (i = 0; i < pDerived->NumModifiers(); i++)
        {
            Modifier *pMod;
            Class_ID cID;

            pMod = pDerived->GetModifier(i);

            cID = pMod->ClassID();
            if (cID == MR3_CLASS_ID)
               return(pMod);
        }
    }

    if (pDerived)
        return FindMorpherModifier(pDerived->GetObjRef());
    else
        return(NULL);
}
*/
//---------------------------------------------------------------------------
/**
    It is used to find physique modifier or skin modifier.

    @code
    Object *obj = node->GetObjectRef();

    Modifier* mod;
    // find physique modifier
    mod = nMaxUtil::FindModifier(obj, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));

    // find skin modifier
    mod = nMaxUtil::FindModifier(obj, SKIN_CLASSID);
    @endcode
*/
Modifier* nMaxUtil::FindModifier(Object *obj, Class_ID classID)
{
    if (!obj)
        return NULL;

    Modifier *mod;

    while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        IDerivedObject* derivObj = static_cast<IDerivedObject*>(obj);

        int modStack = 0;

        // iterate modifier stack.
        while (modStack < derivObj->NumModifiers())
        {
            // Get current modifier.
            mod = derivObj->GetModifier(modStack);
            // See this modifier is skin.
            Class_ID cId = mod->ClassID();
            if (cId == classID)
            {
                // found the modifier
                return mod;
            }
            modStack++;
        }
        obj = derivObj->GetObjRef();
    }

    return NULL;
}
//-----------------------------------------------------------------------------
/**
    Retrieves super class id of the given Object.

    @param obj pointer to the Object class instance.
    @return super class id of the Object.
*/
SClass_ID nMaxUtil::GetSuperClassID(Object* obj)
{
    n_assert(obj != 0);

    SClass_ID sID;

    sID = obj->SuperClassID();

    while(sID == GEN_DERIVOB_CLASS_ID)
    {
        obj = ((IDerivedObject*)obj)->GetObjRef();
        sID = obj->SuperClassID();
    }

    return sID;
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxUtil::IsMorphObject(INode* inode)
{
    /*
    Object *objRef = inode->GetObjectRef();
    Class_ID cID = objRef->ClassID();

    Modifier *morpher = FindMorpherModifier(inode->GetObjectRef());
    if (morpher)
        return true;

    if (cID == Class_ID(DERIVOB_CLASS_ID, 0))
    {
        IDerivedObject *derivedObj = (IDerivedObject*)objRef;
        Object *objref2 = derivedObj->GetObjRef();
        Class_ID cID2 = objref2->ClassID();
        
        if (cID2 == Class_ID(MORPHOBJ_CLASS_ID, 0))
            return true;
    }
    else 
    if (cID == Class_ID(MORPHOBJ_CLASS_ID, 0))
        return true;
    */
    return false ;
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxUtil::IsParticle(INode* inode)
{
/*
    Class_ID eID = inode->GetObjectRef()->ClassID();

    return ((eID == Class_ID(RAIN_CLASS_ID, 0) ||
            eID == Class_ID(SNOW_CLASS_ID, 0) ||
            eID == PCLOUD_CLASS_ID ||
            eID == SUPRSPRAY_CLASS_ID ||
            eID == BLIZZARD_CLASS_ID ||
            eID == PArray_CLASS_ID));
*/
    return false;
}

//-----------------------------------------------------------------------------
/**
*/
Modifier* nMaxUtil::FindPhysique(INode* inode)
{
    Object *obj = inode->GetObjectRef();
    if (!obj)
        return NULL;

    // Is derived object
    if (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        // casting object to derived object.
        IDerivedObject* deriveObj = static_cast<IDerivedObject*>(obj);

        // Iterate the modifier stack.
        int modStack = 0;
        while (modStack < deriveObj->NumModifiers())
        {
            // get current modifier.
            Modifier* modifier = deriveObj->GetModifier(modStack);

            // check the given modifier Is physique type.
            if (modifier->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
            {
                // found the modifier.
                return modifier;
            }

            // move to the next modifier stack.
            modStack++;
        }
    }

    return NULL;
}

//-----------------------------------------------------------------------------
/**
*/
Modifier* nMaxUtil::FindSkin(INode* inode)
{
    // Get object from node. Abort if no object.
    Object* obj = inode->GetObjectRef();
    if (!obj) 
        return NULL;

    // Is derived object ?
    while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        // Yes -> Cast.
        IDerivedObject* derObj = static_cast<IDerivedObject*>(obj);

        // Iterate over all entries of the modifier stack.
        int ModStackIndex = 0;
        while (ModStackIndex < derObj->NumModifiers())
        {
            // Get current modifier.
            Modifier* mod = derObj->GetModifier(ModStackIndex);

            // Is this Skin ?
            if (mod->ClassID() == SKIN_CLASSID )
            {
                // Yes -> Exit.
                return mod;
            }

            // Next modifier stack entry.
            ModStackIndex++;
        }

        obj = derObj->GetObjRef();
    }

    // Not found.
    return NULL;
}

//-----------------------------------------------------------------------------
/**
    Retrieves the number of materials of the given node.
*/
int nMaxUtil::GetNumMaterials(INode* inode)
{
    Mtl* material = 0;
    material = inode->GetMtl();
    if (!material)
        return 1;

    if (material->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
    {
        MultiMtl *mm = (MultiMtl*)material;

        return mm->NumSubMtls();
    }

    return 1;
}

//------------------------------------------------------------------------------
/**
    Correct the given node's name to make it can be used in Nebula.
    If the node has prohibited characters for its name, it is replaced to underbar 
    '_' character.
*/
String
nMaxUtil::CorrectName(String &string)
{
    String tmp = string;
    int i = 0;

    while(tmp[i])
    {
        switch(tmp[i])
        {
        case '>':	//bad character
        case '<':	//bad character
        case '\\':	//bad character
        case '/':	//bad character
        case ':':	//bad character
        case '[':   //bad character
        case ']':   //bad character
        case '(':   //bad character
        case ')':   //bad character
        case ' ':	//space
        case '\n':	//newline
        case '\r':	//carriage return
        case '\t':	//tab
            if(tmp[i+1] == 0 ) 
                tmp[i] = 0;
            else 
                tmp[i] = '_';
            break;
        default:
            break;
        }
        i++;
    }
    return tmp;
}

////------------------------------------------------------------------------------
///**
//    Put the vertices infomation to the log dialog.
//*/
//void nMaxUtil::PutVertexInfo(nMeshBuilder& meshBuilder)
//{
 /*   const nMeshBuilder::Vertex& v = meshBuilder.GetVertexAt(0);

    nArray<nMeshBuilder::Group> groupMap;
    meshBuilder.BuildGroupMap(groupMap);

    n_maxlog(Low, "Groups: %i", groupMap.Size());
    n_maxlog(Low, "Triangles: %i", meshBuilder.GetNumTriangles());
    n_maxlog(Low, "Vertices: %i", meshBuilder.GetNumVertices());

    String msg;

    if (v.HasComponent(nMeshBuilder::Vertex::COORD))
    {
        msg += "coord ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::NORMAL))
    {
        msg += "normal ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::TANGENT))
    {
        msg += "tangent ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::BINORMAL))
    {
        msg += "binormal ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::COLOR))
    {
        msg += "color ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::UV0))
    {
        msg += "uv0 ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::UV1))
    {
        msg += "uv1 ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::UV2))
    {
        msg += "uv2 ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::UV3))
    {
        msg += "uv3 ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::WEIGHTS))
    {
        msg += "weights ";
    }
    if (v.HasComponent(nMeshBuilder::Vertex::JINDICES))
    {
        msg += "jindices";
    }
    msg.TrimRight(" ");*/

    //n_maxlog(Low, "Vertex components: %s", msg.Get());
//}

//------------------------------------------------------------------------------
/**
    Retrieve assign which is defined nmaxtoolbox.ini file for the given type.
*/
String 
nMaxUtil::GetAssignFromType (nAssignType type)
{
    switch(type)
    {
    case Anim:
        return nMaxOptions::Instance()->GetAnimAssign();
    case Gfx:
        return nMaxOptions::Instance()->GetGfxLibAssign();
    case Mesh:
        return nMaxOptions::Instance()->GetMeshesAssign();
    case Texture:
        return nMaxOptions::Instance()->GetTextureAssign();
    default:
        return nMaxOptions::Instance()->GetProjPath();
    }
}

//------------------------------------------------------------------------------
/**
    Retrieve path name which is defined nmaxtoolbox.ini file for the given type.
*/
String
nMaxUtil::GetPathFromType(nAssignType type)
{
    switch(type)
    {
    case Anim:
        return nMaxOptions::Instance()->GetAnimPath();
    case Gfx:
        return nMaxOptions::Instance()->GetGfxLibPath();
    case Mesh:
        return nMaxOptions::Instance()->GetMeshesPath();
    case Texture:
        return nMaxOptions::Instance()->GetTexturePath();
    default:
        return nMaxOptions::Instance()->GetProjPath();
    }
}

//------------------------------------------------------------------------------
/**
    Replace part of the given direcotry path to the Nebula2 assign name.

    -23-Nov-06  kims  Changed to append '/' slash character to the both of 'a' and 'b' string.
                      Thank kaikai for the patch.

    @param type - assign type e.g. texture, gfx, mesh etc.
    @param path - directory path which to be changed to one containing assgin name.
                  e.g. 'c:/nebula2/exports/meshes/char2' will be changed to'meshes:char2'
    @param fileName - resource file name

    @return path which part of path was replaced to assign name.
*/
String 
nMaxUtil::RelacePathToAssign(nAssignType type, String& path, const String& fileName)
{
    //// retrieve texture path and append destination texture path to the end of the directory.
    String ret, assign;

    //assign = GetAssignFromType(type);
    //assign.StripTrailingSlash();

    //String manglePath = nFileServer2::Instance()->ManglePath(assign);

    //String a = manglePath;
    //a.ToLower();

    //path.ConvertBackslashes();

    //String b = path;
    //b.ToLower();
    //b = b.TrimRight("/");
    //b += "/";

    //int len = a.Length();
    //a = a.TrimRight("/");
    //a += "/";
    //// not identical and b has subdirectory
    //if (a != b && (b.Length() > a.Length()))
    //{
    //    if (0 == strncmp(a.Get(), b.Get(), len))
    //    {
    //        // retrieve subdirectory of texture assgin directory,
    //        ret += GetPathFromType(type);
    //        // HACK: every manglepath has no tailing slash. 
    //        //       so we append the tailing slash to remove the first slash
    //        String sub = b.Substitute(a.Get(), "");
    //        ret += sub;
    //        ret = ret.TrimRight("/");
    //        ret += "/";
    //    }
    //    else
    //    {
    //        ret += GetPathFromType(type);
    //    }
    //}
    //else
    //{
    //    n_maxlog(Warning, "A directory for the file %s is not specified. So it is exported to %s.", 
    //        fileName.ExtractFileName().Get(), assign.Get());

    //    ret += GetPathFromType(type);
    //}

    //ret += fileName.ExtractFileName();

    return ret;
}

//-----------------------------------------------------------------------------
/**
*/
bool nMaxUtil::CheckExistingViewer(String& viewerName)
{
    int numViewer = 0;
    HANDLE snapshot = 0;
    PROCESSENTRY32 procEntry;
    String process;

    viewerName.ToUpper();

    // create a handle for processes information.
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    procEntry.dwSize = sizeof (PROCESSENTRY32);

    if (Process32First(snapshot, &procEntry))
    {
        do
        {
            String path, filename;

            path = procEntry.szExeFile;

            filename = path.ExtractFileName();
            filename.ToUpper();

            if (filename == viewerName)
                ++numViewer;
        }
        while(Process32Next(snapshot, &procEntry));
    }

    CloseHandle(snapshot);
    
    //if (numViewer > 0)
    //    return true;

    //return false;
    return ((numViewer > 0) ? true : false);
}
