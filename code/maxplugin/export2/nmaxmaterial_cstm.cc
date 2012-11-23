//-----------------------------------------------------------------------------
//  nmaxmaterial_cstm.cc
//
//  Handles Nebula2 custom material of 3dsmax.
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
#include "export2/nmaxintanimator.h"
#include "export2/nmaxfloatanimator.h"
#include "export2/nmaxvectoranimator.h"
#include "export2/nmaxoptions.h"

#include "kernel/nfileserver2.h"
#include "scene/nshapenode.h"
#include "scene/nintanimator.h"
#include "scene/nvectoranimator.h"
#include "scene/nfloatanimator.h"

const char* nMaxMaterial::predefinedCA[NUM_PREDEFINED_CA] = {
    "Viewport Manager",
    "DirectX Manager",
    "mental ray: material custom attribute"
};

//-----------------------------------------------------------------------------
/**
    Retrieves material values from Mtl's parameter block's parameters then
    specifies it to the given shape node.

    -02-Nov-06  kims  Added the case TYPE_FLOAT_TAB for parameter type.
                      Thank ZHANG Zikai for the patch.

    @param mtl pointer to the Mtl which contains material of the node.
    @param shapeNode pointer to the nebula shape node to be specified its material.
*/
void nMaxMaterial::GetNebulaMaterial(Mtl* mtl, nShapeNode* shapeNode)
{
    if (NULL == mtl || NULL == shapeNode)
        return;

    ICustAttribContainer* caContainer = mtl->GetCustAttribContainer();
    if (caContainer)
    {
        // we assume that there's only one custom attribute.
        int numAttribs = caContainer->GetNumCustAttribs();

        CustAttrib* custAttr = 0;

        for (int ca=0; ca<numAttribs; ca++)
        {
            custAttr = (CustAttrib*)caContainer->GetCustAttrib(ca);

            if (custAttr)
            {
                // we only deal with nebula2 custom material's CA. skip others.
                TCHAR* caName = custAttr->GetName();
                if (IsPredefinedCA(caName))
                    continue;

                shapeNode->SetMayaShaderName(caName);

                int numParamBlocks = custAttr->NumParamBlocks();

                for (int i=0; i<numParamBlocks; i++)
                {
                    // NOTE: the only valid parameter block which can be exported is 
                    //       the custom attribute's one but we process it with general way.
                    IParamBlock2* pblock2 = custAttr->GetParamBlock(i);

                    // get the number of the params.
                    int numParams = pblock2->NumParams();

                    nShaderState::Param shaderParam;
                    int paramType;

                    for (int j=0; j<numParams; j++)
                    {
                        ParamBlockDesc2* pblockDesc = pblock2->GetDesc();
                        if (NULL == pblockDesc)
                            return;

                        ParamID paramID = pblockDesc->IndextoID(j);
                        ParamDef& paramDef = pblockDesc->GetParamDef(paramID);

                        TCHAR* name = paramDef.int_name;
                        
                        // find nShaderState::Param which match to parameter name.
                        shaderParam = this->GetShaderParam(name);

                        // HACK: 3dsmax internally changes some name of parameters.
                        //       known names are
                        //           "Move"->"move"
                        //           "Velocity"->"velocity"
                        //           "Position"->"position"
                        //       As seen from above, the first capital characters are changed to lower characters.
                        if (shaderParam == nShaderState::InvalidParameter)
                        {
                            TCHAR *names[][2] = 
                            {
                                "Move",     "move",
                                "Velocity", "velocity",
                                "Position", "position",
                                0,          0, // keep the tail zeros
                            };

                            for (int i=0; names[i][0]; i++)
                            {
                                if (strcmp(names[i][1], name) == 0)
                                {
                                    name = names[i][0];
                                    // find the parameter name again
                                    shaderParam = this->GetShaderParam(name);

                                    // should not happen!
                                    n_assert(shaderParam != nShaderState::InvalidParameter);
                                    break;
                                }
                            }
                        }

                        // filter it if the given parameter name does not match
                        // to nebula's shader state parameter.
                        //if (shaderParam == nShaderState::InvalidParameter)
                        //    continue;

                        paramType = pblock2->GetParameterType(j);

                        switch(paramType)
                        {
                        case TYPE_STRING: //8
                            {
                                //HACK: string type used only for effect filename at the present.

                                BOOL result;

                                // Shader name
                                TCHAR* value; 
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);

                                if (result)
                                {
                                    // is this parameter for 'shader'?
                                    if (strcmp(name, "Shader") == 0)
                                    {
                                        //FIXME: other type of FourCC should be available.
                                        shapeNode->SetShader(value);
                                    }
                                    else
                                    {
                                        // HACK: we just pass it if the parameter block contains 'dirSetting' for its name.
                                        if (strstr(name, "dirSetting") == 0 )
                                        {
                                            n_maxlog(Error, "The parameter block has string type parameter but it is not an effect filename.");
                                        }
                                    }
                                }
                                else
                                {
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);
                                }
                            }
                            break;

                        case TYPE_FLOAT:
                            {
                                BOOL result;
                                float value;
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);

                                if (result)
                                    shapeNode->SetFloat(shaderParam, value);
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);

                                Control* control = pblock2->GetController(paramID);
                                if (control && control->NumKeys())
                                {
                                    // the parameter was animated.
                                    nFloatAnimator* animator = 0;

                                    nMaxFloatAnimator floatAnimator;
                                    animator = static_cast<nFloatAnimator*>(floatAnimator.Export(name, control));
                                    if (animator)
                                    {
                                        // add the animator to the shapenode.
                                        shapeNode->AddAnimator(animator->GetName());

                                        nKernelServer::Instance()->PopCwd();
                                    }
                                    else
                                    {
                                        n_maxlog(Error, "Failed to create nFloatAnimator for the parameter %s", name);
                                    }
                                }

                            }
                            break;

                        case TYPE_BOOL: //4
                            {
                                BOOL result;
                                int value;
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);

                                if (result)
                                    shapeNode->SetBool(shaderParam, ((value > 0) ? true : false));
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);
                            }
                            break;

                        case TYPE_INT:
                            {
                                BOOL result;
                                int value;
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);

                                if (result)
                                {
                                    if (strcmp(name, "RenderPri") == 0)
                                    {
                                        shapeNode->SetRenderPri(value);
                                        break;
                                    }
                                    else
                                        shapeNode->SetInt(shaderParam, value);
                                }
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);

                                Control* control = pblock2->GetController(paramID);
                                if (control && control->NumKeys())
                                {
                                    // the parameter was animated.
                                    nIntAnimator* animator = 0;

                                    nMaxIntAnimator intAnimator;
                                    animator = static_cast<nIntAnimator*>(intAnimator.Export(name, control));
                                    if (animator)
                                    {
                                        // add the animator to the shapenode.
                                        shapeNode->AddAnimator(animator->GetName());

                                        nKernelServer::Instance()->PopCwd();
                                    }
                                    else
                                    {
                                        n_maxlog(Error, "Failed to create nIntAnimator for the parameter %s", name);
                                    }
                                }
                            }
                            break;

                        case TYPE_RGBA:
                            {
                                BOOL result0, result1;
                                Color value;
                                Interval interval;
                                result0 = pblock2->GetValue(j, 0, value, interval);

                                // Get alpha value of the Color.
                                // The next parameter of the Color type always should be alpha which type is float.
                                float alpha;
                                int alphaIndex = j + 1; // next param index of the color.
                                if (alphaIndex < numParams)
                                {
                                    int type = pblock2->GetParameterType(alphaIndex);
                                    if (type == TYPE_FLOAT)
                                    {
                                        result1 = pblock2->GetValue(alphaIndex, 0, alpha, interval);
                                    }
                                }
                                
                                vector4 color;
                                color.set(value.r, value.g, value.b, alpha);
                                ClampColor(color);

                                if (result0 && result1)
                                    shapeNode->SetVector(shaderParam, color);
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);

                                Control* control = pblock2->GetController(paramID);
                                if (control)
                                {
                                    n_maxlog(Warning, "The animation of rgba type of parameter %s is not supported.", name);
                                }
                            }
                            break;

                    // The following types are Available in 3dsmax6 or higher. 
                    // See '$3dsmax/maxsdk/include/paramtype.h' for more details.
                    #if MAX_RELEASE >= 6000
                        case TYPE_FRGBA:
                            {
                                BOOL result;
                                AColor value;
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);

                                vector4 color;
                                color.set(value.r, value.g, value.b, value.a);
                                ClampColor(color);

                                if (result)
                                    shapeNode->SetVector(shaderParam, color);
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);

                                Control* control = pblock2->GetController(paramID);
                                if (control && control->NumKeys())
                                {
                                    // the parameter was animated.
                                    nVectorAnimator* animator = 0;

                                    nMaxVectorAnimator vectorAnimator;
                                    animator = static_cast<nVectorAnimator*>(vectorAnimator.Export(name, control));
                                    if (animator)
                                    {
                                        // add the animator to the shapenode.
                                        shapeNode->AddAnimator(animator->GetName());

                                        nKernelServer::Instance()->PopCwd();
                                    }
                                    else
                                    {
                                        n_maxlog(Error, "Failed to create nVectorAnimator %s", name);
                                    }
                                }
                            }
                            break;

                        case TYPE_POINT4:
                        case TYPE_FLOAT_TAB:
                            {
                                BOOL result;
                                Point4 pt4;
                                Interval interval;
                                if (paramType == TYPE_FLOAT_TAB)
                                {
                                    result = pblock2->GetValue(j, 0, pt4.x, interval, 0);
                                    result &= pblock2->GetValue(j, 0, pt4.y, interval, 1);
                                    result &= pblock2->GetValue(j, 0, pt4.z, interval, 2);
                                    result &= pblock2->GetValue(j, 0, pt4.w, interval, 3);
                                }
                                else
                                    result = pblock2->GetValue(j, 0, pt4, interval);

                                vector4 value;
                                value.set(pt4.x, pt4.y, pt4.z, pt4.w);
                                if (result)
                                    shapeNode->SetVector(shaderParam, value);
                                else
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);

                                // In the case that we have point4 type, only first control array element, control[0] is used and 
                                // for floattab, all control array elements are used.
                                Control *control[4];
                                BOOL hasKey = FALSE;
                                control[0] = pblock2->GetController(paramID);
                                if (control[0] && control[0]->NumKeys())
                                    hasKey = TRUE;
                                if (paramType == TYPE_FLOAT_TAB)
                                {
                                    control[1] = pblock2->GetController(paramID, 1);
                                    if (control[1] && control[1]->NumKeys())
                                        hasKey = TRUE;
                                    control[2] = pblock2->GetController(paramID, 2);
                                    if (control[2] && control[2]->NumKeys())
                                        hasKey = TRUE;
                                    control[3] = pblock2->GetController(paramID, 3);
                                    if (control[3] && control[3]->NumKeys())
                                        hasKey = TRUE;
                                }
                                if (hasKey)
                                {
                                    // the parameter was animated.
                                    nVectorAnimator* animator = 0;

                                    nMaxVectorAnimator vectorAnimator;

                                    if (paramType == TYPE_FLOAT_TAB)
                                    {
                                        animator = static_cast<nVectorAnimator*>(vectorAnimator.Export(name,
                                            control[0], control[1], control[2], control[3]));
                                    }
                                    else
                                    {
                                        animator = static_cast<nVectorAnimator*>(vectorAnimator.Export(name, control[0]));
                                    }

                                    if (animator)
                                    {
                                        // add the created animator to the shapenode.
                                        shapeNode->AddAnimator(animator->GetName());

                                        nKernelServer::Instance()->PopCwd();
                                    }
                                    else
                                    {
                                        n_maxlog(Error, "Failed to create nVectorAnimator %s", name);
                                    }
                                }
                            }
                            break;
                    #endif // MAX_RELEASE >= 6000

                        case TYPE_TEXMAP:
                            {
                                BOOL result0, result1;
                                Texmap* value;
                                TCHAR* mapDir = NULL;
                                nString texDir;
                                Interval interval0, interval1;
                                
                                // access to texture directory setting parameter block.
                                // we always assume the next parameter block of the texture is 
                                // texture directory setting dialog button.
                                int pbType = pblock2->GetParameterType(j+1);
                                if (pbType == TYPE_STRING)
                                {
                                    result0 = pblock2->GetValue(j+1, 0, mapDir, interval0);
                                    if (!result0)
                                    {
                                        n_maxlog(Error, "The next parameter block of texturemap should be texture directory setting");
                                        continue;
                                    }

                                    texDir = mapDir;

                                    // if the texture map directory is not specified, we use default assigned one
                                    // in the directory setting dialog.
                                    texDir = nFileServer2::Instance()->ManglePath(texDir.IsEmpty() ? nMaxOptions::Instance()->GetTextureAssign() : texDir);
                                }
                                else
                                {
                                    n_maxlog(Error, "The next parameter block of texturemap should be texture directory setting");
                                }

                                result1 = pblock2->GetValue(j, 0, value, interval1);
                                if (result1)
                                {
                                    nMaxTexture texture;
                                    texture.Export(value, shaderParam, shapeNode, texDir);
                                }
                                else
                                {
                                    n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);
                                }
                            }
                            break;

                        case TYPE_REFTARG: //18
                            {
                                BOOL result;
                                ReferenceTarget* value; 
                                Interval interval;

                                result = pblock2->GetValue(j, 0, value, interval);
                                if (result)
                                {
                                    ;//TODO: how can we get the values from the custom control such as nmaxenvelopecurve?
                                    value;
                                    int i=0;
                                    //Value* val = MAXClass::make_wrapper_for(value);
	                                //Value* val = MAXClass::make_node_for(value);
                                }
                            }
                            break;

                        default:
                            n_maxlog(Error, "Unsupported type of the parameter %s.", name);
                            break;
                        }// end of switch.
                    }//end of for each number of parameters.
                }// end of for each param blocks
            }
        }
    }
}

//-----------------------------------------------------------------------------
/**
    Retrieves matched shader param from the given script parameter name.

    @note
    parameter's internal name should be one of the member of type nShaderState::Param.
    (it's case senstive)

    @return return 'nShaderState::InvalidParameter' if there is no matched param.
*/
nShaderState::Param 
nMaxMaterial::GetShaderParam(const char* param)
{
    return nShaderState::StringToParam(param);
}
