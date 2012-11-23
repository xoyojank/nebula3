//------------------------------------------------------------------------------
//  pe_copy.fx
//
//  A simple copy posteffect shader.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "posteffectsampler.fxh"
#include "posteffect.fxh"

//------------------------------------------------------------------------------
/**
    vsMain in posteffect.fxh
*/

//------------------------------------------------------------------------------
/**
*/
float4 psMain(const vsInOutPostEffect psIn) : COLOR
{
    return tex2D(sourceSampler, psIn.uv0);
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
