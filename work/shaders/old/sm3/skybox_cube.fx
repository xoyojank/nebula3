//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"
#include "util.fxh"

struct vsInputCubeSkyboxColor
{
    float4 position : POSITION;
};

struct vsOutputCubeSkyboxColor
{
    float4 position : POSITION;
    float3 uv0      : TEXCOORD0;
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsOutputCubeSkyboxColor
ColorPassVertexShader(const vsOutputCubeSkyboxColor vsIn)
{    
    vsOutputCubeSkyboxColor vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    vsOut.uv0      = mul(vsIn.position, (float3x3)model);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsOutputCubeSkyboxColor psIn) : COLOR
{
    float4 baseColor = texCUBE(environmentSampler, psIn.uv0);
    baseColor = EncodeHDR(baseColor);
    return baseColor;
}

//------------------------------------------------------------------------------
/**
    Skybox is unlit without need of writing depth!
*/
//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;
//------------------------------------------------------------------------------
SimpleTechnique(Solid0, "Solid", ColorPassVertexShader, ColorPassPixelShader);
