 //-----------------------------------------------------------------------------
//  nmaxmaterial_main.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"

#include "export2/nmaxmaterial.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxtexture.h"
#include "export2/nmaxmaterial_shd.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "export2/nmaxcontrol.h"

#include "scene/nshapenode.h"
#include "scene/nvectoranimator.h"
#include "util/nanimlooptype.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxMaterial::nMaxMaterial()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxMaterial::~nMaxMaterial()
{
}

//-----------------------------------------------------------------------------
/**
    Export material.

    -14-Mar-07  Fixed to handle null material. Thank Cho Jun Heung for the patch.

*/
void nMaxMaterial::Export(Mtl* mtl, nShapeNode* shapeNode, int matID)
{
    if (NULL == mtl)
    {
        // no material exist, so just create default material.
        CreateDefaultMaterial(shapeNode);
    }
    else
    {
        nMaxMaterial::Type materialType;
        materialType = nMaxMaterial::GetType(mtl);

        if (nMaxMaterial::Standard == materialType)
        {

        #if (MAX_RELEASE >= 6000)   // 3dsmax R6
            IDxMaterial* dxMaterial = (IDxMaterial*)mtl->GetInterface(IDXMATERIAL_INTERFACE);
            if (dxMaterial)
            {
                // we have directx material
                GetDirectXMaterial(mtl, dxMaterial, shapeNode);
            }
            else
        #endif
            {
                // export standard material
                GetStandardMaterial(mtl, shapeNode);
            }
        }
        else
        if (nMaxMaterial::MultiSub == materialType)
        {
            // we have multi-sub material.
            MultiMtl* mulMtl;
            Mtl* subMtl;

            mulMtl = (MultiMtl*)mtl;
            subMtl = mulMtl->GetSubMtl(matID);

            if (subMtl)
            {
                nMaxMaterial::Type subMtlType;
                subMtlType = nMaxMaterial::GetType(subMtl);

                if (nMaxMaterial::Standard == subMtlType)
                {
                 #if (MAX_RELEASE >= 6000)   
                    // available on 3dsmax R6 or higher.
                    IDxMaterial* dxMaterial = (IDxMaterial*)mtl->GetInterface(IDXMATERIAL_INTERFACE);
                    if (dxMaterial)
                    {
                        // we have directx material
                        GetDirectXMaterial(mtl, dxMaterial, shapeNode);
                    }
                    else
                 #endif
                    {
                        // export single material
                        GetStandardMaterial(subMtl, shapeNode);
                    }
                }
                else
                {
                    // call recursively until the material to be standard.
                    Export(subMtl, shapeNode, matID);
                }
            }
            else
            {
                //HACK: It can be reached here when we removed a material from slot.
                //      By the way, even inspite of the material was removed, total number of 
                //      materials are not changed. I have NO idea that is a bug or intended.

                n_maxlog(Error, "multi material but no sub-material");
                // no material exist, so just create default material.
                CreateDefaultMaterial(shapeNode);
            }
        }
        else 
        if (nMaxMaterial::Shell == materialType)
        {
            // Shell material type which is used for backed texture.
            Mtl* subMtl = mtl->GetSubMtl(1);

            if (!subMtl)
            {
                n_maxlog(Error, "Error, The sub material of the shell material does not exist.");
                return;
            }
            else
            {
                // call recursively until the material to be standard.
                Export(subMtl, shapeNode, matID);
            }    
        }
        else
        if (nMaxMaterial::NebulaCustom == materialType)
        {
            // nebula custom scripted plug-in material.
            GetNebulaMaterial(mtl, shapeNode);


            // if the nshapenode is nparticlenode2, we should retrieve particle part of 
            // the parameter block values from material again.
            if (shapeNode->IsA("nparticleshapenode2"))
            {
                GetParticle2Material(mtl, shapeNode);
            }
        }
        else
        {
            // unknown material type. (nor standard, multi-sub or custom)
            n_maxlog(Error, "Unknown material type.");
        }
    }
}

