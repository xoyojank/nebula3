//------------------------------------------------------------------------------
//  gui.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "util.fxh"

texture guiTexture : DiffMap0;
sampler guiSampler = sampler_state
{
    Texture = <guiTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
};

struct vsInOut
{
    float4 position : POSITION;
    float4 color    : COLOR0;
    float2 uv0      : TEXCOORD0;
};

//------------------------------------------------------------------------------
/**
*/
vsInOut vsMain(const vsInOut vsIn)
{
    vsInOut vsOut;
    vsOut.position = vsIn.position;
    vsOut.uv0 = vsIn.uv0;
    vsOut.color = vsIn.color;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader: generate color values for static geometry.
*/
float4 psMain(const vsInOut psIn) : COLOR
{        
    float4 guiColor = tex2D(guiSampler, psIn.uv0);
    guiColor *= psIn.color;
    guiColor.rgb *= guiColor.a;  // Premultiplied alpha
    return guiColor;
}

//------------------------------------------------------------------------------
/**
    Techniques for shader "gui"
*/
technique t0
{
    pass p0
    {
        ZEnable           = False;
        ZWriteEnable      = False;
        ZFunc             = LessEqual;
        AlphaBlendEnable  = True;
        SrcBlend          = One; // Premultiplied in pixel shader
        DestBlend         = InvSrcAlpha;
        AlphaTestEnable   = false;
        CullMode          = None;
        VertexShader = compile vs_3_0 vsMain();
        PixelShader  = compile ps_3_0 psMain();
    }
}
