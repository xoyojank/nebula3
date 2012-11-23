#ifndef SKINNING_H
#define SKINNING_H
//------------------------------------------------------------------------------
//  skinning.fxh
//
//  Shader functions for vertex skinning.
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "config.fxh"
#include "common.fxh"
#include "depthfuncs_atest.fxh"

#define USE_JOINT_TEXTURE 0 // not supported on some DX9 cards (e.g. ATI X1xxx), use only on DX10 hardware

#if USE_JOINT_TEXTURE
#define MAX_BONE_COUNT 256.0f
#define MAX_CHARACTER_COUNT 256.0f
// Vertex shader texture sampler for the bone matrix palette.
shared texture jointMap : JointTexture;
sampler jointMapSampler = sampler_state 
{ 
    Texture   = <jointMap>;
    MipFilter = NONE; 
    MinFilter = POINT; 
    MagFilter = POINT; 
    AddressU  = CLAMP; 
    AddressV  = CLAMP; 
};
float characterIndex : CharacterIndex = 0;
#else
// vertex shader constant skinning, leads to constant waterfalling!
matrix<float,4,3> JointPalette[72] : JointPalette;
#endif

#if USE_JOINT_TEXTURE
//------------------------------------------------------------------------------
/**
    Fetch skinning matrix from texture.
*/
float4x3 
TFetchBoneMatrix(float BoneIndex)
{    
    const int mipmaplevel = 0;
    float4 texCoord = float4(BoneIndex / MAX_BONE_COUNT, characterIndex / MAX_CHARACTER_COUNT, mipmaplevel, mipmaplevel);  
    const float matrixSize = 4.0f;                
    float offset = 1.0f / (MAX_BONE_COUNT * matrixSize);
    
    float4 row1 = tex2Dlod(jointMapSampler, texCoord);
    texCoord.x += offset;
    float4 row2 = tex2Dlod(jointMapSampler, texCoord);
    texCoord.x += offset;
    float4 row3 = tex2Dlod(jointMapSampler, texCoord);
    
    float4x3 result;
    result._11_21_31_41 = row1.xyzw;    
    result._12_22_32_42 = row2.xyzw;
    result._13_23_33_43 = row3.xyzw;
                
    return result;
}
#endif

//------------------------------------------------------------------------------
/**
    Compute a skinned vertex position.
*/
float4
SkinnedPosition(const float4 inPos, const float4 weights, const float4 indices)
{
    // need to re-normalize weights because of compression
    float3 pos[4];
    float4 normWeights = weights / dot(weights, float4(1.0, 1.0, 1.0, 1.0));
#if USE_JOINT_TEXTURE
    pos[0] = (mul(inPos, TFetchBoneMatrix(indices[0]))) * normWeights[0];
    pos[1] = (mul(inPos, TFetchBoneMatrix(indices[1]))) * normWeights[1];
    pos[2] = (mul(inPos, TFetchBoneMatrix(indices[2]))) * normWeights[2];
    pos[3] = (mul(inPos, TFetchBoneMatrix(indices[3]))) * normWeights[3];
#else
    pos[0] = (mul(inPos, JointPalette[indices[0]])) * normWeights[0];
    pos[1] = (mul(inPos, JointPalette[indices[1]])) * normWeights[1];
    pos[2] = (mul(inPos, JointPalette[indices[2]])) * normWeights[2];
    pos[3] = (mul(inPos, JointPalette[indices[3]])) * normWeights[3];
#endif
    return float4(pos[0] + pos[1] + pos[2] + pos[3], 1.0f);
}

//------------------------------------------------------------------------------
/**
    Compute a skinned vertex normal.
*/
float3
SkinnedNormal(const float3 inNormal, const float4 weights, const float4 indices)
{
    // normals don't need to be 100% perfect, so don't normalize weights
    float3 normal[4];
#if USE_JOINT_TEXTURE
    normal[0] = (mul(inNormal, TFetchBoneMatrix(indices[0]))) * weights[0];
    normal[1] = (mul(inNormal, TFetchBoneMatrix(indices[1]))) * weights[1];
    normal[2] = (mul(inNormal, TFetchBoneMatrix(indices[2]))) * weights[2];
    normal[3] = (mul(inNormal, TFetchBoneMatrix(indices[3]))) * weights[3];
#else
    normal[0] = mul(inNormal, (matrix<float,3,3>)JointPalette[indices[0]]) * weights[0];
    normal[1] = mul(inNormal, (matrix<float,3,3>)JointPalette[indices[1]]) * weights[1];
    normal[2] = mul(inNormal, (matrix<float,3,3>)JointPalette[indices[2]]) * weights[2];
    normal[3] = mul(inNormal, (matrix<float,3,3>)JointPalette[indices[3]]) * weights[3];
#endif
    
    return float3(normal[0] + normal[1] + normal[2] + normal[3]);
}

//------------------------------------------------------------------------------
/**
    Depth pass vertex shader for skinned geometry.
*/
vsDepthPassOutput
SkinnedDepthPassVertexShader(float4 pos : POSITION, float4 weights : BLENDWEIGHT, float4 indices : BLENDINDICES)
{
    vsDepthPassOutput vsOut;
    float4 skinnedPos = SkinnedPosition(pos, weights, indices);
    vsOut.position = mul(skinnedPos, mvp);
    vsOut.depth  = vsOut.position.z / vsOut.position.w;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Depth pass vertex shader for skinned geometry with y-height save.
*/
vsVSMDepthPassOutput
SkinnedVSMDepthPassVertexShader(float4 pos : POSITION, float4 weights : BLENDWEIGHT, float4 indices : BLENDINDICES)
{
    vsVSMDepthPassOutput vsOut;
    float4 skinnedPos = SkinnedPosition(pos, weights, indices);
    vsOut.position = mul(skinnedPos, mvp);
    vsOut.depth.x = vsOut.position.z;
    vsOut.depth.y = skinnedPos.y; // HOTFIX cut in modelspace for apperaing animation
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Depth pass vertex shader for alpha test with skinning.
*/
vsDepthPassAlphaTestOutput
SkinnedDepthPassAlphaTestVertexShader(float4 pos : POSITION,
                                      float2 uv0 : TEXCOORD0,
                                      float4 weights : BLENDWEIGHT,
                                      float4 indices : BLENDINDICES)
{
    vsDepthPassAlphaTestOutput vsOut;
    float4 skinnedPos = SkinnedPosition(pos, weights, indices);
    vsOut.position = mul(skinnedPos, mvp);
    vsOut.uv0depth.xy = UnpackUv(uv0);
    vsOut.uv0depth.z  = vsOut.position.z / vsOut.position.w;
    return vsOut;
}    

#endif