//-----------------------------------------------------------------------------
/**
    The given shape node does not have any material, so we create default one.
*/
void nMaxMaterial::CreateDefaultMaterial(nShapeNode* shapeNode)
{
    n_maxlog(Low, "The shape node '%s' has no material, so create default one", shapeNode->GetFullName());

    //FIXME: do we have move this to nMaxMesh::GetMesh()?
    nClass* clazz = shapeNode->GetClass();
    if (nString(clazz->GetName()) == "nskinshapenode")
    {
        shapeNode->SetShader("skinned");
    }
    else
    if (nString(clazz->GetName()) == "nshapenode")
    {
        shapeNode->SetShader("static");
    }
    else
    {
        n_maxlog(Error, "Unknown shape node type [%s]", shapeNode->GetFullName());
        shapeNode->SetShader("static");
    }

    // we only assign diffuse map on using default material.
    nString defValue;
    if (GetDefaultValueFromDataBase(shapeNode->GetShader(), nShaderState::ParamToString(nShaderState::DiffMap0), defValue) && 
        defValue.IsValid())
    {
        shapeNode->SetTexture(nShaderState::DiffMap0, defValue.Get());
        n_maxlog(Low, "The texmap for '%s' is null. Use default '%s'", 
            nShaderState::ParamToString(nShaderState::DiffMap0), defValue.Get());
    }
    else
    {
        n_maxlog(Warning, "Warning: The texmap for '%s' is null.", 
            nShaderState::ParamToString(nShaderState::DiffMap0));
    }

    vector4 ambient(0.9f, 0.9f, 0.9f, 1.0f);
    vector4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
    vector4 specular(0.5f,0.5f, 0.5f, 1.0f);
    //FIXME:
    float specularPower = 32.0f;

    shapeNode->SetVector(nShaderState::MatAmbient, ambient);
    shapeNode->SetVector(nShaderState::MatDiffuse, diffuse);
    shapeNode->SetVector(nShaderState::MatSpecular, specular);
    shapeNode->SetFloat(nShaderState::MatSpecularPower, specularPower);
}

//-----------------------------------------------------------------------------
/**
    Export DirectX material of 3dsmax (only valid for more than version 6)
*/
#if (MAX_RELEASE >= 6000)
void nMaxMaterial::GetDirectXMaterial(Mtl* mtl, IDxMaterial* dxMat, nShapeNode* shapeNode)
{

}
#endif

//-----------------------------------------------------------------------------
/**
    see shader.xml for more details

        Zero         = 1:
        One          = 2:
        SrcColor     = 3:
        InvSrcColor  = 4:
        SrcAlpha     = 5:
        InvSrcAlpha  = 6:
        DestAlpha    = 7:
        InvDestAlpha = 8:
        DestColor    = 9:
        InvDestColor = 10

    @return true if there any subtractive or additive Transparency exist.
*/
bool nMaxMaterial::SetAlphaParam(StdMat2* stdMat, nShapeNode* shapeNode)
{
    bool hasAlpha = false;

    if (stdMat->GetTransparencyType() == TRANSP_SUBTRACTIVE)
    {
        shapeNode->SetInt(nShaderState::AlphaSrcBlend, 1); // Zero
        shapeNode->SetInt(nShaderState::AlphaDstBlend, 3); // SrcColor

        hasAlpha = true;
    }
    else 
    if (stdMat->GetTransparencyType() == TRANSP_ADDITIVE)
    {
        shapeNode->SetInt(nShaderState::AlphaSrcBlend, 5); // SrcAlpha
        shapeNode->SetInt(nShaderState::AlphaDstBlend, 2); // SrcColor

        hasAlpha = true;
    }

    return hasAlpha;
}

