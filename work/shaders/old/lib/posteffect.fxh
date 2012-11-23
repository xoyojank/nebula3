#ifndef POSTEFFECT_FXH
#define POSTEFFECT_FXH
//------------------------------------------------------------------------------
//  posteffect.fxh
//
//  Post effect shader variables and functions
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "config.fxh"

// hdr|bloom stuff
shared float  hdrBloomScale             : HdrBloomScale             = {1.0};
shared float  hdrBrightPassThreshold    : HdrBrightPassThreshold    = {1.0};
shared float4 hdrBloomColor             : HdrBloomColor             = {1.0, 1.0, 1.0, 1.0};

// general shared variables
shared float4 pixelSize                 : PixelSize;

// color vars
shared float saturation                 : Saturation                = {1.0};
shared float4 balance                   : Balance                   = {1.0f, 1.0f, 1.0f, 1.0f};
shared float4 luminance                 : Lumincance                = {0.299f, 0.587f, 0.114f, 0.0f};
shared float fadeValue                  : FadeValue                 = {1.0f};
shared float3 dofDistances              : DoFDistances              = {0.0f, 100.0f, 0.0f};

// bloom samples
static const int MaxBloomSamples = 16;

// depth of field samples
static const int MaxDofSamples = 23;
float2 DofSamples[MaxDofSamples] = {
    { 
        { 0.0, 0.0 },
        
        { -0.326212, -0.40581  },
        { -0.840144, -0.07358  },
        { -0.695914,  0.457137 },
        { -0.203345,  0.620716 },
        {  0.96234,  -0.194983 },
        {  0.473434, -0.480026 },
        {  0.519456,  0.767022 },
        {  0.185461, -0.893124 },
        {  0.507431,  0.064425 },
        {  0.89642,   0.412458 },
        { -0.32194,   0.93261f },

        {  0.326212,  0.40581  },
        {  0.840144,  0.07358  },
        {  0.695914, -0.457137 },
        {  0.203345, -0.620716 },
        { -0.96234,   0.194983 },
        { -0.473434,  0.480026 },
        { -0.519456, -0.767022 },
        { -0.185461,  0.893124 },
        { -0.507431, -0.064425 },
        { -0.89642,  -0.412458 },
        {  0.32194,  -0.93261f },
    }
};   

//------------------------------------------------------------------------------
/**
    Calculate a gaussian distribution
*/    
float
GaussianDistribution(in const float x, in const float myu, in const float rho)
{
    const float sqrt2pi = 2.5066283f;
    float g = 1.0f / (sqrt2pi * rho);
    g *= exp(-(x * x + myu * myu) / (2 * rho * rho));
    return g;
}

//------------------------------------------------------------------------------
/**
    UpdateSamplesBloom
    
    Get sample offsets and weights for a horizontal or vertical bloom filter.
    This is normally executed in the pre-shader.
*/
void
UpdateSamplesBloom(in bool horizontal, in float pixelSize, in float deviation, in float multiplier, out float3 sampleOffsetsWeights[MaxBloomSamples])
{    
    // fill center texel
    float weight = multiplier * GaussianDistribution(0.0f, 0.0f, deviation);
    sampleOffsetsWeights[0]  = float3(0.0f, 0.0f, weight);
    sampleOffsetsWeights[15] = float3(0.0f, 0.0f, 0.0f);

    // fill first half
    int i;
    for (i = 1; i < 8; i++)
    {
        if (horizontal)
        {
            sampleOffsetsWeights[i].xy = float2(i * pixelSize, 0.0f);
        }
        else
        {
            sampleOffsetsWeights[i].xy = float2(0.0f, i * pixelSize);
        }
        weight = multiplier * GaussianDistribution((float)i, 0, deviation);
        sampleOffsetsWeights[i].z = weight;
    }

    // mirror second half
    for (i = 8; i < 15; i++)
    {
        sampleOffsetsWeights[i] = sampleOffsetsWeights[i - 7] * float3(-1.0f, -1.0f, 1.0f);
    }
}

//------------------------------------------------------------------------------
/**
    Default structure for posteffect pipes
*/
struct vsInOut
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

//------------------------------------------------------------------------------
/**
    Default vertex shader
*/
vsInOut vsMain(const vsInOut vsIn)
{
    vsInOut vsOut;
    vsOut.position = vsIn.position;
    vsOut.uv0 = vsIn.uv0;
    return vsOut;
}

#endif
