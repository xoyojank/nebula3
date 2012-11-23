//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"
#include "depthfuncs_atest.fxh"
#include "skinning.fxh"
#include "util.fxh"

// alpha blend factor
float Reflectivity : Intensity0 = 0.5f;

struct vsColorPassInput
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 uv0      : TEXCOORD0;
};

struct vsSkinnedColorPassInput
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 uv0      : TEXCOORD0;
    float4 weights  : BLENDWEIGHT;
    float4 indices  : BLENDINDICES;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;
    float4 worldPos    : TEXCOORD1;     // position in world space
    float4 projPos     : TEXCOORD2;     // position in projection space
    float3 normal      : TEXCOORD3;     // vertex normal in world space
    float3 tangent     : TEXCOORD4;     // vertex tangent in world space
    float3 binormal    : TEXCOORD5;     // vertex binormal in world space
    float3 worldEyeVec : COLOR0;        // normalized world space eye vector
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
    vsOut.projPos     = vsOut.position;
    vsOut.worldPos    = mul(vsIn.position, model);
    vsOut.worldEyeVec = normalize(eyePos - vsOut.worldPos);
    vsOut.normal      = normalize(mul(UnpackNormal(vsIn.normal), model));
    vsOut.tangent     = normalize(mul(UnpackNormal(vsIn.tangent), model));
    vsOut.binormal    = normalize(mul(UnpackNormal(vsIn.binormal), model));
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
    float3 skinnedNormal   = SkinnedNormal(normalize(UnpackNormal(vsIn.normal)), vsIn.weights, vsIn.indices);
    float3 skinnedTangent  = SkinnedNormal(normalize(UnpackNormal(vsIn.tangent)), vsIn.weights, vsIn.indices);
    float3 skinnedBinormal = SkinnedNormal(normalize(UnpackNormal(vsIn.binormal)), vsIn.weights, vsIn.indices);
    
    vsOut.position    = mul(skinnedPos, mvp);
    vsOut.projPos     = vsOut.position;
    vsOut.worldPos    = mul(skinnedPos, model);
    vsOut.worldEyeVec = normalize(eyePos - vsOut.worldPos);
    vsOut.normal      = mul(skinnedNormal, model);
    vsOut.tangent     = mul(skinnedTangent, model);
    vsOut.binormal    = mul(skinnedBinormal, model);
    vsOut.uv0         = UnpackUv(vsIn.uv0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float4 matEmissive = tex2D(emsvSampler, psIn.uv0);
    
    float3 reflectVec = reflect(psIn.worldEyeVec, worldNormal);
    float4 environmentColor = texCUBE(environmentSampler, reflectVec * matSpecular.a);
    matDiffuse = lerp(matDiffuse, environmentColor, Reflectivity);
    
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);
    color = psFog(psIn.projPos.z, color);    
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
*/
float4 
AlphaBlendColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float4 matEmissive = tex2D(emsvSampler, psIn.uv0);
    
    float3 reflectVec = reflect(psIn.worldEyeVec, worldNormal);
    float4 environmentColor = texCUBE(environmentSampler, reflectVec * matSpecular.a);
    matDiffuse = lerp(matDiffuse, environmentColor, Reflectivity);
    
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);
    color = psFog(psIn.projPos.z, color);
    color.a = matDiffuse.a;
    // premultiplied alpha
    color.rgb *= color.a;    
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
*/
float4 
AdditiveColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float4 color = tex2D(emsvSampler, psIn.uv0);
    color *= MatEmissiveIntensity;
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
    The pixel shader for skinning uses a RimLight effect.
    FIXME: maybe this should better go into a "character", or "skin" shader?
*/
float4 
SkinnedColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float4 matEmissive = tex2D(emsvSampler, psIn.uv0);
    
    float3 reflectVec = reflect(psIn.worldEyeVec, worldNormal);
    float4 environmentColor = texCUBE(environmentSampler, reflectVec);
    matDiffuse = lerp(matDiffuse, environmentColor, Reflectivity * matSpecular.a);
    float4 color = psLightRim(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);
    color = psFog(psIn.projPos.z, color);   
    color = EncodeHDR(color);  
    return color;
}

//------------------------------------------------------------------------------
/**
    The pixel shader for skinning uses a RimLight effect.
    FIXME: maybe this should better go into a "character", or "skin" shader?
*/
float4 
AlphaBlendSkinnedColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float4 matEmissive = tex2D(emsvSampler, psIn.uv0);
    
    float3 reflectVec = reflect(psIn.worldEyeVec, worldNormal);
    float4 environmentColor = texCUBE(environmentSampler, reflectVec);
    matDiffuse = lerp(matDiffuse, environmentColor, Reflectivity * matSpecular.a);
    float4 color = psLightRim(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);
    color = psFog(psIn.projPos.z, color); 
    color.a = matDiffuse.a;
    // premultiplied alpha
    color.rgb *= color.a;   
    color = EncodeHDR(color); 
    return color;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 120;

