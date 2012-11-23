//------------------------------------------------------------------------------
//  pe_downsample2x2.fx
//
//  Posteffect-shader which downsamples a texture to half it's size.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "posteffectsampler.fxh"
#include "posteffect.fxh"
#include "shared.fxh"

// a 5x5 gaussian blur filter kernel (reduced to 13 significant samples)
float3 sampleOffsetWeights[13] = {
    { -1.5,  0.5, 0.024882 },
    { -0.5, -0.5, 0.067638 },
    { -0.5,  0.5, 0.111515 },
    { -0.5,  1.5, 0.067638 },
    {  0.5, -1.5, 0.024882 },
    {  0.5, -0.5, 0.111515 },
    {  0.5,  0.5, 0.183858 },
    {  0.5,  1.5, 0.111515 },
    {  0.5,  2.5, 0.024882 },
    {  1.5, -0.5, 0.067638 },
    {  1.5,  0.5, 0.111515 },
    {  1.5,  1.5, 0.067638 },
    {  2.5,  0.5, 0.024882 },
};

//------------------------------------------------------------------------------
/**
*/
float4 psMain(const vsInOutPostEffect psIn) : COLOR
{
    float4 sample = float4(0.0, 0.0, 0.0, 0.0);
    int i;
    for (i = 0; i < 13; i++)
    {
        sample += sampleOffsetWeights[i].z * tex2D(sourceSampler, psIn.uv0 + sampleOffsetWeights[i].xy * pixelSize.xy);
    }
    return sample;
}

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
        PixelShader      = compile ps_3_0 psMain();
    }
}
