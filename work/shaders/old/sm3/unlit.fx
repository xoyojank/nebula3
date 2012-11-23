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
#include "depthfuncs_atest.fxh"
#include "skinning.fxh"

// alpha blend factor
float   alphaBlendFactor : Intensity0 = 1.0f;

struct vsColorPassInput
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

struct vsSkinnedColorPassInput
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
    float4 weights  : BLENDWEIGHT;
    float4 indices  : BLENDINDICES;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;    
};

//------------------------------------------------------------------------------
/**
    Pixel shader function for rendering variance shadow map. Contains
    the light space depth in R, and squared depth in G.
*/
float4
VSMPassPixelShader(vsDepthPassOutput psIn) : COLOR
{
    return EncodePSSMDepth(psIn.depth.x);
}

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
    Skinned vertex shader function for the color pass.
*/
vsColorPassOutput
SkinnedColorPassVertexShader(const vsSkinnedColorPassInput vsIn)
{
    vsColorPassOutput vsOut;
    float4 skinnedPos = SkinnedPosition(vsIn.position, vsIn.weights, vsIn.indices);
    
    vsOut.position    = mul(skinnedPos, mvp);
    vsOut.uv0         = UnpackUv(vsIn.uv0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsColorPassOutput psIn) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0);     
    color = EncodeHDR(color);
    return color;
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
    // premultiplied alpha
    color.rgb *= color.a;
    return color;
}

//------------------------------------------------------------------------------
/**
    The pixel shader for skinning uses a RimLight effect.
    FIXME: maybe this should better go into a "character", or "skin" shader?
*/
float4 
SkinnedColorPassPixelShader(const vsColorPassOutput psIn) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0); 
    color = EncodeHDR(color);  
    return color;
}

//------------------------------------------------------------------------------
/**
    The pixel shader for skinning uses a RimLight effect.
    FIXME: maybe this should better go into a "character", or "skin" shader?
*/
float4 
AlphaBlendSkinnedColorPassPixelShader(const vsColorPassOutput psIn) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0);     
    color.a *= alphaBlendFactor; 
    // premultiplied alpha
    color.rgb *= color.a;  
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
SimpleTechnique(Depth, "Depth", DepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(DepthAlphaTest, "DepthATest", DepthPassAlphaTestVertexShader, DepthPassAlphaTestPixelShader);
SimpleTechnique(VSMDepth, "VSMDepth", DepthPassVertexShader, VSMPassPixelShader);

SimpleTechnique(Solid, "Solid", ColorPassVertexShader, ColorPassPixelShader);
SimpleTechnique(Alpha, "Alpha", ColorPassVertexShader, AlphaBlendColorPassPixelShader);

//------------------------------------------------------------------------------
/**
    Techniques for skinned geometry.
*/        
SimpleTechnique(SkinnedDepth, "Depth|Skinned", SkinnedDepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(SkinnedDepthAlphaTest, "DepthATest|Skinned", SkinnedDepthPassAlphaTestVertexShader, DepthPassAlphaTestPixelShader);
SimpleTechnique(SkinnedVSMDepth, "VSMDepth|Skinned", SkinnedDepthPassVertexShader, VSMPassPixelShader);

SimpleTechnique(SkinnedSolid, "Solid|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader);
SimpleTechnique(SkinnedAlpha, "Alpha|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader);
