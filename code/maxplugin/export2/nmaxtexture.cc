//-----------------------------------------------------------------------------
//  nmaxtexture.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxmaterial.h"
#include "export2/nmaxtexture.h"
#include "export2/nmaxutil.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxuvanimator.h"
#include "scene/nshapenode.h"
#include "scene/nuvanimator.h"
#include "kernel/nfileserver2.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxTexture::nMaxTexture()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxTexture::~nMaxTexture()
{
}

//-----------------------------------------------------------------------------
/**
    Export textures.

    function for native 3DS Max standard material.
*/
void nMaxTexture::Export(Texmap* texmap, int subID, nShapeNode* shapeNode)
{
    if (0 == texmap)
    {
        n_maxlog(Warning, "Warning: The texmap for '%s' is null.", this->SubMapIDToString(subID));
        return;
    }

    if (IsClassID(texmap, BMTEX_CLASS_ID))
    {
        StdUVGen* uvGen = ((BitmapTex*)texmap)->GetUVGen();

        // get texture name.
        nString mapFileName = ((BitmapTex*)texmap)->GetMapName();

        //FIXME: if no textures is assigned for this material, we should assign default one.

        // get shader parameter.
        nShaderState::Param param = this->GetShaderParamFromStdMapSlot(uvGen, subID);

        // set the texture to the given node.
        nString textureName;
        textureName += nMaxOptions::Instance()->GetTextureAssign();
        textureName += mapFileName.ExtractFileName();
        shapeNode->SetTexture(param, textureName.Get());

        // copy textures.
        if (CopyTexture(mapFileName.Get()))
        {
            n_maxlog(Medium, "'%s' is copied.", mapFileName.Get());
        }

        ExportUVTransform(uvGen, shapeNode);

        //TODO: add the implementations for uv animation export.
        //ExportUVAnimator(uvGen, shapeNode);

        BitmapInfo bi;
        TheManager->GetImageInfo(&bi, mapFileName.Get());

        if (bi.NumberFrames() > 1)
        {
            // we have animated textures.

            //TODO: export a texture animator.
            //...
        }
    }
    else
    {
        // we only support bitmap texture.
        n_maxlog(Error, "Only bitmap texture is supported : %s", shapeNode->GetFullName());
    }

    for (int i=0; i<texmap->NumSubTexmaps(); i++)
    {
        Texmap* subTexmap = texmap->GetSubTexmap(i);
        this->Export(subTexmap, i, shapeNode);
    }
}

