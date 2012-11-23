#ifndef DEPTHFUNCS_ATEST_H
#define DEPTHFUNCS_ATEST_H
//------------------------------------------------------------------------------
//  depthfuncs_atest.fx
//
//  shader programms used for depth pass with alpha testing.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

struct vsDepthPassAlphaTestInput
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

struct vsDepthPassAlphaTestOutput
{
    float4 position    : POSITION;
    float3 uv0depth    : TEXCOORD0;    
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for the depth pass with alpha test.
*/
vsDepthPassAlphaTestOutput
DepthPassAlphaTestVertexShader(vsDepthPassAlphaTestInput vsIn)
{
    vsDepthPassAlphaTestOutput vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    vsOut.uv0depth.xy = UnpackUv(vsIn.uv0);
    vsOut.uv0depth.z  = vsOut.position.z / vsOut.position.w;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 DepthPassAlphaTestPixelShader(const vsDepthPassAlphaTestOutput psIn) : COLOR
{
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0depth.xy);
    return float4(psIn.uv0depth.z, 0.0f, 0.0f, matDiffuse.a);
}
#endif