//-----------------------------------------------------------------------------
/**
*/
void nMaxMaterial::SetStandardNebulaShader(nShapeNode* shapeNode, bool hasOpacityMap)
{
    if (0 == strcmp(shapeNode->GetClass()->GetName(), "nshapenode") && !hasOpacityMap)
    {
        shapeNode->SetShader("static");
    }
    else if (0 == strcmp(shapeNode->GetClass()->GetName(), "nshapenode") && hasOpacityMap)
    {
        shapeNode->SetShader("alpha");
    }
    else
    if (0 == strcmp(shapeNode->GetClass()->GetName(), "nskinshapenode") && !hasOpacityMap)
    {
        shapeNode->SetShader("skinned");
    }
    else
    if (0 == strcmp(shapeNode->GetClass()->GetName(), "nskinshapenode") && hasOpacityMap)
    {
        shapeNode->SetShader("skinned_alpha");
    }
    else
    if (0 == strcmp(shapeNode->GetClass()->GetName(), "nswingshapenode"))
    {
        //FIXME: we should have a method to determine which shader we will choose.
        shapeNode->SetShader("tree");
        shapeNode->SetShader("leaf");
    }
    else
    {
        // unknwon shapenode.
        n_maxlog(Error, "Could not specify the shader of %s", shapeNode->GetName());
    }
}

