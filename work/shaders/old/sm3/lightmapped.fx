//------------------------------------------------------------------------------
//  lightmapped.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"
#include "depthfuncs_atest.fxh"
#include "util.fxh"

float brightness : Intensity0;

struct vsVSMPassOutput
{
    float4 position : POSITION;
    float depth : TEXCOORD0;
};

struct vsColorPassInput
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 uv0      : TEXCOORD0;
    float2 uv1      : TEXCOORD1;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;
    float2 uv1         : TEXCOORD1;
    float4 worldPos    : TEXCOORD2;     // position in world space
    float4 projPos     : TEXCOORD3;     // position in projection space
    float3 normal      : TEXCOORD4;     // vertex normal in world space
    float3 tangent     : TEXCOORD5;     // vertex tangent in world space
    float3 binormal    : TEXCOORD6;     // vertex binormal in world space
    float3 worldEyeVec : COLOR0;        // normalized world space eye vector
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for rendering variance shadow map.
*/
vsVSMPassOutput
VSMPassVertexShader(float4 pos : POSITION)
{
    vsVSMPassOutput vsOut;
    vsOut.position = mul(pos, mvp);
    vsOut.depth = vsOut.position.z;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader function for rendering variance shadow map. Contains
    the light space depth in R, and squared depth in G.
*/
float4
VSMPassPixelShader(vsVSMPassOutput psIn) : COLOR
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
    vsOut.uv0 = UnpackUv(vsIn.uv0);
    vsOut.uv1 = UnpackUv(vsIn.uv1);
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
    float4 lightmapColor = tex2D(lightMapSampler, psIn.uv1);
    matDiffuse.rgb = matDiffuse.rgb * lightmapColor.rgb * brightness;
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);    
    color = psFog(psIn.projPos.z, color);
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
*/
float4 
AlphaColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float4 matEmissive = tex2D(emsvSampler, psIn.uv0);
    float4 lightmapColor = tex2D(lightMapSampler, psIn.uv1);
    matDiffuse.rgb = matDiffuse.rgb * lightmapColor.rgb * brightness;
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, MatSpecularPower, matEmissive);
    color = psFog(psIn.projPos.z, color);
    color.a = matDiffuse.a;
    // premultiplied alpha
    color.rgb *= color.a;
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;
//------------------------------------------------------------------------------
SimpleTechnique(Depth, "Depth", DepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(DepthAlphaTest, "DepthATest", DepthPassAlphaTestVertexShader, DepthPassAlphaTestPixelShader);
SimpleTechnique(VSMDepth, "VSMDepth", VSMPassVertexShader, VSMPassPixelShader);

LightTechnique(Solid0, "Solid|LocalLights0", ColorPassVertexShader, ColorPassPixelShader, 0);
LightTechnique(Solid1, "Solid|LocalLights1", ColorPassVertexShader, ColorPassPixelShader, 1);
LightTechnique(Solid2, "Solid|LocalLights2", ColorPassVertexShader, ColorPassPixelShader, 2);
LightTechnique(Solid3, "Solid|LocalLights3", ColorPassVertexShader, ColorPassPixelShader, 3);
LightTechnique(Solid4, "Solid|LocalLights4", ColorPassVertexShader, ColorPassPixelShader, 4);

LightTechnique(Alpha0, "Alpha|LocalLights0", ColorPassVertexShader, AlphaColorPassPixelShader, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", ColorPassVertexShader, AlphaColorPassPixelShader, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", ColorPassVertexShader, AlphaColorPassPixelShader, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", ColorPassVertexShader, AlphaColorPassPixelShader, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", ColorPassVertexShader, AlphaColorPassPixelShader, 4);
