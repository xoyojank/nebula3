#ifndef DEFERRED_SHARED_H
#define DEFERRED_SHARED_H
// includes:
#include "skinning.fxh"
#include "shared.fxh"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define numAlphaLayers 4
const float2 stippleMasks[numAlphaLayers] = {0,0, 
                                             1,0, 
                                             0,1,
                                             1,1};
                                
const float ratio = 0.75f;
#define INFERRED_LIGHTING 1 // inferred with 0.75 gives up to 30% performance on high lightened scenes!

//---------------------------------------------------------------------------------------------------------------------
/**
    ID's are values between 0 and 255 mapped to float 0.0 .. 1.0
*/
bool CompareIdEqual(float a, float b)
{
    float diff = abs(a - b);
    return diff < (0.1f/255.0f);
}

//---------------------------------------------------------------------------------------------------------------------
/**
    Compare current pixel shader pixel data with its dsf data
    false if one or more components are not equal
*/
bool CompareDSFData(float2 dsf, float2 currentObjectNormalGroup)
{
    return 
        // compare Object Ids
        CompareIdEqual(dsf.x, currentObjectNormalGroup.x) && 
        // compare Normal Group Ids
        CompareIdEqual(dsf.y, currentObjectNormalGroup.y);
}
    
//---------------------------------------------------------------------------------------------------------------------
/**
    Conditional Bilinear Sampling, if usePixel is false we dont use the light-sample
    if useNeighbourX is false, the weight in X is 1
    if useNeighbourY is false, the weight in Y is 1
*/    
float4 CalcLightSample(float4 pixel, bool usePixel, bool useNeighbourX, float weightX, bool useNeighbourY, float weightY)
{
    float4 result;
    
    if(!usePixel) 
    {
        result = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {    
        if(!useNeighbourX) weightX = 1.0f;
        if(!useNeighbourY) weightY = 1.0f;
        result = pixel * weightX * weightY;
    }
    
    return result;
}

//-----------------------------------------------HELPER FUNCTIONS---------------------------------------------------------
/**
*/
float4 
SampleLightBuffer(float2 screenPixel, float2 Object_NormalGroup, sampler2D dsfSampler, sampler2D lightSampler)
{       
    // for details look at http://graphics.cs.uiuc.edu/~kircher/inferred/inferred_lighting_paper.pdf
    // d -> (Firgure 4) screen pixel
    // p -> (Firgure 4) associated light buffer pixel      
    
    float2 lightSamplerPixelSize = pixelSize.xy / float2(ratio, ratio);

    // Note: actually here should be "screenPixel + float2(0.5f, 0.5f)", but results were better without
    float2 p_center = screenPixel;
    float2 d_raw = p_center * ratio;
    float2 d_center = floor(d_raw) + float2(0.5f, 0.5f);;      

    // look up 4 pixel in lightSampler
    float2 uv00 = d_center                                     * lightSamplerPixelSize;
    float2 uv10 = float2(d_center.x + 1.0,  d_center.y       ) * lightSamplerPixelSize;
    float2 uv01 = float2(d_center.x,        d_center.y + 1.0f) * lightSamplerPixelSize;
    float2 uv11 = float2(d_center.x + 1.0f, d_center.y + 1.0f) * lightSamplerPixelSize;

    struct PixelData
    {
        float4  c;
        // objectId, normalgroupId, depth
        float2  dsf;
    };
    PixelData p00, p10, p01, p11;

    p00.c = tex2D(lightSampler, uv00);
    p01.c = tex2D(lightSampler, uv01);
    p10.c = tex2D(lightSampler, uv10);
    p11.c = tex2D(lightSampler, uv11);  

    p00.dsf = tex2D(dsfSampler, uv00).xy;
    p01.dsf = tex2D(dsfSampler, uv01).xy;
    p10.dsf = tex2D(dsfSampler, uv10).xy;
    p11.dsf = tex2D(dsfSampler, uv11).xy;  
    
    float x_ratio = frac(d_raw.x);
    float y_ratio = frac(d_raw.y);
    float x_opposite = 1 - x_ratio;
    float y_opposite = 1 - y_ratio;  
  
    bool use00 = CompareDSFData(p00.dsf, Object_NormalGroup);
    bool use10 = CompareDSFData(p10.dsf, Object_NormalGroup);
    bool use01 = CompareDSFData(p01.dsf, Object_NormalGroup);
    bool use11 = CompareDSFData(p11.dsf, Object_NormalGroup);

    float4 pix00 = CalcLightSample(p00.c, use00,     use10, x_opposite,      use01, y_opposite);
    float4 pix10 = CalcLightSample(p10.c, use10,     use00, x_ratio,         use11, y_opposite);
    float4 pix01 = CalcLightSample(p01.c, use01,     use11, x_opposite,      use00, y_ratio);
    float4 pix11 = CalcLightSample(p11.c, use11,     use01, x_ratio,         use10, y_ratio);

    float4 resultLight;
    if((!use00) && (!use10) && (!use01) && (!use11))
    {
        // fallback if all pixels are rejected, normal bilinear filtering
        // wiki:  http://en.wikipedia.org/wiki/Bilinear_filtering
        resultLight = (p00.c * x_opposite * y_opposite)   +   (p10.c * x_ratio * y_opposite) + 
                      (p01.c * x_opposite * y_ratio)      +   (p11.c * x_ratio * y_ratio);                                                                  
    }
    else
    {
        resultLight = pix00 + pix10 + pix01 + pix11;
    }   
    
    return resultLight;
}

//-----------------------------------------------HELPER FUNCTIONS---------------------------------------------------------
/**
*/
float4 
SampleLightBufferAlpha(float2 screenPixel, float2 curObjectId, uniform float index)
{    
#if INFERRED_LIGHTING
    float2 patternOffset = stippleMasks[index];
    float2 lightBufferPixelPos = floor(screenPixel.xy * ratio);
    float2 pattern = float2(fmod(lightBufferPixelPos.x + patternOffset.x, 2), fmod(lightBufferPixelPos.y + patternOffset.y, 2));    
    float2 lightBufferPixelSize = pixelSize.xy / float2(ratio, ratio);
    float2 lightBufferPixel = ((lightBufferPixelPos + float2(0.5f, 0.5f) - pattern) + float2(1,1))* lightBufferPixelSize.xy;
    
    // get four samples from dsf buffer   
    float2 uv = lightBufferPixel;
    float2 uv01 = float2(uv.x + 2.0f * lightBufferPixelSize.x, uv.y);
    float2 uv10 = float2(uv.x, uv.y + 2.0f * lightBufferPixelSize.y);
    float2 uv11 = uv + 2.0f * lightBufferPixelSize.xy;
    
    float2 objectId00 = tex2D(alphaDsfObjectIdSampler, uv).xy;
    float2 objectId01 = tex2D(alphaDsfObjectIdSampler, uv01).xy;
    float2 objectId10 = tex2D(alphaDsfObjectIdSampler, uv10).xy;
    float2 objectId11 = tex2D(alphaDsfObjectIdSampler, uv11).xy;
    
    curObjectId.x /= 255;
    float usesample00 = any(saturate(abs(curObjectId - objectId00) - float2(0.001,0.001))); 
    float usesample01 = any(saturate(abs(curObjectId - objectId01) - float2(0.001,0.001))); 
    float usesample10 = any(saturate(abs(curObjectId - objectId10) - float2(0.001,0.001)));     
    float usesample11 = any(saturate(abs(curObjectId - objectId11) - float2(0.001,0.001)));  
    
    float4 c00 = tex2D(alphaLightSampler, uv);
    float4 c01 = tex2D(alphaLightSampler, uv01);
    float4 c10 = tex2D(alphaLightSampler, uv10);
    float4 c11 = tex2D(alphaLightSampler, uv11);   

    // calculate bilinear weights, throw away wrong samples
    float2 ratios = frac((screenPixel.xy + float2(0.5f, 0.5f))* pixelSize.xy);    
    float lerpXFactor = saturate(ratios.x + usesample00 - usesample01);
    float4 lerped0001 = lerp(c00, c01, lerpXFactor);
    lerpXFactor = saturate(ratios.x + usesample10 - usesample11);
    float4 lerped1011 = lerp(c10, c11, lerpXFactor);
    float lerpYFactor = saturate(ratios.y + (usesample00 + usesample01 > 0) - (usesample10 + usesample11 > 0));
    float4 resultLight = lerp(lerped0001, lerped1011, lerpYFactor);
#else
    float2 uv = (screenPixel + float2(0.5f, 0.5f))* pixelSize.xy;
    float4 resultLight = tex2D(lightSampler, uv);
#endif              
    return resultLight;
}

//---------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------NORMAL DEPTH----------------------------------------------------------
/**
    The NormalDepth pass, this fills the color buffer in xy with
    2 surface normal components, and zw with the depth value from 0 to 1 (0 is
    near plane, 1 is far plane) encoded in 16 bit.
*/
void
NormalDepthPassVertexShader(float4 position         : POSITION,
                            float4 normal           : NORMAL,
                            float2 uv0              : TEXCOORD0,
                            float4 tangent          : TANGENT,
                            float4 binormal         : BINORMAL,
                            out float4 oPos         : POSITION,
                            out float2 oUv0         : TEXCOORD0,
                            out float3 oTangent     : TEXCOORD1,
                            out float3 oNormal      : TEXCOORD2,
                            out float3 oBinormal    : TEXCOORD3,
                            out float4 viewSpacePos : TEXCOORD4)
{
    oPos = mul(position, mvp);
    oUv0 = UnpackUv(uv0).xy;
    viewSpacePos.xyz = mul(position, modelView).xyz;
    viewSpacePos.w = normal.w;
    
    // setup matrix components to transform tangent space normals 
    // into view space
    oTangent  = mul(imv, UnpackNormal4(tangent)).xyz;
    oNormal   = mul(imv, UnpackNormal4(normal)).xyz;
    oBinormal = mul(imv, UnpackNormal4(binormal)).xyz;
} 

//---------------------------------------------------------------------------------------------------------------------
void
NormalDepthPassPixelShader(float2 uv0             : TEXCOORD0,
                           float3 tangent         : TEXCOORD1,
                           float3 normal          : TEXCOORD2,
                           float3 binormal        : TEXCOORD3,
                           float4 viewSpacePos    : TEXCOORD4,
                           out float4 oColor      : COLOR0,
                           out float4 oDSF        : COLOR1)
{
    float3x3 tangentViewMatrix = float3x3(normalize(tangent.xyz), normalize(binormal.xyz), normalize(normal.xyz));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);

    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, viewSpacePos.w, length(viewSpacePos.xyz));
}

