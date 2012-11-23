//------------------------------------------------------------------------------
//  unlit.fx
//  
//  shader with simple diffuse texture color, with or without alpha or/and skinning
//  
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "util.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"

// alpha blend factor
float   alphaBlendFactor : Intensity0 = 1.0f;

struct vsColorPassInput
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;    
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsColorPassOutput
ColorPassVertexShader(const vsColorPassInput vsIn)
{
    vsColorPassOutput vsOut;
    vsOut.position    = mul(vsIn.position, mvp);    
    vsOut.uv0         = UnpackUv(vsIn.uv0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
AlphaBlendColorPassPixelShader(const vsColorPassOutput psIn) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0);    
    color.a *= alphaBlendFactor; 
    color = EncodeHDR(color);  
    return color;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;

//------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/        
SimpleTechnique(Alpha, "Alpha", ColorPassVertexShader, AlphaBlendColorPassPixelShader);