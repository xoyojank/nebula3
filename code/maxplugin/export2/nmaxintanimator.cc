//-----------------------------------------------------------------------------
//  nmaxintanimator.cc
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxintanimator.h"
#include "export2/nmaxcontrol.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxcustattrib.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "gfx2/nshaderstate.h"
#include "scene/nintanimator.h"
#include "util/nanimlooptype.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxIntAnimator::nMaxIntAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxIntAnimator::~nMaxIntAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nAnimator* nMaxIntAnimator::Export(const char* paramName, Control* control)
{
    nIntAnimator* createdAnimator = 0;

    // the number of the keys in the control should be checked cause some of 
    // the node has control even it has no animations.
    if (control && control->NumKeys())
    {
        nShaderState::Param param = nShaderState::StringToParam(paramName);
        if (param == nShaderState::InvalidParameter)
        {
            n_maxlog(Error, "Invalid shader parameter for vector animator.");
            return NULL;
        }

        // e.g. convert 'AlphaRef' to 'alpharefanimator'
        nString animatorName;
        animatorName += paramName;
        animatorName += "animator";
        animatorName.ToLower();

        createdAnimator = static_cast<nIntAnimator*>(CreateNebulaObject("nintanimator", 
                                                                        animatorName.Get()));

        if (createdAnimator)
        {
            int numFrames = nMaxInterface::Instance()->GetNumFrames();;

            // retrieves sampled keys from the control.
            nArray<nMaxSampleKey> sampleKeyArray;

            int sampleRate;
            sampleRate = nMaxOptions::Instance()->GetSampleRate();

            //HACK: 3dsmax does not support int type of control, so we use float and cast it.
            nMaxControl::GetSampledKey(control, sampleKeyArray, sampleRate, nMaxFloat, true);

            // add key values to the animator.
            for (int i=0; i<sampleKeyArray.Size(); i++)
            {
                nMaxSampleKey key = sampleKeyArray[i];

                int val = (int)key.fval;

                createdAnimator->AddKey(key.time, val);
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
