//-----------------------------------------------------------------------------
//  nmaxvectoranimator.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxvectoranimator.h"
#include "export2/nmaxcontrol.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxcustattrib.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "gfx2/nshaderstate.h"
#include "scene/nvectoranimator.h"
#include "util/nanimlooptype.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxVectorAnimator::nMaxVectorAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxVectorAnimator::~nMaxVectorAnimator()
{
}

//-----------------------------------------------------------------------------
/**
    Create a nVectorAnimator from the given shader parameter and control.

    FIXME: only full sampled keys are supported at the moment.
           Do we need any other control type supporting via IKeyControl?

    @param paramName A valid shader parameter name. e.g. "MatDiffuse"
    @param control Controller which is animatable. 
                   It can be retrieved from parameter block.
*/
nAnimator* 
nMaxVectorAnimator::Export(const char* paramName, Control* control)
{
    nVectorAnimator* createdAnimator = NULL;

    // the number of the keys in the control should be checked due to 
    // some of the nodes can have control even it has no animations.
    if (control && control->NumKeys())
    {
        nShaderState::Param param = nShaderState::StringToParam(paramName);
        if (param == nShaderState::InvalidParameter)
        {
            n_maxlog(Error, "Invalid shader parameter for vector animator.");
            return NULL;
        }

        // e.g. convert 'MatDiffuse' to 'matdiffuseanimator'
        nString animatorName;
        animatorName += paramName;
        animatorName += "animator";
        animatorName.ToLower();

        createdAnimator = static_cast<nVectorAnimator*>(CreateNebulaObject("nvectoranimator", 
                                                                           animatorName.Get()));

        if (createdAnimator)
        {
            int numFrames = nMaxInterface::Instance()->GetNumFrames();;

            // retrieves sampled keys from the control.
            nArray<nMaxSampleKey> sampleKeyArray;
            
            int sampleRate;
            sampleRate = nMaxOptions::Instance()->GetSampleRate();

            nMaxControl::GetSampledKey(control, sampleKeyArray, sampleRate, nMaxPoint4, true);

            // add key values to the animator.
            for (int i=0; i<sampleKeyArray.Size(); i++)
            {
                nMaxSampleKey key = sampleKeyArray[i];

                vector4 color;
                color.x = key.pt4.x; // r
                color.y = key.pt4.y; // g
                color.z = key.pt4.z; // b
                color.w = key.pt4.w; // a

                createdAnimator->AddKey(key.time, color);
            }

            createdAnimator->SetParamName(paramName);
            createdAnimator->SetChannel("time");

            // check if the channel was set in custom attributes
            nMaxCustAttrib custAttrib;
            TiXmlDocument xmlDoc;
            if (custAttrib.Convert(control, xmlDoc))
            {
                // xmlDoc.SaveFile("c:\\animatorCustAttrib.xml");
                TiXmlHandle h(&xmlDoc);
                TiXmlElement* child = h.FirstChild().FirstChild("channel").Child("", 0).Element();
                if (child)
                {
                    const char *channel = child->Attribute("value");
                    if (channel)
                    {
                        createdAnimator->SetChannel(channel);
                    }
                }
            }

            //FIXME: 'oneshot' loop type should be available too.
            createdAnimator->SetLoopType(nAnimLoopType::Loop);
        }
    }

    return createdAnimator;
}

//-----------------------------------------------------------------------------
/**
    Create a nVectorAnimator from the given shader parameter and control.

    FIXME: only full sampled keys are supported at the moment.
    Do we need any other control type supporting via IKeyControl?

    -02-Nov-06  kims  Added to handle floattab type. Thank ZHANG Zikai for the patch.

    @param paramName A valid shader parameter name. e.g. "MatDiffuse"
    @param control Controller which is animatable. 
    It can be retrieved from parameter block.
*/
nAnimator* 
nMaxVectorAnimator::Export(const char* paramName, 
                           Control* control_x, Control* control_y, Control* control_z, Control* control_w)
{
    nVectorAnimator* createdAnimator = NULL;

    Control *controls[4] = 
    {
        control_x, control_y, control_z, control_w,
    };

    // see that each of controls has any key.
    for (int i=0; i<4; i++)
    {
        if (controls[i] && controls[i]->NumKeys() == 0)
            controls[i] = 0;
    }

    // the number of the keys in the control should be checked due to 
    // some of the nodes can have control even it has no animations.
    if (controls[0] || controls[1] || controls[2] || controls[3])
    {
        nShaderState::Param param = nShaderState::StringToParam(paramName);
        if (param == nShaderState::InvalidParameter)
        {
            n_maxlog(Error, "Invalid shader parameter for vector animator.");
            return NULL;
        }

        // e.g. convert 'MatDiffuse' to 'matdiffuseanimator'
        nString animatorName;
        animatorName += paramName;
        animatorName += "animator";
        animatorName.ToLower();

        createdAnimator = static_cast<nVectorAnimator*>(CreateNebulaObject("nvectoranimator", 
            animatorName.Get()));

        if (createdAnimator)
        {
            int numFrames = nMaxInterface::Instance()->GetNumFrames();;

            // retrieves sampled keys from the control.
            nArray<nMaxSampleKey> sampleKeyArray[4];

            int sampleRate;
            sampleRate = nMaxOptions::Instance()->GetSampleRate();

            vector4 value;
            float time = 0;
            float max_time = 0;
            int size[4] = {0}, cur[4] = {0};
            for (int i = 0; i < 4; i++)
            {
                if (controls[i])
                {
                    nMaxControl::GetSampledKey(controls[i], sampleKeyArray[i], sampleRate, nMaxFloat, true);
                    size[i] = sampleKeyArray[i].Size();
                    n_assert(size[i] > 0);
                    max_time = n_max(max_time, sampleKeyArray[i][size[i] - 1].time);
                }
            }

            // add key values to the animator.
            while(cur[0] < size[0] || cur[1] < size[1] || cur[2] < size[2] || cur[3] < size[3])
            {
                float min_time = max_time + 1;
                for (int i = 0; i < 4; i++)
                {
                    if (cur[i] < size[i])
                    {
                        nMaxSampleKey key = sampleKeyArray[i][cur[i]];
                        min_time = n_min(min_time, key.time);
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    if (cur[i] < size[i])
                    {
                        nMaxSampleKey key = sampleKeyArray[i][cur[i]];
                        if (key.time <= min_time)
                        {
                            ((float*)&value)[i] = key.fval;
                            cur[i]++;
                        }
                    }
                }
                createdAnimator->AddKey(min_time, value);
            }

            createdAnimator->SetParamName(paramName);
            createdAnimator->SetChannel("time");

            //FIXME: 'oneshot' loop type should be available too.
            createdAnimator->SetLoopType(nAnimLoopType::Loop);
        }
    }

    return createdAnimator;
}
