//---------------------------------------------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//---------------------------------------------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "common.fxh"
#include "util.fxh"
#include "deferred_shared.fxh"
#include "shadowmapping.fxh"
#include "shared.fxh"

// alpha blend factor
float Reflectivity : Intensity0 = 0.5f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorEnvVertexShader(float4 position           : POSITION,
                     float4 normal             : NORMAL,
                     float2 uv0                : TEXCOORD0,
                     float4 tangent            : TANGENT,
                     float4 binormal           : BINORMAL,
                     out float4 oPos           : POSITION,
                     out float4 oProjPos       : TEXCOORD0,
                     out float4 oViewSpacePos  : TEXCOORD1,
                     out float2 oUv0           : TEXCOORD2,
                     out float3 oTangent       : TEXCOORD3,
                     out float3 oNormal        : TEXCOORD4,
                     out float3 oBinormal      : TEXCOORD5,
                     out float3 oWorldSpacePos : TEXCOORD6)
{
    oPos              = mul(position, mvp);
    oProjPos          = oPos;
    oUv0              = UnpackUv(uv0).xy;
    oWorldSpacePos    = mul(position, model);  
    oViewSpacePos.xyz = mul(position, modelView); 
    oViewSpacePos.w   = normal.w; 
                
    // setup matrix components to transform tangent space normals into world space
    oTangent  = normalize(mul(UnpackNormal4(tangent), model).xyz);
    oNormal   = normalize(mul(UnpackNormal4(normal), model).xyz);
    oBinormal = normalize(mul(UnpackNormal4(binormal), model).xyz);   
}     

//----------------------------------------------- SKINNED --------------------------------------------------------------
void
SkinnedEnvColorVertexShader(float4 position           : POSITION,
                         float4 normal             : NORMAL,
                         float2 uv0                : TEXCOORD0,
                         float4 tangent            : TANGENT,
                         float4 binormal           : BINORMAL,
                         float4 weights            : BLENDWEIGHT,
                         float4 indices            : BLENDINDICES,
                         out float4 oPos           : POSITION,
                         out float4 oProjPos       : TEXCOORD0,
                         out float4 oViewSpacePos  : TEXCOORD1,
                         out float2 oUv0           : TEXCOORD2,
                         out float3 oTangent       : TEXCOORD3,
                         out float3 oNormal        : TEXCOORD4,
                         out float3 oBinormal      : TEXCOORD5,
                         out float3 oWorldSpacePos : TEXCOORD6)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    oViewSpacePos.xyz = mul(skinnedPos, modelView).xyz;
    oViewSpacePos.w   = normal.w; 
    oWorldSpacePos = mul(skinnedPos, model).xyz;
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy;  
    
    // setup matrix components to transform tangent space normals into world space
    oTangent  = normalize(mul(UnpackNormal4(tangent), model).xyz);
    oNormal   = normalize(mul(UnpackNormal4(normal), model).xyz);
    oBinormal = normalize(mul(UnpackNormal4(binormal), model).xyz);    
} 
 
//---------------------------------------------------------------------------------------------------------------------
void
ColorEnvPixelShader(float4 projPos       : TEXCOORD0,
                    float4 viewSpacePos  : TEXCOORD1,
                    float2 uv0           : TEXCOORD2,                    
                    float3 tangent       : TEXCOORD3,
                    float3 normal        : TEXCOORD4,
                    float3 binormal      : TEXCOORD5,
                    float3 worldSpacePos : TEXCOORD6,
                    float2 screenPos     : VPOS,    
                    out float4 oColor    : COLOR)
{         
    float4 lightValues = SampleLightBuffer(screenPos.xy, float2(ObjectId, viewSpacePos.w), dsfObjectDepthSampler, lightSampler);
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    float4 specColor = tex2D(specMapSampler, uv0.xy);
    
    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3x3 tangentWorldMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));
    float3 worldSpaceNormal = mul(tNormal, tangentWorldMatrix);
    float3 worldViewVec = normalize(worldSpacePos.xyz - eyePos.xyz);
    // compute reflected lookup vector
    float3 reflectVec = reflect(worldViewVec, worldSpaceNormal);
    // lookup reflective color
    float4 environmentColor = texCUBE(environmentSampler, reflectVec);
    float4 envDiffColor = float4(lerp(diffColor.rgb, environmentColor.rgb, Reflectivity * specColor.a), diffColor.a);
    
    // modulate diffuse texture with lightvalues from lightbuffer and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             envDiffColor,         // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity, // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity); // objects specular intensity 
    
    oColor.a = diffColor.a;
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // hrd stuff
    oColor = EncodeHDR(oColor);
}  

