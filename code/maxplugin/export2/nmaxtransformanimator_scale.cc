//-----------------------------------------------------------------------------
//  nmaxtransformanimator_scale.cc
//
//  (C)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxcontrol.h"
#include "export2/nmaxtransformanimator.h"
#include "export2/nmaxtransform.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxinterface.h"
#include "pluginlibs/nmaxdlg.h"
#include "pluginlibs/nmaxlogdlg.h"

#include "kernel/ntypes.h"
#include "scene/ntransformanimator.h"

//-----------------------------------------------------------------------------
/**
    Export scale animation depends on the type of the controller which is used 
    for the  scale animatoion.

    @param control Controller which used for the animation.
    @param animator transform animator for scale animation.

    @return the number of the keys which to be used for the actual animation.
*/
int nMaxTransformAnimator::ExportScale(Control *control, nTransformAnimator* animator)
{
    if (control && control->NumKeys())
    {
        IKeyControl* iKeyControl = GetKeyControlInterface(control);
        if (iKeyControl)
        {
            int numKeys = iKeyControl->GetNumKeys();
            if (numKeys > 0)
            {
                nMaxControl::Type type = nMaxControl::GetType(control);

                switch(type)
                {
                case nMaxControl::TCBScale:
                    return ExportTCBScale(iKeyControl, numKeys, animator);

                case nMaxControl::HybridScale:
                    return ExportHybridScale(iKeyControl, numKeys, animator);

                case nMaxControl::LinearScale:
                    return ExportLinearScale(iKeyControl, numKeys, animator);

                default:
                    return ExportSampledScale(animator);
                }
            }
            else
            {
                // we have Controller but it has no keys.
                n_maxlog(Warning, "The node '%s' has Control but no keys.", this->maxNode->GetName());
                return 0;
            }
        }
        else
        {
            return ExportSampledScale(animator);
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/**
    -03-Jun-06  kims  Fixed to use second key value if there is no first one 
                      when its start time.
                      Thanks Kim, Seung Hoon for this fix.

    @note
    Only scale value is exported. The quaternion value is not exported.

    @return the number of the keys which to be used for the actual animation.
*/
int nMaxTransformAnimator::ExportTCBScale(IKeyControl* ikc, int numKeys, 
                                          nTransformAnimator* animator)
{
    n_maxlog(Warning, "The TCB controller is used for scaling. Only scale value will be exported.");

    TimeValue start = nMaxInterface::Instance()->GetAnimStartTime();
    TimeValue end   = nMaxInterface::Instance()->GetAnimEndTime();

    nArray<ITCBScaleKey> keyArray;

    // collect key
    for (int i=0; i<numKeys; i++)
    {
        ITCBScaleKey key;
        ikc->GetKey(i, &key);

        if( key.time < start || end < key.time )
            continue;

        keyArray.Append(key);
    }

    // start key
    if( keyArray.Size() == 0 || keyArray.Front().time > start )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(start), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        animator->AddScaleKey(0.0f, scale);
    }

    // key array
    for( int i = 0; i < keyArray.Size(); ++i )
    {
        vector3 scale;
        scale.x = keyArray[i].val.s.x;
        scale.y = keyArray[i].val.s.z;
        scale.z = keyArray[i].val.s.y;

        float time = (keyArray[i].time - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }

    // end key
    if( keyArray.Size() == 0 || keyArray.Back().time != end )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(end), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        float time =  (end - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }

    return numKeys;
}

//-----------------------------------------------------------------------------
/**
    -03-Jun-06  kims  Fixed to use second key value if there is no first one 
                      when its start time.
                      Thanks Kim, Seung Hoon for this fix.

    @note
    Only scale value is exported. Any tangent or in/out length values are
    not exported.

    @return the number of the keys which to be used for the actual animation.
*/
int nMaxTransformAnimator::ExportHybridScale(IKeyControl* ikc, int numKeys, 
                                             nTransformAnimator* animator)
{
    n_maxlog(Warning, "Warning: The bezier controller is used for scaling. Only scale value will be exported.");

    TimeValue start = nMaxInterface::Instance()->GetAnimStartTime();
    TimeValue end   = nMaxInterface::Instance()->GetAnimEndTime();

    nArray<IBezScaleKey> keyArray;

    // collect key
    for (int i=0; i<numKeys; i++)
    {
        IBezScaleKey key;
        ikc->GetKey(i, &key);

        if( key.time < start || end < key.time )
            continue;

        keyArray.Append(key);
    }

     // start key
    if( keyArray.Size() == 0 || keyArray.Front().time > start )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(start), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        animator->AddScaleKey(0.0f, scale);
    }

    // key array
    for( int i = 0; i < keyArray.Size(); ++i )
    {
        vector3 scale;
        scale.x = keyArray[i].val.s.x;
        scale.y = keyArray[i].val.s.z;
        scale.z = keyArray[i].val.s.y;

        float time = (keyArray[i].time - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }

    // end key
    if( keyArray.Size() == 0 || keyArray.Back().time != end )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(end), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        float time =  (end - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }


    return numKeys;
}

//-----------------------------------------------------------------------------
/**
    Interpolates between animation keys by evenly dividing the change from 
    one key value to the next by the amount of time between the keys.

    -03-Jun-06  kims  Fixed to use second key value if there is no first one 
                      when its start time.
                      Thanks Kim, Seung Hoon for this fix.

    @return the number of the keys which to be used for the actual animation.
*/
int nMaxTransformAnimator::ExportLinearScale(IKeyControl* ikc, int numKeys, 
                                             nTransformAnimator* animator)
{
    TimeValue start = nMaxInterface::Instance()->GetAnimStartTime();
    TimeValue end   = nMaxInterface::Instance()->GetAnimEndTime();

    nArray<ILinScaleKey> keyArray;

    // collect key
    for (int i=0; i<numKeys; i++)
    {
        ILinScaleKey key;
        ikc->GetKey(i, &key);

        if( key.time < start || end < key.time )
            continue;

        keyArray.Append(key);
    }

    // start key
    if( keyArray.Size() == 0 || keyArray.Front().time > start )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(start), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        animator->AddScaleKey(0.0f, scale);
    }

    // key array
    for( int i = 0; i < keyArray.Size(); ++i )
    {
        vector3 scale;
        scale.x = keyArray[i].val.s.x;
        scale.y = keyArray[i].val.s.z;
        scale.z = keyArray[i].val.s.y;

        float time = (keyArray[i].time - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }

    // end key
    if( keyArray.Size() == 0 || keyArray.Back().time != end )
    {
        AffineParts ap;
        decomp_affine( maxNode->GetNodeTM(end), &ap );

        vector3 scale;
        scale.x = ap.k.x;
        scale.y = ap.k.z;
        scale.z = ap.k.y;

        float time =  (end - start) * SECONDSPERTICK;
        animator->AddScaleKey(time, scale);
    }


    return numKeys;
}

//-----------------------------------------------------------------------------
/**
    Export sampled key scale animation.

    -03-Jun-06  kims  Fixed to use second key value if there is no first one 
                      when its start time.
                      Thanks Kim, Seung Hoon for this fix.

    @return the number of the keys which to be used for the actual animation.
*/
int nMaxTransformAnimator::ExportSampledScale(nTransformAnimator* animator)
{
    int sampleRate = nMaxOptions::Instance()->GetSampleRate();

    nArray<nMaxSampleKey> sampleKeyArray;

    nMaxControl::GetSampledKey(this->maxNode, sampleKeyArray, sampleRate, nMaxScale, true);

    // assign sample keys to animator.
    for (int i=0; i<sampleKeyArray.Size(); i++)
    {
        nMaxSampleKey sampleKey = sampleKeyArray[i];

        float time = sampleKey.time;

        //HACK: note that we only support uniform scale at the moment.
        vector3 scale;
        scale.x = sampleKey.pos.x;
        scale.y = sampleKey.pos.z;
        scale.z = sampleKey.pos.y;

        animator->AddScaleKey(time, scale);
    }

    return sampleKeyArray.Size();
}
