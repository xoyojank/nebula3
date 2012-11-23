//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "util.fxh"
#include "common.fxh"
#include "skinning.fxh"
#include "deferred_shared.fxh"
#include "shadowmapping.fxh"

// alpha blend factor
float alphaBlendFactor : Intensity0 = 1.0f;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorVertexShader(float4 position          : POSITION,
                  float4 normal            : NORMAL,
                  float2 uv0               : TEXCOORD0,
                  out float4 oPos          : POSITION,
                  out float4 oProjPos      : TEXCOORD0,
                  out float4 oViewSpacePos : TEXCOORD1,
                  out float2 oUv0          : TEXCOORD2)
{
    oPos = mul(position, mvp);
    oViewSpacePos.xyz = mul(position, modelView).xyz;
    oViewSpacePos.w = normal.w;
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy; 
}     

//---------------------------------------------------------------------------------------------------------------------
void
ColorPixelShader(float4 projPos      : TEXCOORD0,
                 float4 viewSpacePos : TEXCOORD1,
                 float2 uv0          : TEXCOORD2,
                 float2 screenPos    : VPOS,  
                 out float4 oColor   : COLOR)
{     
    float4 lightValues = SampleLightBuffer(screenPos.xy, float2(ObjectId, viewSpacePos.w), dsfObjectDepthSampler, lightSampler);
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    float3 specColor = tex2D(specMapSampler, uv0.xy);

    // modulate diffuse texture with lightvalues from lightbuffer and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity,        // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity);        // objects specular intensity   
    
    oColor.a = diffColor.a;    
    // modulate alpha by alphaBlendFactor
    oColor.a *= alphaBlendFactor;
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
AlphaColorPixelShader(float4 projPos      : TEXCOORD0,
                             float4 viewSpacePos : TEXCOORD1,
                             float2 uv0          : TEXCOORD2,
                             float2 screenPos    : VPOS,  
                             out float4 oColor   : COLOR,
                             uniform bool useLight)
{   
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);    
    if (useLight)
    {
        float4 lightValues = SampleLightBuffer(screenPos.xy, float2(ObjectId, viewSpacePos.w), alphaDsfObjectIdSampler, alphaLightSampler);    
        float3 emsvColor = tex2D(emsvSampler, uv0.xy);
        float3 specColor = tex2D(specMapSampler, uv0.xy);

        // modulate diffuse texture with lightvalues from lightSampler and objects' emissive and specular maps    
        oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity,        // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity);        // objects specular intensity   
    }
    else
    {
        oColor = diffColor;
    }
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    oColor.a = diffColor.a;
    // modulate alpha by alphaBlendFactor
    oColor.a *= alphaBlendFactor;
    // hrd stuff
    oColor = EncodeHDR(oColor);
} 
    
//----------------------------------------------- ADDITIVE ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
AdditiveVertexShader(float4 position          : POSITION,
                      float4 normal            : NORMAL,
                      float2 uv0               : TEXCOORD0,
                      out float4 oPos          : POSITION,
                      out float4 oProjPos      : TEXCOORD0,
                      out float2 oUv0          : TEXCOORD1)
{
    oPos = mul(position, mvp);
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy; 
} 
//---------------------------------------------------------------------------------------------------------------------
/**
*/ 
void
SkinnedAdditiveVertexShader(float4 position           : POSITION,
                             float4 normal             : NORMAL,
                             float2 uv0                : TEXCOORD0,
                             float4 tangent            : TANGENT,
                             float4 binormal           : BINORMAL,
                             float4 weights            : BLENDWEIGHT,
                             float4 indices            : BLENDINDICES,
                             out float4 oPos           : POSITION,
                             out float4 oProjPos       : TEXCOORD0,
                             out float2 oUv0           : TEXCOORD1)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy;   
} 
//---------------------------------------------------------------------------------------------------------------------
/**
*/
void 
AdditivePassPixelShader(float4 projPos      : TEXCOORD0,
                        float2 uv0          : TEXCOORD1,
                        out float4 oColor : COLOR)
{   
    oColor = tex2D(diffMapSampler, uv0);
    // addtive particles will be just fade out into fog, doesn't lerp to fogColor
    float fogIntensity = clamp((fogDistances.y - projPos.z) / (fogDistances.y - fogDistances.x), fogColor.a, 1.0);
    oColor *= fogIntensity;
    // for blending out
    oColor *= alphaBlendFactor;
    oColor = EncodeHDR(oColor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------- VARIATIONS ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// FIXME: move cullmode and alpharef to better position if possible!
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 0;

//---------------------------- Deferred Lighting -----------------------------------------------------------------------     
//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/
SimpleTechnique(ESMDepth, "ESMDepth", ShadowMappingVertexShader, ShadowMappingPixelShader);
SimpleTechnique(ESMDepthATest, "ESMDepthATest", ShadowMappingATestVertexShader, ShadowMappingATestPixelShader);
SimpleTechnique(NormalDepth, "NormalDepth", NormalDepthPassVertexShader, NormalDepthPassPixelShader);

SimpleTechnique(Solid, "Solid", ColorVertexShader, ColorPixelShader);
SimpleTechnique(Additive, "Additive", AdditiveVertexShader, AdditivePassPixelShader);
AlphaTechnique(AlphaLit, "AlphaLit", ColorVertexShader, AlphaColorPixelShader, true);
AlphaTechnique(AlphaUnlit, "AlphaUnlit", ColorVertexShader, AlphaColorPixelShader, false);

//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for skinned geometry.
*/
SimpleTechnique(SkinnedESMDepth, "ESMDepth|Skinned", SkinnedShadowMappingVertexShader, ShadowMappingPixelShader);  
SimpleTechnique(SkinnedESMDepthATest, "ESMDepthATest|Skinned", SkinnedShadowMappingATestVertexShader, ShadowMappingATestPixelShader);
SimpleTechnique(SkinnedNormalDepth, "NormalDepth|Skinned", SkinnedNormalDepthPassVertexShader, RimNormalDepthPassPixelShader);

SimpleTechnique(SkinnedSolid, "Solid|Skinned", SkinnedColorVertexShader, ColorPixelShader);
SimpleTechnique(SkinnedAdditive, "Additive|Skinned", SkinnedAdditiveVertexShader, AdditivePassPixelShader);
AlphaTechnique(SkinnedAlphaLit, "AlphaLit|Skinned", SkinnedColorVertexShader, AlphaColorPixelShader, true);
AlphaTechnique(SkinnedAlphaUnlit, "AlphaLit|Skinned", SkinnedColorVertexShader, AlphaColorPixelShader, false);