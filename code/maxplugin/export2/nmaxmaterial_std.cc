//-----------------------------------------------------------------------------
//  nmaxmaterial_cstm.cc
//
//  Handles standard material of 3dsmax.
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"

#include "export2/nmaxmaterial.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxtexture.h"

#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"
#include "export2/nmaxcontrol.h"

#include "scene/nshapenode.h"

//-----------------------------------------------------------------------------
/**
    Export standard material of 3dsmax.

    @note
    From 3DSMax SDK Help

    Converting material specular values to D3D

    For Max's viewport rendering it simply takes the value of GetShininess 
    and multiplies by 100.0f and uses this as the power value. 
    To set the specular element for the material it simple takes the Specular 
    color and multiplies by GetShinStr().

    @param mtl pointer to the material which to export.
    @param shapeNode shape node which to be specified.
*/
void nMaxMaterial::GetStandardMaterial(Mtl* mtl, nShapeNode* shapeNode)
{
    nMaxMaterial::Type mtlType;
    mtlType = nMaxMaterial::GetType(mtl);
    if (mtlType != nMaxMaterial::Standard)
    {
        CreateDefaultMaterial(shapeNode);
        return ;
    }
    else
    {
        StdMat2* stdMat = (StdMat2*)mtl;

        stdMat->Update(0, FOREVER);

        TimeValue animStart = nMaxInterface::Instance()->GetAnimStartTime();

        Color ambient  = stdMat->GetAmbient(animStart);
        Color diffuse  = stdMat->GetDiffuse(animStart);
        Color specular = stdMat->GetSpecular(animStart);

        float selfIillum = stdMat->GetSelfIllum(animStart); // not used for nebula
        float shiness    = stdMat->GetShininess(animStart); // not used for nebula
        float opacity    = stdMat->GetOpacity(animStart);
        float shinStr    = stdMat->GetShinStr(animStart);

        vector4 nAmbient  (ambient.r,  ambient.g,  ambient.b,  opacity); ClampColor(nAmbient);
        vector4 nDiffuse  (diffuse.r,  diffuse.g,  diffuse.b,  opacity); ClampColor(nDiffuse);
        vector4 nSpecular (specular.r, specular.g, specular.b, opacity); ClampColor(nSpecular);

        shapeNode->SetVector(nShaderState::MatAmbient,  nAmbient);
        shapeNode->SetVector(nShaderState::MatDiffuse,  nDiffuse);
        shapeNode->SetVector(nShaderState::MatSpecular, nSpecular);

        // we use shininess strength for specular power.
        // it has [0,1] range so we need it to convert nebula range by multiplying 255.
        float specularPower = 255.0f * shinStr;

        // if the specular power is 0.0f, all mesh is drawn to black.
        if (specularPower < 0.000001f)
            specularPower = 0.000001f;
        shapeNode->SetFloat(nShaderState::MatSpecularPower, specularPower);

        if (stdMat->GetTransparencyType() == TRANSP_SUBTRACTIVE)
        {
            shapeNode->SetInt(nShaderState::AlphaSrcBlend, 1/*Zero*/);
            shapeNode->SetInt(nShaderState::AlphaDstBlend, 3/*SrcColor*/);
        }
        else 
        if (stdMat->GetTransparencyType() == TRANSP_ADDITIVE)
        {
            shapeNode->SetInt(nShaderState::AlphaSrcBlend, 5/*SrcAlpha*/);
            shapeNode->SetInt(nShaderState::AlphaDstBlend, 2/*One*/);
        }
        bool hasAlpha = this->SetAlphaParam(stdMat, shapeNode);

        // self-illumination is enabled
        //if (stdMat->MapEnabled(ID_SI))
        //{
        //    Texmap* tex;
        //    tex = (BitmapTex*)stdMat->GetSubTexmap(ID_SI);
        //    if (tex)
        //    {
        //        ;
        //    }
        //}

        Texmap* opacity_map  = stdMat->GetSubTexmap(ID_OP);
        this->SetStandardNebulaShader(shapeNode, opacity_map != 0);

        nMaxTexture texture;
        for (int i=0; i<stdMat->NumSubTexmaps(); i++)
        {
            Texmap* subTexmap = stdMat->GetSubTexmap(i);
            float amount = 1.0f;

            if (subTexmap)
            {
                if (nMaxMaterial::IsClassID(stdMat, DMTL_CLASS_ID))
                {
                    if (!stdMat->MapEnabled(i))
                        continue;

                    TimeValue time = nMaxInterface::Instance()->GetAnimStartTime();
                    amount = stdMat->GetTexmapAmt(i, time);
                }

                texture.Export(subTexmap, i, shapeNode);
            }
        }
    }

    // export shader animations.
    if (IsClassID(mtl, DMTL_CLASS_ID))
    {
        // we only export shader animation for original standard material type.
        ExportShaderAnimations(mtl, shapeNode);
    }
}