//---------------------------------------------------------------------------------------------------------------------
void
AlphaTestNormalDepthPassPixelShader(float2 uv0             : TEXCOORD0,
                                   float3 tangent         : TEXCOORD1,
                                   float3 normal          : TEXCOORD2,
                                   float3 binormal        : TEXCOORD3,
                                   float4 viewSpacePos    : TEXCOORD4,
                                   out float4 oColor      : COLOR0,
                                   out float4 oDSF        : COLOR1)
{
    float alpha = tex2D(diffMapSampler, uv0.xy).a;
    clip(alpha - 0.01f);

    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);

    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, viewSpacePos.w, length(viewSpacePos.xyz));
}

//---------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------SKINNED NORMAL DEPTH--------------------------------------------------
/**
    Normal depth pass with skinned position, normal, tangent and binormal.
*/
void
SkinnedNormalDepthPassVertexShader(float4 position          : POSITION,
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
                                    out float4 viewSpacePos : TEXCOORD4)
{
    float4 skinnedPos      = SkinnedPosition(position, weights, indices);
    float3 skinnedNormal   = SkinnedNormal(normalize(UnpackNormal4(normal)), weights, indices);
    float3 skinnedTangent  = SkinnedNormal(normalize(UnpackNormal4(tangent)), weights, indices);
    float3 skinnedBinormal = SkinnedNormal(normalize(UnpackNormal4(binormal)), weights, indices);
    
    oPos = mul(skinnedPos, mvp);
    oUv0 = UnpackUv(uv0).xy;
    viewSpacePos.xyz = mul(skinnedPos, modelView).xyz;
    viewSpacePos.w = normal.w;
    
    // setup matrix components to transform tangent space normals 
    // into view space
    oTangent  = mul(imv, skinnedTangent).xyz;
    oNormal   = mul(imv, skinnedNormal).xyz;
    oBinormal = mul(imv, skinnedBinormal).xyz;
} 