//-----------------------------------------------------------------------------
/**
    function for Nebula2 custom material.

    @param texmap    - pointer to 3dsmax texture map object for this texture map file.
    @param param     - shader parameter.
    @param shapeNode - Nebula node for this texture file will be attached.
    @param texPath   - directory path where the texture to be copied.

    -26-Oct-06  kims  Changed to read default texture map from shader.xml 
                      instead of using hard coded one.
    -21-Aug-06  kims  Changed to support to recognize subdirectories under 
                      the 'export/textures'directory.
*/
void nMaxTexture::Export(Texmap* texmap, nShaderState::Param param, nShapeNode* shapeNode, nString texPath)
{
    if (0 == texmap)
    {
        // A common mistake on this warning is that:
        // Usually there is more than one texmap object in Nebula2 custom material.
        // For example, standard material has two texmap objects, one for diffuse map 
        // and the other for bump map. 
        // The warning will be shown If you set only diffuse map.
        switch(param)
        {
        case nShaderState::DiffMap0:                       // texture: diffuse map layer 0
        case nShaderState::DiffMap1:                       // texture: diffuse map layer 1
        case nShaderState::DiffMap2:                       // texture: diffuse map layer 2
        case nShaderState::DiffMap3:                       // texture: diffuse map layer 3
        case nShaderState::DiffMap4:                       // texture: diffuse map layer 4 
        case nShaderState::DiffMap5:                       // texture: diffuse map layer 5
        case nShaderState::DiffMap6:                       // texture: diffuse map layer 6
        case nShaderState::DiffMap7:                       // texture: diffuse map layer 7
        case nShaderState::SpecMap0:                       // texture: specular map layer 0
        case nShaderState::SpecMap1:                       // texture: specular map layer 1
        case nShaderState::SpecMap2:                       // texture: specular map layer 2
        case nShaderState::SpecMap3:                       // texture: specular map layer 3
        case nShaderState::AmbientMap0:                    // texture: ambient map layer 1
        case nShaderState::AmbientMap1:                    // texture: ambient map layer 2
        case nShaderState::AmbientMap2:                    // texture: ambient map layer 3
        case nShaderState::AmbientMap3:                    // texture: ambient map layer 4
        case nShaderState::BumpMap0:                       // texture: bump map layer 0
        case nShaderState::BumpMap1:                       // texture: bump map layer 1
        case nShaderState::BumpMap2:                       // texture: bump map layer 2
        case nShaderState::BumpMap3:                       // texture: bump map layer 3
        case nShaderState::CubeMap0:                       // texture: cube map layer 0
        case nShaderState::CubeMap1:                       // texture: cube map layer 1
        case nShaderState::CubeMap2:                       // texture: cube map layer 2
        case nShaderState::CubeMap3:                       // texture: cube map layer 3
            {
                nString defValue;
                if (GetDefaultValueFromDataBase(shapeNode->GetShader(), nShaderState::ParamToString(param), defValue) && defValue.IsValid())
                {
                    shapeNode->SetTexture(param, defValue.Get());
                    n_maxlog(Low, "The texmap for '%s' is null. Use default '%s'", nShaderState::ParamToString(param), defValue.Get());
                }
                else
                    n_maxlog(Warning, "Warning: The texmap for '%s' is null.", nShaderState::ParamToString(param));
            }
            break;
        default:
            n_maxlog(Warning, "Warning: The texmap for '%s' is null.", nShaderState::ParamToString(param));
            break;
        }
        return;
    }

    if (IsClassID(texmap, BMTEX_CLASS_ID))
    {
        // get texture name.
        nString mapFileName = ((BitmapTex*)texmap)->GetMapName();
        mapFileName.ConvertBackslashes();
        mapFileName.ToLower();

        nString textureAssign = nMaxOptions::Instance()->GetTextureAssign();
        nString texturePath = nFileServer2::Instance()->ManglePath(textureAssign) + "/";

        bool doNotCopy = false;
        nString textureName;
        int findIndex = mapFileName.FindStringIndex(texturePath, 0);
        if( findIndex > -1 )
        {
            // export:textures/abc/abc.dds -> textures:abc/abc.dds
            textureName = mapFileName.Substitute(texturePath.Get(), "textures:");
            doNotCopy = true;
        }
        else
        {
            static nString definedTexPath = "/textures/";
            findIndex = mapFileName.FindStringIndex(definedTexPath, 0);
            if( findIndex >  -1 )
            {
                // c:/anything name/textures/abc/abc.dds
                // -> abc/abc.dds
                // -> textures:abc/abc.dds
                int stripLength = findIndex + definedTexPath.Length();
                nString subPathName = mapFileName.ExtractRange(stripLength, mapFileName.Length() - stripLength);
                textureName = "textures:" + subPathName;

                // make directory
                nString textureDir = textureName.ExtractDirName();
                nFileServer2::Instance()->MakePath(textureDir);
            }
            else
            {
                // e.g. destination path : d:/nebula2/exports/textrues/char2
                //      texture map      : char2_1.dds
                //      it changes to 'textures:char2/char2_1.dds'.
                textureName = nMaxUtil::RelacePathToAssign(nMaxUtil::Texture, texPath, mapFileName);
            }
        }

        // set the texture to the given node.
        shapeNode->SetTexture(param, textureName.Get());

        //TODO: check the texture image file already exist and the overwrite option.

        // copy textures.
        if( doNotCopy == false)
        {
            if (CopyTexture(mapFileName.Get(), textureName.Get()))
            {
                n_maxlog(Medium, "'%s' is copied.", mapFileName.Get());
            }
        }

        // get uv transform if it exist.
        StdUVGen* uvGen = ((BitmapTex *)texmap)->GetUVGen();
  
        ExportUVTransform(uvGen, shapeNode);

        nMaxUVAnimator maxAnimator;
        nUvAnimator* animator = static_cast<nUvAnimator*>(maxAnimator.Export(texmap));
        if (animator)
        {
            // add the animator to the shapenode.
            shapeNode->AddAnimator(animator->GetName());

            nKernelServer::Instance()->PopCwd();
        }

        BitmapInfo bi;
        TheManager->GetImageInfo(&bi, mapFileName.Get());

        if (bi.NumberFrames() > 1)
        {
            // we have animated textures.

            //TODO: export a texture animator.
            //...
        }
    }
    else
    {
        // we only support bitmap texture.
        n_maxlog(Error, "Only bitmap texture is supported : %s", shapeNode->GetFullName());
    }
}

