#ifndef SHADOWMAPPING_FXH
#define SHADOWMAPPING_FXH
//------------------------------------------------------------------------------
//  shadowmapping.fxh
//
//  Shader programs used by shadowmap rendering
//
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "skinning.fxh"
#include "defaultsampler.fxh"

//--------------------------------------------------------------------------------------------------------------
/**
    Static Vertex Shader    
*/
void 
ShadowMappingVertexShader(float4 position     : POSITION,
                          out float4 oPos     : POSITION,
                          out float4 oProjPos : TEXCOORD0)
{
    oPos = mul(position, mvp);
    #if SHADOW_DEPTH_PROJSPACE
        oProjPos = oPos;
    #else
        oProjPos = mul(position, mv); 
    #endif
}

//--------------------------------------------------------------------------------------------------------------
/**
    Static Vertex Shader with uv coordin ates for alpha testing in pixelshader   
*/
void 
ShadowMappingATestVertexShader(float4 position     : POSITION,
                               float2 uv0          : TEXCOORD0,
                               out float4 oPos     : POSITION,
                               out float2 oUv0     : TEXCOORD0,
                               out float4 oProjPos : TEXCOORD1)
{
    oPos = mul(position, mvp);
    oUv0 = uv0;
    #if SHADOW_DEPTH_PROJSPACE
        oProjPos = oPos;
    #else
        oProjPos = mul(position, mv); 
    #endif
}

//--------------------------------------------------------------------------------------------------------------
/**
    Skinned Vertex Shader    
*/
void 
SkinnedShadowMappingVertexShader(float4 position     : POSITION,
                                 float4 weights      : BLENDWEIGHT,
                                 float4 indices      : BLENDINDICES,
                                 out float4 oPos     : POSITION,
                                 out float4 oProjPos : TEXCOORD0)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    #if SHADOW_DEPTH_PROJSPACE
        oProjPos = oPos;
    #else 
        oProjPos = mul(skinnedPos, mv);
    #endif
}

//--------------------------------------------------------------------------------------------------------------
/**
    Static Vertex Shader with uv coordin ates for alpha testing in pixelshader   
*/
void 
SkinnedShadowMappingATestVertexShader(float4 position     : POSITION,
                                      float2 uv0          : TEXCOORD0,
                                      float4 weights      : BLENDWEIGHT,
                                      float4 indices      : BLENDINDICES,
                                      out float4 oPos     : POSITION,
                                      out float2 oUv0     : TEXCOORD0,
                                      out float4 oProjPos : TEXCOORD1)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    oUv0 = uv0;
    #if SHADOW_DEPTH_PROJSPACE
        oProjPos = oPos;
    #else 
        oProjPos = mul(skinnedPos, mv);
    #endif
}

//--------------------------------------------------------------------------------------------------------------
/**
    Pixel Shader which writes depth in Projection Space
*/
void 
ShadowMappingPixelShader(float4 projPos      : TEXCOORD0,                         
                         out float4 oColor    : COLOR)
{
    #if SHADOW_DEPTH_PROJSPACE
        oColor = (projPos.z/ projPos.w) * shadowConstants.x;
    #else
        oColor = length(projPos.xyz) * shadowConstants.x;
    #endif
}

//--------------------------------------------------------------------------------------------------------------
/**
    Pixel Shader which writes depth in Projection Space
*/
void 
ShadowMappingATestPixelShader(float2 uv0        : TEXCOORD0,
                              float4 projPos    : TEXCOORD1,                         
                              out float4 oColor : COLOR)
{
    // clip alpha texels
    float alpha = tex2D(diffMapSampler, uv0).a;
    clip(alpha - 1/255);
    #if SHADOW_DEPTH_PROJSPACE
        oColor = (projPos.z/ projPos.w) * shadowConstants.x;
    #else
        oColor = length(projPos.xyz) * shadowConstants.x;
    #endif
}
//------------------------------------------------------------------------------
#endif

