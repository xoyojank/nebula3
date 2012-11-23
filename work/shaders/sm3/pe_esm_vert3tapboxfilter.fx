//------------------------------------------------------------------------------
//  pe_esm_vert3tapboxfilter.fx
//
//  Post Process for blurring shadowmap
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "posteffectsampler.fxh"

#define Vert
#define Taps3
#include "ntapboxfilter.fxh"

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
        VertexShader     = compile vs_3_0 vsMainBoxFilter();
        PixelShader      = compile ps_3_0 psMain();
    }
}