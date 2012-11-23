
//------------------------------------------------------------------------------
//  volumefog.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "common.fxh"
#include "util.fxh"
#include "deferred_shared.fxh"

float depthDensity  : Intensity0 = 1.0f;
float alphaOffset   : Intensity1 = 0.0f;
float alphaDensity  : Intensity2 = 0.0f;
float layerDistance : Intensity3 = 0.0f;

// NOTE: number of layers is hardcoded into pixel and vertex shader!
const float2 LayerVelocities[4] = { {  1.0,   1.0 },
                                    { -0.9,  -0.8 },
                                    {  0.8,  -0.9 },
                                    { -0.4,   0.5 }};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
void SampleTexture(in float2 uv, in float4 vertexColor, inout float4 dstColor)
{
    float4 srcColor = tex2D(diffMapSampler, uv) * vertexColor;
    dstColor.rgb = lerp(dstColor.rgb, srcColor.rgb, srcColor.a);
    dstColor.a += srcColor.a * 0.25;
}

//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorVertexShader(float4 position          : POSITION,
                  float4 color             : COLOR0,
                  float3 normal            : NORMAL,
                  float2 uv0               : TEXCOORD0,                    
                  float2 uv1               : TEXCOORD1,
                  out float4 oPos          : POSITION,
                  out float4 oProjPos      : TEXCOORD0,
                  out float4 oUv01         : TEXCOORD1,
                  out float4 oUv23         : TEXCOORD2,
                  out float4 oDepth        : TEXCOORD3,
                  out float4 oColor        : TEXCOORD4,
                  out float4 oViewSpacePos : TEXCOORD5)
{
    oPos = mul(position, mvp);
    oProjPos = oPos;
    
    float4 worldPos    = mul(position, model);
    float3 worldEyeVec = normalize(eyePos - worldPos);
    float3 worldNormal = normalize(mul(UnpackNormal(normal), model));
    // compute animated / raycasted uvs
    float2 uvs[4];
    float2 uvDiff = worldEyeVec * layerDistance;
    float2 uvOffset = -4 * uvDiff;
    int i;
    for (i = 0; i < 4; i++)
    {
        uvOffset += uvDiff;
        uvs[i] = UnpackUv(uv0) + uvOffset + velocity.xy * time * LayerVelocities[i];
    }
    
    oUv01.xy = uvs[0];
    oUv01.zw = uvs[1];
    oUv23.xy = uvs[2];
    oUv23.zw = uvs[3];
    
    // encode silhouette factor into alpha of per-vertex color    
    oColor = color; 
    float dotProduct = dot(worldNormal, worldEyeVec);    
    oColor.a = dotProduct*dotProduct;
    
    // calculate depth
    oDepth.xy = position.xy / position.w;
    oDepth.zw = position.zw;
    
    oViewSpacePos = mul(position, modelView);
}     

//---------------------------------------------------------------------------------------------------------------------
void
ColorPixelShader(float4 projPos      : TEXCOORD0,
                 float4 uv01         : TEXCOORD1,
                 float4 uv23         : TEXCOORD2,
                 float4 depth        : TEXCOORD3,
                 float4 color        : TEXCOORD4,
                 float4 viewSpacePos : TEXCOORD5,
                 float2 screenPos    : VPOS,  
                 out float4 oColor   : COLOR)
{     
    // compute uv-coord for light buffer lookup
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    
    // sample 4 layers
    float4 dstColor = { 0, 0, 0, 0 };    
    SampleTexture(uv01.xy, color, dstColor);
    SampleTexture(uv01.zw, color, dstColor);
    SampleTexture(uv23.xy, color, dstColor);
    SampleTexture(uv23.zw, color, dstColor);
    
    // compute bump map          
    float3 viewVec = normalize(viewSpacePos.xyz);   
    float4 normalPacked = tex2D(normalMapSampler, screenUv);    
    float3 viewSpaceNormal = UnpackViewSpaceNormal(normalPacked);    
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float depthDSF = UnpackDepth(objectDepthPacked);    
    float3 surfacePos = viewVec * depthDSF;

    float depthDiff = viewSpacePos.z - surfacePos.z;
    
    // modulate alpha by depth
    float modAlpha = saturate(depthDiff * 5.0f) * depthDensity * alphaDensity  * 2;
    oColor.rgb = dstColor.rgb;
    oColor.a = dstColor.a * modAlpha;
    oColor = psFog(projPos.z, oColor);    
    oColor = EncodeHDR(oColor);
}    
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------- VARIATIONS ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// FIXME: move cullmode and alpharef to better position if possible!
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 120;

SimpleTechnique(Alpha, "Alpha", ColorVertexShader, ColorPixelShader);
SimpleTechnique(AlphaLit, "AlphaLit", ColorVertexShader, ColorPixelShader);
SimpleTechnique(NormalDepth, "NormalDepth", ColorVertexShader, ColorPixelShader);
SimpleTechnique(Additive, "Additive", ColorVertexShader, ColorPixelShader);
