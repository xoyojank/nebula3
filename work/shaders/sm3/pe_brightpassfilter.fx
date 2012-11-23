//------------------------------------------------------------------------------
//  pe_brightpass.fx
//
//  Posteffect-shader which invokes the bright pass filter
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "posteffect.fxh"
#include "util.fxh"
#include "posteffectsampler.fxh"

//------------------------------------------------------------------------------
//  Implements a bright pass filter. Not implemented: Uses the measured 1x1 luminance texture
//  as indication what areas of the picture are classified as "bright".
//------------------------------------------------------------------------------
float4 psMain(const vsInOutPostEffect psIn) : COLOR
{
    float4 sample = DecodeHDR(tex2D(sourceSampler, psIn.uv0));
    float3 brightColor = max(sample.rgb - hdrBrightPassThreshold, 0);
    bool isBright = any(brightColor);
    float luminanceResult = dot(brightColor, luminance);
    return hdrBloomColor * sample * luminanceResult * isBright;
}

//------------------------------------------------------------------------------
//  Implement the adaptive eye model bright pass filter.
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
