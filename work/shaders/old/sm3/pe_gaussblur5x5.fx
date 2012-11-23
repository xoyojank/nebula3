//------------------------------------------------------------------------------
//  pe_gaussblur5x5.fx
//
//  Posteffect-shader which applies a blooming
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "posteffect.fxh"
#include "posteffectsampler.fxh"

//------------------------------------------------------------------------------
/**
    UpdateSamplesGaussBlur5x5
    
    Update the sample offsets and weights for a horizontal or vertical blur.
    This is normally executed in the pre-shader.
*/
void 
UpdateSamplesGaussBlur5x5(in float pixelWidth, in float pixelHeight, in float multiplier, out float3 sampleOffsetsWeights[MaxBloomSamples])
{    
    float totalWeight = 0.0f;
    int index = 0;
    int x = -2;
	int y = -2;
	
	for (index = 0; index < MaxBloomSamples; index++)
	{
		// exclude pixels with block distance > 2, to reduce 5x5 filter
		// to 13 sample points instead of 25 (we only do 16 lookups)
		if ((abs(x) + abs(y)) <= 2)
		{
			// get unscaled Gaussian intensity
			sampleOffsetsWeights[index].xy = float2(x * pixelWidth, y * pixelHeight);
			float curWeight = GaussianDistribution(x, y, 1.0f);
			sampleOffsetsWeights[index].z = curWeight;
			totalWeight += curWeight;
		}
		else
		{
			// make sure the extra samples dont influence the result
			sampleOffsetsWeights[index] = float3(0.0f, 0.0f, 0.0f);
		}
		// next sample col
		x++;
		// next sample row?
		if(x >= 2)
		{
			x = -2;
			y++;
		}
    }
    
    // normalize weights
    int i;
    float invTotalWeightMultiplier = (1.0f / totalWeight) * multiplier;
    for (i = 0; i < index; i++)
    {
        sampleOffsetsWeights[i].z *= invTotalWeightMultiplier;
    }     
}

//------------------------------------------------------------------------------
//  Implements a 5x5 Gaussian blur filter.
//------------------------------------------------------------------------------
float4 psGaussBlur5x5(const vsInOut psIn) : COLOR
{	
    // preshader
	float3 sampleOffsetsWeights[MaxBloomSamples];
    UpdateSamplesGaussBlur5x5(pixelSize.x, pixelSize.y, 2.0f, sampleOffsetsWeights);
    
    // shader
    float4 sample = 0.0f;
    int i;
    for (i = 0; i < 12; i++)
    {
        sample += sampleOffsetsWeights[i].z * tex2D(sourceSampler, psIn.uv0 + sampleOffsetsWeights[i].xy);
    }
    return sample;
}

//------------------------------------------------------------------------------
//  Implements a Gaussian blur filter.
//------------------------------------------------------------------------------
technique t0
{
    pass p0
    {
        ZWriteEnable     = False;
        ZEnable          = False;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;        
        AlphaBlendEnable = False;
        AlphaTestEnable  = False;
        CullMode         = None;
        StencilEnable    = False;
        VertexShader     = compile vs_3_0 vsMain();
        PixelShader      = compile ps_3_0 psGaussBlur5x5();
    }
}
