//------------------------------------------------------------------------------
//  pe_bloomhori.fx
//
//  Posteffect-shader which applies a horizontal bloom
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "posteffect.fxh"
#include "posteffectsampler.fxh"
#include "shared.fxh"

//------------------------------------------------------------------------------
//  Pixel shader which performs a vertical bloom effect.
//------------------------------------------------------------------------------
float4 psBloomVert(const vsInOutPostEffect psIn) : COLOR
{
    // preshader...
    float3 sampleOffsetsWeights[MaxBloomSamples];
    UpdateSamplesBloom(false, pixelSize.y, 3.0f, hdrBloomScale, sampleOffsetsWeights);

    // shader...
    int i;
    float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
    for (i = 0; i < MaxBloomSamples; i++)
    {
        color += sampleOffsetsWeights[i].z * tex2D(sourceSampler, psIn.uv0 + sampleOffsetsWeights[i].xy);
    }
    return color;
}

//------------------------------------------------------------------------------
//  Technique to perform the vertical bloom effect.
//------------------------------------------------------------------------------
technique tBloomHori
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
        PixelShader      = compile ps_3_0 psBloomVert();
    }
}        
