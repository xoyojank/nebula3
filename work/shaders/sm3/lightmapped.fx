//------------------------------------------------------------------------------
//  lightmapped.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "common.fxh"
#include "util.fxh"
#include "deferred_shared.fxh"
#include "shadowmapping.fxh"

float brightness : Intensity0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorVertexShader(float4 position      : POSITION,
                  float2 uv0           : TEXCOORD0,                    
                  float2 uv1           : TEXCOORD1,
                  out float4 oPos      : POSITION,
                  out float4 oProjPos  : TEXCOORD0,
                  out float2 oUv0      : TEXCOORD1,
                  out float2 oUv1      : TEXCOORD2)
{
    oPos = mul(position, mvp);
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy;
    oUv1.xy = UnpackUv(uv1).xy;
}     

//---------------------------------------------------------------------------------------------------------------------
void
ColorPixelShader(float4 projPos           : TEXCOORD0,
                 float2 uv0               : TEXCOORD1,
                 float2 uv1               : TEXCOORD2,
                 float2 screenPos         : VPOS,  
                 out float4 oColor        : COLOR)
{     
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    float3 specColor = tex2D(specMapSampler, uv0.xy);
    
    // modulate by lightmap
    float4 lightmapColor = tex2D(lightMapSampler, uv1);
    diffColor.rgb = diffColor.rgb * lightmapColor.rgb * brightness;

    // modulate diffuse texture with lightvalues from lightbuffer and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity, // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity); // objects specular intensity   
    
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
ColorAlphaPixelShader(float4 projPos    : TEXCOORD0,
                      float3 uv0        : TEXCOORD1,  
                      float2 uv1        : TEXCOORD2,                         
                      float2 screenPos  : VPOS,  
                      out float4 oColor : COLOR)
{    
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
    float4 diffColor = tex2D(diffMapSampler, uv0.xy);
    float3 emsvColor = tex2D(emsvSampler, uv0.xy);
    float3 specColor = tex2D(specMapSampler, uv0.xy);

    // modulate by lightmap
    float4 lightmapColor = tex2D(lightMapSampler, uv1);
    diffColor.rgb = diffColor.rgb * lightmapColor.rgb * brightness;
    
    // modulate diffuse texture with lightvalues from lightSampler and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             emsvColor,            // objects emissive texture color
                             MatEmissiveIntensity, // objects emissive intensity
                             specColor,            // objects specular texture color
                             MatSpecularIntensity); // objects specular intensity   
    
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // FIXME: alphaBlendFactor uses INETSITY0 which is already used by brightness
    // modulate alpha by alphaBlendFactor 
    //oColor.a *= alphaBlendFactor;
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
SimpleTechnique(NormalDepth, "NormalDepth", NormalDepthPassVertexShader, NormalDepthPassPixelShader);
SimpleTechnique(ESMDepth, "ESMDepth", ShadowMappingVertexShader, ShadowMappingPixelShader);
//SimpleTechnique(NormalDepthATest, "NormalDepthATest", NormalDepthPassVertexShader, NormalDepthPassAlphaTestPixelShader);

SimpleTechnique(Solid0, "Solid", ColorVertexShader, ColorPixelShader);
SimpleTechnique(Alpha0, "Alpha", ColorVertexShader, ColorAlphaPixelShader);