//-----------------------------------------------------------------------------
/**
Copy texture file to texture assign directory.

@param textureName texture filename which to be copied.
@param destName texture destination path.
@return true, if the file was successfully copied, otherwise false.
*/
bool nMaxTexture::CopyTexture(const char* textureName, const char *destName /* = 0 */)
{
    n_assert(textureName);

    // copy texture if it does not exist in texture path.
    nFileServer2* fileServer = nKernelServer::Instance()->GetFileServer();
    const char* from = textureName;

    nString tmp = textureName;

    nString dest = nMaxOptions::Instance()->GetTextureAssign().Get();
    dest += tmp.ExtractFileName();
    const char* to = destName ? destName : dest.Get();

    if (!fileServer->FileExists(from))
    {
        // An error message is much nicer than an assert that takes down Max.
        nString fromFile = from;
        fromFile.ReplaceChars("\\", '\\');
        n_maxlog(Error, "Could not find source texture %s to copy.", fromFile.Get());
        return false;
    }
    else if (!fileServer->FileExists(to))
    {
        // make sure the output path exist
        fileServer->MakePath(nString(to).ExtractDirName());

        // if the texture file does not exist, copy it to texture assign directory.
        if (!fileServer->CopyFile(from, to))
        {
            n_maxlog(Error, "Failed to copy %s to %s", from, to);
            return false;
        }
    }
    else
    {
        // texture exist but force copy it to the directory.
        // if overwrite option is on.
        if (nMaxOptions::Instance()->OverwriteExistTexture())
        {
            if (!fileServer->CopyFile(from, to))
            {
                n_maxlog(Error, "Failed to copy %s to %s", from, to);
                return false;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
/**
    Retrieves uv offset, scale angle then specifies it to the given shape.
*/
void nMaxTexture::ExportUVTransform(StdUVGen* uvGen, nShapeNode* shapeNode)
{
    n_assert(uvGen);
    n_assert(shapeNode);

    TimeValue t = nMaxInterface::Instance()->GetAnimStartTime();

    float offset_u = uvGen->GetUOffs(t);
    float offset_v = uvGen->GetVOffs(t);
    float scale_u  = uvGen->GetUScl(t);
    float scale_v  = uvGen->GetVScl(t);
    float rot_u    = uvGen->GetUAng(t); // radian value.
    float rot_v    = uvGen->GetVAng(t); // radian value.
    float rot_w    = uvGen->GetWAng(t); // radian value.

    int mapChannel = uvGen->GetMapChannel() - 1; // TO DO: there must be a way to get the channel automatically from the shader material??

    // nebula needs degree value for its uv euler angle.
    float angle_u = n_rad2deg(rot_u);
    float angle_v = n_rad2deg(rot_v);
    float angle_w = n_rad2deg(rot_w);

    shapeNode->SetUvPos(mapChannel, vector2(offset_u, offset_v));
    shapeNode->SetUvEuler(mapChannel, vector3(angle_u, angle_v, angle_w));
    shapeNode->SetUvScale(mapChannel, vector2(scale_u, scale_v));
}

//-----------------------------------------------------------------------------
/**
    Retrieves string from the given submap id.
*/
const char* nMaxTexture::SubMapIDToString(int subID)
{
    switch(subID)
    {
    case ID_AM: return "<ambient>";
    case ID_DI: return "<diffuse>";
    case ID_SP: return "<specular>";
    case ID_BU: return "<bump>";
    case ID_OP: return "<opacity>";
    case ID_FI: return "<filter color>";
    case ID_SH: return "<shininess>";
    case ID_SS: return "<shininess strength>";
    case ID_SI: return "<self-illumination>";
    case ID_RL: return "<reflection>";
    case ID_RR: return "<refraction>";
    case ID_DP: return "<displacement>";
    default:
        break;
    }
    return "<unknown>";
}

//-----------------------------------------------------------------------------
/**
    Retrieves Nebula shader parameter from the given texmap and its submap ID.
*/
nShaderState::Param nMaxTexture::GetShaderParamFromStdMapSlot(StdUVGen* uvGen, int subID)
{
    nShaderState::Param shaderParam = nShaderState::InvalidParameter;

    // get its mapping channel it is applied to
    // (map channel 0 is vertex color channel)
    const int mapChannel = uvGen->GetMapChannel();

    //if (IsClassID(texmap, DMTL_CLASS_ID))
    //{
    switch(subID)
    {
    case ID_AM: // ambient
        switch(mapChannel)
        {
        case 1: shaderParam = nShaderState::AmbientMap0; break;
        case 2: shaderParam = nShaderState::AmbientMap1; break;
        case 3: shaderParam = nShaderState::AmbientMap2; break;
        case 4: shaderParam = nShaderState::AmbientMap3; break;
        default:
            n_maxlog(Low, "Use mapping channel [1~4] for %s map. map channel [%d] was used", 
                        SubMapIDToString(subID), mapChannel);
            shaderParam = nShaderState::AmbientMap0;
            break;
        }
        break;

    case ID_DI: // diffuse
        switch(mapChannel)
        {
        case 1: shaderParam = nShaderState::DiffMap0; break;
        case 2: shaderParam = nShaderState::DiffMap1; break;
        case 3: shaderParam = nShaderState::DiffMap2; break;
        case 4: shaderParam = nShaderState::DiffMap3; break;
        default:
            n_maxlog(Low, "Use mapping channel [1~4] for %s map. map channel [%d] was used", 
                SubMapIDToString(subID), mapChannel);
            shaderParam = nShaderState::DiffMap0;
            break;
        }
        break;

    case ID_SP: // specular
        switch(mapChannel)
        {
        case 1: shaderParam = nShaderState::SpecMap0; break;
        case 2: shaderParam = nShaderState::SpecMap1; break;
        case 3: shaderParam = nShaderState::SpecMap2; break;
        case 4: shaderParam = nShaderState::SpecMap3; break;
        default:
            n_maxlog(Low, "Use mapping channel [1~4] for %s map. map channel [%d] was used", 
                SubMapIDToString(subID), mapChannel);
            shaderParam = nShaderState::SpecMap0;
            break;
        }
        break;

    case ID_BU: // bump
        switch(mapChannel)
        {
        case 1: shaderParam = nShaderState::BumpMap0; break;
        case 2: shaderParam = nShaderState::BumpMap1; break;
        case 3: shaderParam = nShaderState::BumpMap2; break;
        case 4: shaderParam = nShaderState::BumpMap3; break;
        default:
            n_maxlog(Low, "Use mapping channel [1~4] for %s map. map channel [%d] was used", 
                SubMapIDToString(subID), mapChannel);
            shaderParam = nShaderState::BumpMap0;
            break;
        }
        break;

    case ID_OP:
    case ID_FI:
    case ID_SH:
    case ID_SS:
    case ID_SI:
    case ID_RL:
    case ID_RR:
    case ID_DP:
        n_maxlog(Error, "Nebula does not support %s map", SubMapIDToString(subID));
        break;

    default:
        n_maxlog(Error, "Unknown submap ID [%d]", subID);
        break;
    }
    //}
    //else
    //{
    //    // we only support standard material
    //}

    return shaderParam;
}

//-----------------------------------------------------------------------------
/**
    Check texture map ID is identical to given class ID.

    @param texmap
    @param classID 
*/
bool nMaxTexture::IsClassID(Texmap* texmap, ulong classID)
{
    if (texmap && texmap->ClassID() == Class_ID(classID, 0))
        return true;

    return false;
}

//-----------------------------------------------------------------------------
/**
    Get map type from given texture map.

    @param map
*/
nMaxTexture::Type nMaxTexture::GetType(Texmap* map)
{
    n_assert (map);

    nMaxTexture::Type type;

    if (map->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
        type = Bitmap;
    else
    if (map->ClassID() == Class_ID(COMPOSITE_CLASS_ID, 0))
        type = Composite;
    else
    if (map->ClassID() == Class_ID(MIX_CLASS_ID, 0))
        type = Mix;
    else
    if (map->ClassID() == Class_ID(RGBMULT_CLASS_ID, 0))
        type = RGBMultiply;
    else
        type = Unknown;

    return type;
}