//-----------------------------------------------------------------------------
/**
    Export shader animations if it exist. (nvectoranimator is created for it)

    The followings are supported material type:
      -# MatAmbient - ambient color of the material.
      -# MatDiffuse - diffuse color of the material.
      -# MatSpecular - specular color of the material.

    @note
        Use Nebula2 custom material as it possible. 
        It support various type of material animations.
*/
void nMaxMaterial::ExportShaderAnimations(Mtl* mtl, nShapeNode* shapeNode)
{
    Control* ctrlAlpha = NULL;

    StdMat2* stdMat = static_cast<StdMat2*>(mtl);

    IParamBlock2* pblock;
    pblock = stdMat->GetParamBlockByID(std2_extended);
    if (pblock)
    {
        // check that the param block has control for alpha animation.
        Control* control = pblock->GetController(std2_opacity);
        if (control)
        {
            ctrlAlpha = control;
        }
    }

    Shader* shader;
    shader = (Shader*)stdMat->GetReference(2);
    if (NULL == shader)
    {
        return;
    }

    n_assert(SHADER_CLASS_ID == shader->SuperClassID());

    pblock = shader->GetParamBlockByID(shdr_params);
    if (pblock)
    {
        Control* ctrlColor;

        // ambient
        ctrlColor = pblock->GetController(shdr_ambient);
        if (ctrlColor)
        {
            CreateShaderAnimator(shapeNode, GetShaderParam("MatAmbient"), ctrlColor, ctrlAlpha);
        }

        // diffuse
        ctrlColor = pblock->GetController(shdr_diffuse);
        if (ctrlColor)
        {
            CreateShaderAnimator(shapeNode, GetShaderParam("MatDiffuse"), ctrlColor, ctrlAlpha);
        }

        // specular
        ctrlColor = pblock->GetController(shdr_specular);
        if (ctrlColor)
        {
            CreateShaderAnimator(shapeNode, GetShaderParam("MatSpecular"), ctrlColor, ctrlAlpha);
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Create nVectorAnimator for 3DS Max standard material animations and add it
    to the given shapenode.

    @param shapeNode nebula node which the created animator is attached.
    @param param A valid parameter of nShaderState.
    @param ctrlColor Control for material color. 
                     (Only ambient, diffuse and specular are supported)
    @param ctrlAlpha Control for material opacity(aka alpha)
*/
void nMaxMaterial::CreateShaderAnimator(nShapeNode* shapeNode, 
                                        nShaderState::Param param, 
                                        Control* ctrlColor, Control* ctrlAlpha)
{
    if (!ctrlColor && !ctrlAlpha)
        return;

    if (param == nShaderState::InvalidParameter)
    {
        n_maxlog(Error, "Invalid shader parameter for vector animator.");
        return;
    }

    int numFrames = nMaxInterface::Instance()->GetNumFrames();;

    // retrieves alpha animation
    nArray<nMaxSampleKey> alphaKeyArray;

    if (ctrlAlpha)
    {
        nMaxControl::GetSampledKey(ctrlAlpha, alphaKeyArray, 1, nMaxFloat);
    }

    // retrieves color animations.
    nArray<nMaxSampleKey> colorKeyArray;

    const int sampleRate = 1;
    nMaxControl::GetSampledKey(ctrlColor, colorKeyArray, sampleRate, nMaxPoint3);

    nKernelServer* ks = nKernelServer::Instance();

    // make a valid name of animator. e.g. 'MatDiffuse' to 'matdiffuseanimator'
    nString animatorName;
    animatorName += nShaderState::ParamToString(param);
    animatorName += "animator";
    animatorName.ToLower();

    nVectorAnimator* animator = (nVectorAnimator*)ks->NewNoFail("nvectoranimator", animatorName.Get());
    if (animator)
    {
        // merge color and alpha values to a vector4.
        for (int i=0; i<numFrames; i++)
        {
            nMaxSampleKey colorKey = colorKeyArray[i];

            nMaxSampleKey alphaKey;

            if (ctrlAlpha)
                alphaKey = alphaKeyArray[i];

            vector4 color;
            color.x = colorKey.pos.x; // r
            color.y = colorKey.pos.y; // g
            color.z = colorKey.pos.z; // b

            if (ctrlAlpha)
                color.w = alphaKey.fval;  // a
            else
                color.w = 1.0f;

            //FIXME: what happened if color and alpha aniamtion has different time line?
            animator->AddKey(colorKey.time, color);
        }

        animator->SetParamName(nShaderState::ParamToString(param));
        animator->SetChannel("time");

        //FIXME: 'oneshot' loop type should be available too.
        animator->SetLoopType(nAnimLoopType::Loop);

        shapeNode->AddAnimator(animator->GetName());
    }
    else
        n_maxlog(Error, "Failed to create nvectoranimator for 3dsmax standard material");
}

//-----------------------------------------------------------------------------
/**
*/
int nMaxMaterial::GetNumMaps(StdMat2 *mtl)
{
    int numMaps = 0;

    for (int mapID=0; mapID<mtl->NumSubTexmaps(); mapID++)
    {
        // get the texture map.
        Texmap* texmap = mtl->GetSubTexmap(mapID);

        if (texmap && mtl->MapEnabled(mapID))
            numMaps++;
    }

    return numMaps;
}

//-----------------------------------------------------------------------------
/**
    Get material type from given max material
*/
nMaxMaterial::Type 
nMaxMaterial::GetType(Mtl* material)
{
    n_assert(material);
  
    nMaxMaterial::Type type;

    if (material->ClassID() == Class_ID(CMTL_CLASS_ID, 0) ||
        material->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
    {
        // top-bottom material or original standard material.
        type = Standard;
    }
    else
    if (material->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
    {
        // multi material.
        type = MultiSub;
    }
    else
    if (material->ClassID() == Class_ID(BAKE_SHELL_CLASS_ID, 0))
    {
        //FIXME: is 'BAKE_SHELL_CLASS_ID' right Class_ID for shell material?

        // shell material which used for backed texture.
        type = Shell;
    }
    else
    if (material->ClassID() == NEBULAMTL_CLASSID)
    {
        // nebula2 custom material.
        type = NebulaCustom;
    }
    else
    {
        type = Unknown;
    }

    return type;
}

//-----------------------------------------------------------------------------
/**
    Returns a pointer to the 'i-th' sub-texmap managed by the material or texture.

    The followings are sub-texmap index for the 3dsmax standard material.

      - ID_AM - Ambient (value 0)
      - ID_DI - Diffuse (value 1)
      - ID_SP - Specular (value 2)
      - ID_SH - Shininess (value 3). In R3 and later this is called Glossiness.
      - ID_SS - Shininess strength (value 4). In R3 and later this is called Specular Level.
      - ID_SI - Self-illumination (value 5)
      - ID_OP - Opacity (value 6)
      - ID_FI - Filter color (value 7)
      - ID_BU - Bump (value 8)
      - ID_RL - Reflection (value 9)
      - ID_RR - Refraction (value 10)
      - ID_DP - Displacement (value 11)
    
*/
Texmap* nMaxMaterial::GetSubTexmap(Mtl* mtl, int subMapIdx)
{
    Texmap* map = 0;
    if (((StdMat2*)mtl)->MapEnabled(subMapIdx))
    {
        // only valid for bitmap texture map.
        map = ((BitmapTex*)mtl)->GetSubTexmap(subMapIdx);
        if (map && mtl->ClassID() != Class_ID(BMTEX_CLASS_ID, 0))
            map = 0;
    }
    return map;
}