//------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/        
SimpleTechnique(Depth, "Depth", DepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(DepthAlphaTest, "DepthATest", DepthPassAlphaTestVertexShader, DepthPassAlphaTestPixelShader);
SimpleTechnique(VSMDepth, "VSMDepth", DepthPassVertexShader, VSMPassPixelShader);

LightTechnique(Solid0, "Solid|LocalLights0", ColorPassVertexShader, ColorPassPixelShader, 0);
LightTechnique(Solid1, "Solid|LocalLights1", ColorPassVertexShader, ColorPassPixelShader, 1);
LightTechnique(Solid2, "Solid|LocalLights2", ColorPassVertexShader, ColorPassPixelShader, 2);
LightTechnique(Solid3, "Solid|LocalLights3", ColorPassVertexShader, ColorPassPixelShader, 3);
LightTechnique(Solid4, "Solid|LocalLights4", ColorPassVertexShader, ColorPassPixelShader, 4);

LightTechnique(Alpha0, "Alpha|LocalLights0", ColorPassVertexShader, AlphaBlendColorPassPixelShader, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", ColorPassVertexShader, AlphaBlendColorPassPixelShader, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", ColorPassVertexShader, AlphaBlendColorPassPixelShader, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", ColorPassVertexShader, AlphaBlendColorPassPixelShader, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", ColorPassVertexShader, AlphaBlendColorPassPixelShader, 4);

LightTechnique(Additive0, "Additive|LocalLights0", ColorPassVertexShader, AdditiveColorPassPixelShader, 0);
LightTechnique(Additive1, "Additive|LocalLights1", ColorPassVertexShader, AdditiveColorPassPixelShader, 1);
LightTechnique(Additive2, "Additive|LocalLights2", ColorPassVertexShader, AdditiveColorPassPixelShader, 2);
LightTechnique(Additive3, "Additive|LocalLights3", ColorPassVertexShader,  AdditiveColorPassPixelShader, 3);
LightTechnique(Additive4, "Additive|LocalLights4", ColorPassVertexShader, AdditiveColorPassPixelShader, 4);

//------------------------------------------------------------------------------
/**
    Techniques for skinned geometry.
*/        
SimpleTechnique(SkinnedDepth, "Depth|Skinned", SkinnedDepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(SkinnedDepthAlphaTest, "DepthATest|Skinned", SkinnedDepthPassAlphaTestVertexShader, DepthPassAlphaTestPixelShader);
SimpleTechnique(SkinnedVSMDepth, "VSMDepth|Skinned", SkinnedDepthPassVertexShader, VSMPassPixelShader);

LightTechnique(SkinnedSolid0, "Solid|LocalLights0|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader, 0);
LightTechnique(SkinnedSolid1, "Solid|LocalLights1|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader, 1);
LightTechnique(SkinnedSolid2, "Solid|LocalLights2|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader, 2);
LightTechnique(SkinnedSolid3, "Solid|LocalLights3|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader, 3);
LightTechnique(SkinnedSolid4, "Solid|LocalLights4|Skinned", SkinnedColorPassVertexShader, SkinnedColorPassPixelShader, 4);

LightTechnique(SkinnedAlpha0, "Alpha|LocalLights0|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader, 0);
LightTechnique(SkinnedAlpha1, "Alpha|LocalLights1|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader, 1);
LightTechnique(SkinnedAlpha2, "Alpha|LocalLights2|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader, 2);
LightTechnique(SkinnedAlpha3, "Alpha|LocalLights3|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader, 3);
LightTechnique(SkinnedAlpha4, "Alpha|LocalLights4|Skinned", SkinnedColorPassVertexShader, AlphaBlendSkinnedColorPassPixelShader, 4);

LightTechnique(SkinnedAdditive0, "Additive|LocalLights0|Skinned", SkinnedColorPassVertexShader, AdditiveColorPassPixelShader, 0);
LightTechnique(SkinnedAdditive1, "Additive|LocalLights1|Skinned", SkinnedColorPassVertexShader, AdditiveColorPassPixelShader, 1);
LightTechnique(SkinnedAdditive2, "Additive|LocalLights2|Skinned", SkinnedColorPassVertexShader, AdditiveColorPassPixelShader, 2);
LightTechnique(SkinnedAdditive3, "Additive|LocalLights3|Skinned", SkinnedColorPassVertexShader, AdditiveColorPassPixelShader, 3);
LightTechnique(SkinnedAdditive4, "Additive|LocalLights4|Skinned", SkinnedColorPassVertexShader, AdditiveColorPassPixelShader, 4);
