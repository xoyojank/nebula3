//-----------------------------------------------------------------------------
//  nmaxuvanimator.cc
//
//  (c)2004 Ben Wooller
//-----------------------------------------------------------------------------

#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxuvanimator.h"
#include "export2/nmaxcontrol.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxcustattrib.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "scene/nuvanimator.h"
#include "util/nanimlooptype.h"

//-----------------------------------------------------------------------------
/**
*/
nMaxUVAnimator::nMaxUVAnimator()
{
}

//-----------------------------------------------------------------------------
/**
*/
nMaxUVAnimator::~nMaxUVAnimator()
{
}

//-----------------------------------------------------------------------------
/**
    Export UV animation.

    - 31-May-05    kims    Fixed UV tiling bug to have 1.0 for its default value (not 0.0f)
*/
nAnimator* nMaxUVAnimator::Export(Texmap* texmap)
{
    n_assert(texmap);

    int sampleRate = nMaxOptions::Instance()->GetSampleRate();

    // get frames (if any)
    nArray<nMaxSampleKey> uOffsetKeys;
    nArray<nMaxSampleKey> vOffsetKeys;
    nArray<nMaxSampleKey> uScaleKeys;
    nArray<nMaxSampleKey> vScaleKeys;
    nArray<nMaxSampleKey> uAngleKeys;
    nArray<nMaxSampleKey> vAngleKeys;
    nArray<nMaxSampleKey> wAngleKeys;

    // W Angle unsupported by nebula... TO DO: add it
    Control* c = GetControlFromAnimatable(texmap, "U Offset");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, uOffsetKeys, sampleRate, nMaxFloat, true);
    }

    c = GetControlFromAnimatable(texmap, "V Offset");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, vOffsetKeys, sampleRate, nMaxFloat, true);
    }

    c = GetControlFromAnimatable(texmap, "U Tiling");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, uScaleKeys, sampleRate, nMaxFloat, true);
    }

    c = GetControlFromAnimatable(texmap, "V Tiling");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, vScaleKeys, sampleRate, nMaxFloat, true);
    }

    c = GetControlFromAnimatable(texmap, "U Angle");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, uAngleKeys, sampleRate, nMaxFloat, true);
    }

    c = GetControlFromAnimatable(texmap, "V Angle");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, vAngleKeys, sampleRate, nMaxFloat, true);
    }
    
    c = GetControlFromAnimatable(texmap, "W Angle");
    if(c && c->NumKeys())
    {
        nMaxControl::GetSampledKey(c, wAngleKeys, sampleRate, nMaxFloat, true);
    }

    
    if(uOffsetKeys.Size() || vOffsetKeys.Size() ||
       uScaleKeys.Size()  || vScaleKeys.Size()  ||
       uAngleKeys.Size()  || vAngleKeys.Size()  || wAngleKeys.Size())
    {
        // create UV anim
        nUvAnimator* createdAnimator = static_cast<nUvAnimator*>(CreateNebulaObject("nuvanimator", "uvanimator"));
        if(!createdAnimator)
        {
          n_maxlog(Error, "Failed to create nUvAnimator.");
          return NULL;
        }

        StdUVGen* uvGen = ((BitmapTex*)texmap)->GetUVGen();

        // TO DO: there must be a way to get the channel automatically from the shader material??
        int mapChannel = uvGen->GetMapChannel() - 1; 

        // add UV Offsets
        vector2 key;
        float time = 0.0f;
        int u = 0;
        int v = 0;
        while((u < uOffsetKeys.Size()) || (v < vOffsetKeys.Size()))
        {
            if((u < uOffsetKeys.Size()) && (v < vOffsetKeys.Size()))
            {
                float uTime = uOffsetKeys[u].time;
                float vTime = vOffsetKeys[v].time;
                if(uTime <= vTime)
                {
                    key.x = uOffsetKeys[u].fval;
                    ++u;
                }

                if(vTime <= uTime)
                {
                    key.y = vOffsetKeys[v].fval;
                    ++v;
                }
                time = n_min(uTime, vTime);
            }
            else 
            if(u < uOffsetKeys.Size())
            {
                key.x = uOffsetKeys[u].fval;
                time = uOffsetKeys[u].time;
                ++u;
            }
            else // (v < vOffsetKeys.Size())
            {
                key.y = vOffsetKeys[v].fval;
                time = vOffsetKeys[v].time;
                ++v;
            }

            createdAnimator->AddPosKey(mapChannel, time, key);
        }

        // add UV Scales
        key = vector2(1.0f, 1.0f);
        time = 0.0f;
        u = 0;
        v = 0;
        while((u < uScaleKeys.Size()) || (v < vScaleKeys.Size()))
        {
            if((u < uScaleKeys.Size()) && (v < vScaleKeys.Size()))
            {
                float uTime = uScaleKeys[u].time;
                float vTime = vScaleKeys[v].time;

                if(uTime <= vTime)
                {
                    key.x = uScaleKeys[u].fval;
                    ++u;
                }

                if(vTime <= uTime)
                {
                    key.y = vScaleKeys[v].fval;
                    ++v;
                }

                time = n_min(uTime, vTime);
            }
            else 
            if(u < uScaleKeys.Size())
            {
                key.x = uScaleKeys[u].fval;
                time = uScaleKeys[u].time;
                ++u;
            }
            else // (v < vScaleKeys.Size())
            {
                key.y = vScaleKeys[v].fval;
                time = vScaleKeys[v].time;
                ++v;
            }

            createdAnimator->AddScaleKey(mapChannel, time, key);
        }

        // add UV Angles
        vector3 key3 = vector3();
        time = 0.0f;
        u = 0;
        v = 0;
        int w = 0;
        while(u < uAngleKeys.Size() || v < vAngleKeys.Size() || w < wAngleKeys.Size())
        {
            float uTime = 10000000000.0f;
            float vTime = 10000000000.0f;
            float wTime = 10000000000.0f;

            if(u < uAngleKeys.Size())
            {
                uTime = uAngleKeys[u].time;
                if (uTime <= vTime)
                {
                    key3.x = uAngleKeys[u].fval;
                    ++u;
                }
            }
            if(v < vAngleKeys.Size())
            {
                vTime = vAngleKeys[v].time;
                if (vTime <= uTime)
                {
                    key3.y = vAngleKeys[v].fval;
                    ++v;
                }
            }
            if(w < wAngleKeys.Size())
            {
                wTime = wAngleKeys[w].time;
                if (wTime <= uTime || wTime <= vTime)
                {
                    key3.z = -wAngleKeys[w].fval;
                    ++w;
                }
            }

            time = n_min(n_min(uTime, vTime), wTime);
            createdAnimator->AddEulerKey(mapChannel, time, key3);
        }

        createdAnimator->SetChannel("time");

#if 0
        // FIXME: kaiaki:i don't know how to attach custom attributes on texture maps.
        // check if the channel was set in custom attributes
        nMaxCustAttrib custAttrib;
        TiXmlDocument xmlDoc;
        if (custAttrib.Convert(texmap, xmlDoc))
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
#endif

        //FIXME: 'oneshot' loop type should be available too.
        createdAnimator->SetLoopType(nAnimLoopType::Loop);
        return createdAnimator;
    }
    return NULL;
}

//-----------------------------------------------------------------------------
/**
*/
Control* nMaxUVAnimator::GetControlFromAnimatable(Animatable* anim, char* name)
{
    if(!anim) 
    {
        return NULL;
    }

    for(int i = 0; i < anim->NumSubs(); ++i)
    {
        if(!strcmp(anim->SubAnimName(i), name))
        {
            return GetControlInterface(anim->SubAnim(i));
        }

        Control* c = GetControlFromAnimatable(anim->SubAnim(i), name);
        if(c)
        {
            return c;
        }
    }

    return NULL;
}
