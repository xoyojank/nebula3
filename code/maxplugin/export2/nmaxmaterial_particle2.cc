//-----------------------------------------------------------------------------
//  nmaxmaterial_particle2.cc
//
//  Retrieves particle2 custom attributes from 3dsmax material editor.
//
//  (c)2006 Kim, Hyoun Woo
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
#include "mathlib/envelopecurve.h"
#include "particle/nparticle2emitter.h"
#include "scene/nparticleshapenode2.h"

nParticle2Emitter::CurveType GetCurveTpe(TCHAR* name);

//-----------------------------------------------------------------------------
/**
*/
void nMaxMaterial::GetParticle2Material(Mtl* mtl, nShapeNode* shapeNode)
{
    if (NULL == mtl || NULL == shapeNode)
        return;

    nParticleShapeNode2* particleNode = static_cast<nParticleShapeNode2*>(shapeNode);

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

                int numParamBlocks = custAttr->NumParamBlocks();

                for (int i=0; i<numParamBlocks; i++)
                {
                    // NOTE: the only valid parameter block which can be exported is 
                    //       the custom attribute's one but we process it with general way.
                    IParamBlock2* pblock2 = custAttr->GetParamBlock(i);

                    // get the number of the params.
                    int numParams = pblock2->NumParams();

                    int paramType;

                    for (int j=0; j<numParams; j++)
                    {
                        ParamBlockDesc2* pblockDesc = pblock2->GetDesc();
                        if (NULL == pblockDesc)
                            return;

                        ParamID paramID = pblockDesc->IndextoID(j);
                        ParamDef& paramDef = pblockDesc->GetParamDef(paramID);
                        TCHAR* name = paramDef.int_name;
                        
                        paramType = pblock2->GetParameterType(j);

                        switch(paramType)
                        {
                        case TYPE_FLOAT:
                            {
                                BOOL result;
                                float value;
                                Interval interval;
                                result = pblock2->GetValue(j, 0, value, interval);
                                if (result)
                                {
		                            if (strcmp(name, "EmissionDuration") == 0)
                                    {
                                        particleNode->SetEmissionDuration(value);
                                    }
                                    else
                                    if (strcmp(name, "ActivityDistance") == 0)
                                    {
                                        particleNode->SetActivityDistance(value);
                                    }
                                    else
                                    if (strcmp(name, "StartRotationMin") == 0)
                                    {
                                        particleNode->SetStartRotationMin(value);
                                    }
                                    else
                                    if (strcmp(name, "StartRotationMax") == 0)
                                    {
                                        particleNode->SetStartRotationMax(value);
                                    }
                                    else
                                    // HACK: 3ds max changes the name Gravity to gravity, so use gravity here
                                    if (strcmp(name, "gravity") == 0) 
                                    {
                                        particleNode->SetGravity(value);
                                    }
                                    else
                                    if (strcmp(name, "ParticleStretch") == 0)
                                    {
                                        particleNode->SetParticleStretch(value);
                                    }
                                    else
                                    if (strcmp(name, "ParticleVelocityRandomize") == 0)
                                    {
                                        particleNode->SetParticleVelocityRandomize(value);
                                    }
                                    else
                                    if (strcmp(name, "ParticleRotationRandomize") == 0)
                                    {
                                        particleNode->SetParticleRotationRandomize(value);
                                    }
                                    else
                                    if (strcmp(name, "ParticleSizeRandomize") == 0)
                                    {
                                        particleNode->SetParticleSizeRandomize(value);
                                    }
                                    else
                                    if (strcmp(name, "PrecalcTime") == 0)
                                    {
                                        particleNode->SetPrecalcTime(value);
                                    }
                                    else
                                    if (strcmp(name, "StartDelay") == 0)
                                    {
                                        particleNode->SetStartDelay(value);
                                    }
                                    else
                                    {
                                        n_maxlog(Error, "Failed to retrieve the value of the parameter %s.", name);
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
                                {
                                    bool bvalue = value > 0 ? true : false;
                                    // HACK: 3ds max changes the name Loop to loop, so use loop here
                                    if (strcmp(name, "loop") == 0)
                                    {
                                        particleNode->SetLoop(bvalue);
                                    }
                                    else if (strcmp(name, "StretchToStart") == 0)
                                    {
                                        particleNode->SetStretchToStart(bvalue);
                                    }
                                    else if (strcmp(name, "RandomRotDir") == 0)
                                    {
                                        particleNode->SetRandomRotDir(bvalue);
                                    }
                                    else if (strcmp(name, "ViewAngleFade") == 0)
                                    {
                                        particleNode->SetViewAngleFade(bvalue);
                                    }
                                    else if (strcmp(name, "RenderOldestFirst") == 0)
                                    {
                                        particleNode->SetRenderOldestFirst(bvalue);
                                    }
                                    else if (strcmp(name, "EmitOnSurface") == 0)
                                    {
                                        particleNode->SetEmitOnSurface(bvalue);
                                    }
                                }
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
                                    if (strcmp(name, "TileTexture") == 0)
                                    {
                                        particleNode->SetTileTexture(value);
                                    }
                                    else if (strcmp(name, "StretchDetail") == 0)
                                    {
                                        particleNode->SetStretchDetail(value);
                                    }
                                }
                            }
                            break;
                        case TYPE_FLOAT_TAB:
                            {
                                BOOL result;
                                float values[14] = {0};
                                Interval interval;

                                short tabSize = paramDef.tab_size;

                                // get values of parameters block.
                                for (int p=0; p<tabSize; ++p)
                                {
                                    result &= pblock2->GetValue(j, 0, values[p],interval, p);
                                }

                                //FIXME: onlue nParticle2ShapeNode::SetRGBCurve() has 14 in-args.
                                if (tabSize > 11)
                                {
                                    n_assert2(tabSize == 14, "It is not particle rgb.\n");

                                    particleNode->SetRGBCurve(nVector3EnvelopeCurve (
                                        vector3(values[0], values[1], values[2]),
                                        vector3(values[3], values[4], values[5]),
                                        vector3(values[6], values[7], values[8]),
                                        vector3(values[9], values[10], values[11]),
                                        values[12], values[13]));
                                }
                                else
                                {
                                    nParticle2Emitter::CurveType curveType;
                                    curveType = GetCurveTpe(name);

                                    // for previously version
                                    if( values[9] == 0.0f && values[10] == 0.0f)
                                    {
                                        // empty
                                    }
                                    // use min max
                                    else
                                    {
                                        values[0] = n_lerp(values[9], values[10], values[0]);
                                        values[1] = n_lerp(values[9], values[10], values[1]);
                                        values[2] = n_lerp(values[9], values[10], values[2]);
                                        values[3] = n_lerp(values[9], values[10], values[3]);
                                    }

                                    nEnvelopeCurve envelopeCurve 
                                    (
                                        values[0], values[1], values[2], values[3],
                                        values[4], values[5], values[6], values[7],
                                        (int)(values[8])
                                    );

                                    if (curveType < nParticle2Emitter::CurveTypeCount)
                                        particleNode->SetCurve(curveType, envelopeCurve);
                                }
                            }
                            break;
                        case TYPE_STRING:
                        case TYPE_TEXMAP:
                            {
                                // HACK: ignore "Shader", "DiffMap0", "dirSettingDiffMap0" in particle
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
*/
static
nParticle2Emitter::CurveType GetCurveTpe(TCHAR* name)
{
    static struct CurveTypeInfo
    {
        const char *name;
        nParticle2Emitter::CurveType type;
    } curvetypes[] = 
    {
        {"EmissionFrequency",           nParticle2Emitter::EmissionFrequency, },
        {"ParticleLifeTime",            nParticle2Emitter::ParticleLifeTime, },
        {"ParticleSpreadMin",           nParticle2Emitter::ParticleSpreadMin, },
        {"ParticleSpreadMax",           nParticle2Emitter::ParticleSpreadMax, },
        {"ParticleStartVelocity",       nParticle2Emitter::ParticleStartVelocity, },
        {"ParticleRotationVelocity",    nParticle2Emitter::ParticleRotationVelocity, },
        {"ParticleScale",               nParticle2Emitter::ParticleScale, },
        {"ParticleAlpha",               nParticle2Emitter::ParticleAlpha, },
        {"ParticleMass",                nParticle2Emitter::ParticleMass, },
        {"TimeManipulator",             nParticle2Emitter::TimeManipulator, },
        {"ParticleAirResistance",       nParticle2Emitter::ParticleAirResistance, },
        {"ParticleVelocityFactor",      nParticle2Emitter::ParticleVelocityFactor, },
    };
    for (int i = 0; i < sizeof(curvetypes) / sizeof(curvetypes[0]); i++)
    {
        if (strcmp(curvetypes[i].name, name) == 0)
            return curvetypes[i].type;
    }
    return nParticle2Emitter::CurveTypeCount; // invalid param
}