//---------------------------------------------------------------------------------------------------------------------
/**
    Normal depth pass with rim lighting for characters
*/
void
RimNormalDepthPassPixelShader(float2 uv0             : TEXCOORD0,
                              float3 tangent         : TEXCOORD1,
                              float3 normal          : TEXCOORD2,
                              float3 binormal        : TEXCOORD3,
                              float4 viewSpacePos    : TEXCOORD4,
                              out float4 oColor      : COLOR0,
                              out float4 oDSF        : COLOR1)
{
    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);

    //float rimTerm = RimLightIntensity(viewSpaceNormal, normalize(viewSpacePos)); 

    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, viewSpacePos.w, length(viewSpacePos.xyz));
}

//---------------------------------------------------------------------------------------------------------------------
/**
    Normal depth pass with rim lighting for characters
*/
void
AlphaTestRimNormalDepthPassPixelShader(float2 uv0             : TEXCOORD0,
                              float3 tangent         : TEXCOORD1,
                              float3 normal          : TEXCOORD2,
                              float3 binormal        : TEXCOORD3,
                              float4 viewSpacePos    : TEXCOORD4,
                              out float4 oColor      : COLOR0,
                              out float4 oDSF        : COLOR1)
{
    float alpha = 255 * tex2D(diffMapSampler, uv0.xy).a;
    clip(alpha - 254);
    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);

    //float rimTerm = RimLightIntensity(viewSpaceNormal, normalize(viewSpacePos)); 
    
    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, viewSpacePos.w, length(viewSpacePos.xyz));
}

