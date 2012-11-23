//---------------------------------------------------------------------------
//  nmaxlight.cc
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxlight.h"
#include "export2/nmaxutil.h"
#include "export2/nmaxvectoranimator.h"
#include "export2/nmaxoptions.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "scene/nvectoranimator.h"
#include "scene/nlightnode.h"

//---------------------------------------------------------------------------
/**
*/
nMaxLight::nMaxLight()
{
}

//---------------------------------------------------------------------------
/**
*/
nMaxLight::~nMaxLight()
{
}

//---------------------------------------------------------------------------
/**
    Export light node.

    @param inode 3dsmax node.
    @param obj light object.
    @return created nebula light node.
*/
nSceneNode* nMaxLight::Export(INode* inode, Object* obj)
{
    nLightNode* createdNode = 0;

    nString lightname = nMaxUtil::CorrectName(nString(inode->GetName()));

    createdNode = static_cast<nLightNode*>(CreateNebulaObject("nlightnode", lightname.Get()));
    if (createdNode)
    {
        //TODO: check the custom attribute of the max node.
        //...

        if (!BuildLight(obj, createdNode))
        {
            n_maxlog(Error, "Failed to export light node %s.", inode->GetName());
            createdNode->Release();
            return NULL;
        }
    }

    return createdNode;
}

//---------------------------------------------------------------------------
/**
    Extract light properties and specifies it to the given light node.

    @param obj 3dsmax light node object.
    @param light Created nebula light node to export.
*/
bool nMaxLight::BuildLight(Object *obj, nLightNode* light)
{
    n_assert(light);

    TimeValue startTime	= nMaxInterface::Instance()->GetAnimStartTime();

    // contains properties of the light such as color, intensity, etc.
    LightState lightState;

    // get generic light from the object.
    GenLight* genericLight = (GenLight*)obj;

    // provides access to the lights state.
    genericLight->EvalLightState(startTime, Interval(0,0), &lightState);

    // specifies type of the light.
    switch(lightState.type)
    {
    case OMNI_LIGHT:  // point light.
        {
            float size = 0;
            light->SetType(nLight::Point);
            if (lightState.useAtten)
            {
                size = lightState.attenEnd;
            }
            else if (lightState.useNearAtten)
            {
                size = lightState.nearAttenEnd;
            }
            else
            {
                size = 500.f; // use default
            }

            // apply geom scale
            float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();
            size *= geomScale;
            
            light->SetFloat(nShaderState::LightRange, size);
            
            bbox3 box;
            box.set(vector3(), vector3(size, size, size));
            light->SetLocalBox(box);
        }
        break;

    case DIR_LIGHT:   // directional light.
        light->SetType(nLight::Directional);
        break;

    case FSPOT_LIGHT: // spot light.
    case TSPOT_LIGHT:
        {
            light->SetType(nLight::Spot);

            // apply geom scale
            float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();

            float hotsize  = lightState.hotsize * geomScale;
            float fallsize = lightState.fallsize * geomScale;
        }
        break;

    default:
        n_maxlog(Error, "The node %s is unknown light type.", light->GetName());
        return false;
    }

    // specifies activity.
    if (lightState.on)
        light->SetActive(true);
    else
        light->SetActive(false);

    // cast shadows
    light->SetCastShadows(lightState.shadow != FALSE);

    // light intensity. (The multiplier applied to the color)
    float intensity = lightState.intens;

    //specifies ambient, diffuse and specular color of the light.
    vector4 color;
    color.x = lightState.color.r;
    color.y = lightState.color.g;
    color.z = lightState.color.b;
    color.w = 1.0f;

    light->SetVector(nShaderState::LightDiffuse, color);
    light->SetVector(nShaderState::LightSpecular, color);

    //HACK: get interfaces ambient color from max interface.
    //      the ambient color is the one of the scene.
    Color ambient;
    ambient = nMaxInterface::Instance()->GetInterface()->GetAmbient(startTime, FOREVER);
    color.x = ambient.r;
    color.y = ambient.g;
    color.z = ambient.b;
    color.w = 1.0f;
    
    light->SetVector(nShaderState::LightAmbient, color);

    //TODO: any good way to specifies secondary colors for a light node?
    //      such as the param of LightDiffuse1, LIghtSpecular1 etc.
    //      One possible solution is that provides a custom attribute for that parameters.
    //...

    // export diffuse/specular color animation of the light if it exist.
    Control* control;

    control = genericLight->GetColorControl();
    if (control)
    {
        // the color of the light is animated.
        nVectorAnimator* animator = 0;

        nString paramName;

        //HACK: set the shader state param of the color animation of the light with 
        //      GenLight::GetAffectDiffuse() and GetAffectSpecular() function.
        //      This is experimental I have no idea is there any proper way for this.
        //      Do we speicifies this through custom attribute either?
        if (genericLight->GetAffectDiffuse())
            paramName += "LightDiffuse";
        else
        if (genericLight->GetAffectSpecular())
            paramName += "LightSpecular";

        nMaxVectorAnimator vectorAnimator;
        animator = static_cast<nVectorAnimator*>(vectorAnimator.Export(paramName.Get(), control));
        if (animator)
        {
            // add the animator to the light node.
            light->AddAnimator(animator->GetName());

            nKernelServer::Instance()->PopCwd();
        }
        else
        {
            n_maxlog(Error, "Failed to create nVectorAnimator %s", animator->GetName());
            return false;
        }
    }

    // export ambient color animation.
    control = nMaxInterface::Instance()->GetInterface()->GetAmbientController();
    if (control && control->NumKeys())
    {
        // the color of the light is animated.
        nVectorAnimator* animator = 0;

        nMaxVectorAnimator vectorAnimator;
        animator = static_cast<nVectorAnimator*>(vectorAnimator.Export("LightAmbient", control));
        if (animator)
        {
            // add the animator to the light node.
            light->AddAnimator(animator->GetName());

            nKernelServer::Instance()->PopCwd();
        }
        else
        {
            n_maxlog(Error, "Failed to create vector animator for ambient color of the light.");
            return false;
        }
    }

    return true;
}
