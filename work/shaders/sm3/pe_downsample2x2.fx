//------------------------------------------------------------------------------
//  pe_downsample2x2.fx
//
//  Posteffect-shader which downsamples a texture to half it's size.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "posteffectsampler.fxh"
#include "shared.fxh"

//------------------------------------------------------------------------------
/**
    vsMain in posteffect.fxh
*/

//------------------------------------------------------------------------------
/**
*/
float4 psMain(const vsInOutPostEffect psIn) : COLOR
{
    // take 4 samples around current fragment
    float4 c0 = tex2D(sourceSampler, psIn.uv0 + pixelSize.xy * float2(-0.5, 0.0));
    float4 c1 = tex2D(sourceSampler, psIn.uv0 + pixelSize.xy * float2(+0.5, 0.0));
    float4 c2 = tex2D(sourceSampler, psIn.uv0 + pixelSize.xy * float2(0.0, +0.5));
    float4 c3 = tex2D(sourceSampler, psIn.uv0 + pixelSize.xy * float2(0.0, -0.5));
    return (c0 + c1 + c2 + c3) * 0.25;
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
