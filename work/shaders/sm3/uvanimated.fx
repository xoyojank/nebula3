//------------------------------------------------------------------------------
//  uvanimated.fx
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "util.fxh"
#include "common.fxh"
#include "skinning.fxh"
#include "deferred_shared.fxh"

// alpha blend factor
float   alphaBlendFactor : Intensity0 = 1.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------NORMAL DEPTH----------------------------------------------------------
/**
    The NormalDepth pass, this fills the color buffer in xy with
    2 surface normal components, and zw with the depth value from 0 to 1 (0 is
    near plane, 1 is far plane) encoded in 16 bit.
*/
void
NormalDepthUVTransformedVertexShader(float4 position        : POSITION,
                                    float4 normal           : NORMAL,
                                    float2 uv0              : TEXCOORD0,
                                    float4 tangent          : TANGENT,
                                    float4 binormal         : BINORMAL,
                                    out float4 oPos         : POSITION,
                                    out float2 oUv0         : TEXCOORD0,
                                    out float3 oTangent     : TEXCOORD1,
                                    out float3 oNormal      : TEXCOORD2,
                                    out float3 oBinormal    : TEXCOORD3,
                                    out float3 viewSpacePos : TEXCOORD4)
{
    oPos = mul(position, mvp);    
    oUv0 = mul(float4(UnpackUv(uv0).xy, 0, 1), textureTransform0);
    viewSpacePos = mul(position, modelView).xyz;
    
    // setup matrix components to transform tangent space normals 
    // into view space
    oTangent  = mul(imv, UnpackNormal4(tangent)).xyz;
    oNormal   = mul(imv, UnpackNormal4(normal)).xyz;
    oBinormal = mul(imv, UnpackNormal4(binormal)).xyz;
} 

//---------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------SKINNED NORMAL DEPTH--------------------------------------------------
/**
    The NormalDepth pass, this fills the color buffer in xy with
    2 surface normal components, and zw with the depth value from 0 to 1 (0 is
    near plane, 1 is far plane) encoded in 16 bit.
*/
void
SkinnedNormalDepthUVTransformedVertexShader(float4 position         : POSITION,
                                            float4 normal           : NORMAL,
                                            float2 uv0              : TEXCOORD0,
                                            float4 tangent          : TANGENT,
                                            float4 binormal         : BINORMAL,
                                            float4 weights          : BLENDWEIGHT,
                                            float4 indices          : BLENDINDICES,
                                            out float4 oPos         : POSITION,
                                            out float2 oUv0         : TEXCOORD0,
                                            out float3 oTangent     : TEXCOORD1,
                                            out float3 oNormal      : TEXCOORD2,
                                            out float3 oBinormal    : TEXCOORD3,
                                            out float3 viewSpacePos : TEXCOORD4)
{
    float4 skinnedPos      = SkinnedPosition(position, weights, indices);
    float3 skinnedNormal   = SkinnedNormal(normalize(UnpackNormal(normal)), weights, indices);
    float3 skinnedTangent  = SkinnedNormal(normalize(UnpackNormal(tangent)), weights, indices);
    float3 skinnedBinormal = SkinnedNormal(normalize(UnpackNormal(binormal)), weights, indices);
    
    oPos = mul(skinnedPos, mvp);
    oUv0 = mul(float4(UnpackUv(uv0).xy, 0, 1), textureTransform0);
    viewSpacePos = mul(skinnedPos, modelView).xyz;
    
    // setup matrix components to transform tangent space normals 
    // into view space
    oTangent  = mul(imv, UnpackNormal4(tangent)).xyz;
    oNormal   = mul(imv, UnpackNormal4(normal)).xyz;
    oBinormal = mul(imv, UnpackNormal4(binormal)).xyz;
} 

//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorUVTransformedVertexShader(float4 position            : POSITION,
                    float2 uv0                 : TEXCOORD0,
                    out float4 oPos            : POSITION,
                    out float4 oProjPos        : TEXCOORD0,
                    out float2 oUv0            : TEXCOORD1)
{
    oPos = mul(position, mvp);
    oProjPos = oPos;    
    oUv0 = mul(float4(UnpackUv(uv0).xy, 0, 1), textureTransform0);  
}     

//----------------------------------------------- SKINNED --------------------------------------------------------------
void
SkinnedColorUVTransformedVertexShader(float4 position            : POSITION,
                           float2 uv0               : TEXCOORD0,
                           float4 weights           : BLENDWEIGHT,
                           float4 indices           : BLENDINDICES,
                           out float4 oPos          : POSITION,
                           out float4 oProjPos      : TEXCOORD0,
                           out float2 oUv0          : TEXCOORD1)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    oProjPos = oPos;    
    oUv0 = mul(float4(UnpackUv(uv0).xy, 0, 1), textureTransform0);  
}    

//---------------------------------------------------------------------------------------------------------------------
void
ColorPixelShader(float4 projPos           : TEXCOORD0,
                 float2 uv0               : TEXCOORD1,
                 float2 screenPos         : VPOS,  
                 out float4 oColor        : COLOR)
{     
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
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
ColorAlphaPixelShader(float4 projPos             : TEXCOORD0,
                         float3 uv0                 : TEXCOORD1,                         
                         float2 screenPos         : VPOS,  
                         out float4 oColor          : COLOR)
{    
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    float3 specColor = tex2D(specMapSampler, uv0.xy);

    // modulate diffuse texture with lightvalues from lightSampler and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity,        // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity);        // objects specular intensity   
    
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // modulate alpha by alphaBlendFactor
    oColor.a *= alphaBlendFactor;
    // hrd stuff
    oColor = EncodeHDR(oColor);
}    

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------- VARIATIONS ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// FIXME: move cullmode and alpharef to better position if possible!
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 120;

//---------------------------- Deferred Lighting -----------------------------------------------------------------------     
//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/ 
SimpleTechnique(NormalDepth, "NormalDepth", NormalDepthUVTransformedVertexShader, NormalDepthPassPixelShader);
//SimpleTechnique(NormalDepthATest, "NormalDepthATest", NormalDepthPassVertexShader, NormalDepthPassAlphaTestPixelShader);

SimpleTechnique(Solid0, "Solid", ColorUVTransformedVertexShader, ColorPixelShader);
SimpleTechnique(Alpha0, "Alpha", ColorUVTransformedVertexShader, ColorAlphaPixelShader);

//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for skinned geometry.
*/  
SimpleTechnique(SkinnedNormalDepth, "NormalDepth|Skinned", SkinnedNormalDepthUVTransformedVertexShader, RimNormalDepthPassPixelShader);
//SimpleTechnique(NormalDepthATest, "NormalDepthATest|Skinned", SkinnedNormalDepthPassVertexShader, NormalDepthPassAlphaTestPixelShader);

SimpleTechnique(SkinnedSolid0, "Solid|Skinned", SkinnedColorUVTransformedVertexShader, ColorPixelShader);
SimpleTechnique(SkinnedAlpha0, "Alpha|Skinned", SkinnedColorUVTransformedVertexShader, ColorAlphaPixelShader);