//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- COLOR ALPHA ---------------------------------------------------------
/**
    The color pass with alpha flag  
*/
void
ColorAlphaEnvPixelShader(float4 projPos       : TEXCOORD0,
                         float4 viewSpacePos  : TEXCOORD1,
                         float2 uv0           : TEXCOORD2,                    
                         float3 tangent       : TEXCOORD3,
                         float3 normal        : TEXCOORD4,
                         float3 binormal      : TEXCOORD5,
                         float3 worldSpacePos : TEXCOORD6,                                                    
                         float2 screenPos     : VPOS,  
                         out float4 oColor    : COLOR,
                         uniform bool useLight)
{   
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float4 specColor = tex2D(specMapSampler, uv0.xy); 
    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3x3 tangentWorldMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));
    float3 worldSpaceNormal = mul(tNormal, tangentWorldMatrix);
    float3 worldViewVec = normalize(worldSpacePos.xyz - eyePos.xyz);
    // compute reflected lookup vector
    float3 reflectVec = reflect(worldViewVec, worldSpaceNormal);
    // lookup reflective color
    float4 environmentColor = texCUBE(environmentSampler, reflectVec);
    float4 envDiffColor = float4(lerp(diffColor.rgb, environmentColor.rgb, Reflectivity * specColor.a), diffColor.a);

    if (useLight)
    {
        float4 lightValues = SampleLightBuffer(screenPos.xy, float2(ObjectId, viewSpacePos.w), alphaDsfObjectIdSampler, alphaLightSampler);    
        float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    
        // modulate diffuse texture with lightvalues from lightSampler and objects' emissive and specular maps    
        oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                                 envDiffColor,         // objects albedo diffuse texture color
                                 emsvColor,            // objects emissive texture color
                                 MatEmissiveIntensity, // objects emissive intensity
                                 specColor,            // objects specular texture color
                                 MatSpecularIntensity); // objects specular intensity   
    }
    else
    {
        oColor = envDiffColor;
    }
    
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // modulate alpha by alphaBlendFactor, 
    //FIXME: INTENSITY0 is used for reflectivity
    //oColor.a *= alphaBlendFactor;
    // hrd stuff
    oColor = EncodeHDR(oColor);
}    

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------- VARIATIONS ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 0;
//---------------------------- Deferred Lighting -----------------------------------------------------------------------     
//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/ 
SimpleTechnique(ESMDepth, "ESMDepth", ShadowMappingVertexShader, ShadowMappingPixelShader);
SimpleTechnique(ESMDepthATest, "ESMDepthATest", ShadowMappingATestVertexShader, ShadowMappingATestPixelShader);
SimpleTechnique(NormalDepthEnv, "NormalDepth", NormalDepthPassVertexShader, NormalDepthPassPixelShader);

SimpleTechnique(SolidEnv, "Solid", ColorEnvVertexShader, ColorEnvPixelShader);
AlphaTechnique(SolidEnvAlpha, "AlphaLit", ColorEnvVertexShader, ColorAlphaEnvPixelShader, true);
AlphaTechnique(SolidEnvAlphaUnlit, "AlphaUnlit", ColorEnvVertexShader, ColorAlphaEnvPixelShader, false);

//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for skinned geometry.
*/ 
SimpleTechnique(SkinnedESMDepth, "ESMDepth|Skinned", SkinnedShadowMappingVertexShader, ShadowMappingPixelShader);   
SimpleTechnique(SkinnedESMDepthATest, "ESMDepthATest|Skinned", SkinnedShadowMappingATestVertexShader, ShadowMappingATestPixelShader);
SimpleTechnique(SkinnedNormalDepthEnv, "NormalDepth|Skinned", SkinnedNormalDepthPassVertexShader, RimNormalDepthPassPixelShader);

SimpleTechnique(SkinnedSolidEnv, "Solid|Skinned", SkinnedEnvColorVertexShader, ColorEnvPixelShader);
AlphaTechnique(SkinnedEnvAlpha, "AlphaLit|Skinned", SkinnedEnvColorVertexShader, ColorAlphaEnvPixelShader, true);
AlphaTechnique(SkinnedEnvAlphaUnlit, "AlphaUnlit|Skinned", SkinnedEnvColorVertexShader, ColorAlphaEnvPixelShader, false);