//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- ALPHA NORMAL DEPTH---------------------------------------------------
void
AlphaStippleNormalDepthPixelShader(float2 uv0             : TEXCOORD0,
                                   float3 tangent         : TEXCOORD1,
                                   float3 normal          : TEXCOORD2,
                                   float3 binormal        : TEXCOORD3,
                                   float4 viewSpacePos    : TEXCOORD4,
                                   float2 screenPos       : VPOS,
                                   out float4 oColor      : COLOR0,
                                   out float4 oDSF        : COLOR1)
{    
    // the object id determines one of three stipple masks
    int index = fmod(ObjectId, numAlphaLayers);
    float2 patternOffset = stippleMasks[index];
    float2 writePixel;
    writePixel.x = (fmod(screenPos.x + patternOffset.x, 2) * fmod(screenPos.y + patternOffset.y, 2)) - 0.5f;
    // alphatest
    float alpha = 255 * tex2D(diffMapSampler, uv0.xy).a;
    writePixel.x = max(writePixel.x, alpha - 254); // full opaque pixel are drawn anyway
    writePixel.y = alpha - 1; // full transparent pixels aren't drawn
    clip(writePixel);
    
    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);

    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, viewSpacePos.w, length(viewSpacePos.xyz));
}

//----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------COLOR PASS-------------------------------------------------------------
//----------------------------------------------- SKINNED --------------------------------------------------------------
void
SkinnedColorVertexShader(float4 position           : POSITION,
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
                         out float3 oWorldSpacePos : TEXCOORD3)
{
    float4 skinnedPos = SkinnedPosition(position, weights, indices);
    oPos = mul(skinnedPos, mvp);
    oViewSpacePos.xyz = mul(skinnedPos, modelView).xyz;
    oViewSpacePos.w = normal.w;
    oWorldSpacePos = mul(skinnedPos, model).xyz;
    oProjPos = oPos;
    oUv0.xy = UnpackUv(uv0).xy;   
} 

#endif

