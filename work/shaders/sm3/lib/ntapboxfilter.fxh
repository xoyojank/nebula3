#ifndef NTAPBOXFILTER_H
#define NTAPBOXFILTER_H
//------------------------------------------------------------------------------
//  ntapboxfilter.fxh
//
//  Posteffect-shader which applies a generic separable box filter (up to 7 taps) in shadow mapping pass
//
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "util.fxh"
#include "shared.fxh"

#ifdef Hori
    #define DIRECTION   float2(1.0, 0.0)
#endif
#ifdef Vert
    #define DIRECTION   float2(0.0, 1.0)
#endif

#ifdef Taps3
    #define STEP_TAPS   3
#endif
#ifdef Taps5
    #define STEP_TAPS   5
#endif
#ifdef Taps7
    #define STEP_TAPS   7
#endif

//---------------------------------------------------------------------------------------------------------------------
void
GenerateFilterSteps(float2 uv, out float2 result[STEP_TAPS])
{
    // calulate some uv's   
    float2 texelSize = 1.0f / float2(shadowConstants.w, shadowConstants.w);
    float2 step = DIRECTION * texelSize;
    float2 base = uv - ((((float)STEP_TAPS - 1)*0.5f) * step);
      
    for (int i = 0; i < STEP_TAPS; i++)
    {
        result[i] = base;
        base += step;
    }
}

//---------------------------------------------------------------------------------------------------------------------
float 
FilterTaps(float2 uvs[STEP_TAPS],
           sampler2D srcSampler)
{
    float samples[STEP_TAPS]; 
    const float c = (1.f/float(STEP_TAPS));    
    
#if SHADOW_DEPTH_PROJSPACE
    float filteredValue = 0;
    int index;
    for (index = 0; index < STEP_TAPS; index++)
    {
        filteredValue += tex2D( sourceSampler, uvs[index] ).x;
    }   
    filteredValue *= c;
#else
    int index;    
    for (index = 0; index < STEP_TAPS; index++)
    {
        samples[index] = tex2D( sourceSampler, uvs[index] ).x;
    }
    float filteredValue = log_conv( c, samples[0], c, samples[1] );
    for (index = 2; index < STEP_TAPS; index++)
    {
        filteredValue = log_conv( 1.f, filteredValue, c, samples[index] );
    }   
#endif   
    return filteredValue;
}

//---------------------------------------------------------------------------------------------------------------------
struct vsInOut
{
    float4 position : POSITION;
    float4 taps1_2  : TEXCOORD0;
#ifdef Taps3 
    float2 taps3_4  : TEXCOORD1;
#endif
#ifdef Taps5 
    float4 taps3_4  : TEXCOORD1;
    float2 taps5    : TEXCOORD2;
#endif
#ifdef Taps7
    float4 taps3_4  : TEXCOORD1;
    float4 taps5_6  : TEXCOORD2;
    float2 taps7    : TEXCOORD3;
#endif
};


//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------- ESM DEPTH N TAPS --------------------------------------------------
/**
    Implements a generic separable box filter    
*/
void
vsMainBoxFilter(float4 position     : POSITION,
                float2 uv           : TEXCOORD0,
                out vsInOut vsOut)
{
    // just pass the fullscreenquad pos
    vsOut.position = position;
    float2 taps[STEP_TAPS];
    GenerateFilterSteps(uv, taps);
    vsOut.taps1_2.xy = taps[0];
    vsOut.taps1_2.zw = taps[1];
    vsOut.taps3_4.xy = taps[2];
 #if (STEP_TAPS > 3)    
    vsOut.taps3_4.zw = taps[3];   
    vsOut.taps5_6.xy = taps[4];
 #if (STEP_TAPS > 5)    
    vsOut.taps5_6.zw = taps[5];
    vsOut.tap7       = taps[6];
 #endif
 #endif
}
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
    Implements a generic separable box filter
*/
void
psMain(const vsInOut psIn,
            out float4 outDepth : COLOR)
{   
    float2 uvs[STEP_TAPS] = { psIn.taps1_2.xy, psIn.taps1_2.zw, psIn.taps3_4.xy, 
                            #if STEP_TAPS > 3
                              psIn.taps3_4.zw, psIn.taps5_6.xy, 
                            #if STEP_TAPS > 5
                              psIn.taps5_6.zw, psIn.tap7
                            #endif
                            #endif
                            };
                              
    outDepth = FilterTaps(uvs, sourceSampler);
}
#endif

