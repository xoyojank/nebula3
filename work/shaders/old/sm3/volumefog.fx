//------------------------------------------------------------------------------
//  volumefog.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"
#include "util.fxh"

float depthDensity  : Intensity0 = 1.0f;
float alphaOffset   : Intensity1 = 0.0f;
float alphaDensity  : Intensity2 = 0.0f;
float layerDistance : Intensity3 = 0.0f;

shared float time : Time;
// NOTE: number of layers is hardcoded into pixel and vertex shader!
const float2 LayerVelocities[4] = { {  1.0,   1.0 },
                                    { -0.9,  -0.8 },
                                    {  0.8,  -0.9 },
                                    { -0.4,   0.5 }};

struct vsColorPassInput
{
    float4 position : POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 uv0      : TEXCOORD0;    
};

struct vsColorPassOutput
{
    float4 position      : POSITION;
    float4 color         : COLOR0;
    float4 uv01          : TEXCOORD0;
    float4 uv23          : TEXCOORD1;
    float4 projPos       : TEXCOORD2;    
    float4 worldPos      : TEXCOORD3;     // position in world space    
    float3 normal        : TEXCOORD4;     // vertex normal in world space
    float3 tangent       : TEXCOORD5;     // vertex tangent in world space
    float3 binormal      : TEXCOORD6;     // vertex binormal in world space
    float3 worldEyeVec   : TEXCOORD7;     // normalized world space eye vector
    float4 depth         : TEXCOORD8;
};

//------------------------------------------------------------------------------
/**
*/
void SampleTexture(in float2 uv, in float4 vertexColor, inout float4 dstColor)
{
    float4 srcColor = tex2D(diffMapSampler, uv) * vertexColor;
    dstColor.rgb = lerp(dstColor.rgb, srcColor.rgb, srcColor.a);
    dstColor.a += srcColor.a * 0.25;
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

    // compute animated / raycasted uvs
    float2 uvs[4];
    float2 uvDiff = vsOut.worldEyeVec * layerDistance;
    float2 uvOffset = -4 * uvDiff;
    int i;
    for (i = 0; i < 4; i++)
    {
        uvOffset += uvDiff;
        uvs[i] = UnpackUv(vsIn.uv0) + uvOffset + velocity.xy * time * LayerVelocities[i];
    }
    
    vsOut.uv01.xy = uvs[0];
    vsOut.uv01.zw = uvs[1];
    vsOut.uv23.xy = uvs[2];
    vsOut.uv23.zw = uvs[3];
    
    // encode silhouette factor into alpha of per-vertex color    
    vsOut.color = vsIn.color; 
    float dotProduct = dot(vsOut.normal, vsOut.worldEyeVec);    
    vsOut.color.a = dotProduct*dotProduct;
    
    // calculate depth
    vsOut.depth.xy = vsOut.position.xy / vsOut.position.w;
    vsOut.depth.zw = vsOut.position.zw;
    
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    // sample 4 layers
    float4 dstColor = { 0, 0, 0, 0 };    
    SampleTexture(psIn.uv01.xy, psIn.color, dstColor);
    SampleTexture(psIn.uv01.zw, psIn.color, dstColor);
    SampleTexture(psIn.uv23.xy, psIn.color, dstColor);
    SampleTexture(psIn.uv23.zw, psIn.color, dstColor);
    
    // compute bump map    
    float3x3 tangentToWorld = float3x3(psIn.tangent, psIn.binormal, psIn.normal);
    float3 norm0 = SampleNormal(bumpMapSampler,  psIn.uv01.xy);
    float3 norm1 = SampleNormal(bumpMapSampler,  psIn.uv01.zw);
    float3 worldNormal = mul(normalize(norm0 + norm1), tangentToWorld);    
    
    // iluminate that
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, dstColor, float4(0,0,0,1), MatSpecularPower, dstColor);
   
    // calc depths (use inverted projection matrix to retrieve the screen space Z
    // this is required to get a linear difference value
    float bgDepth = DecodeDepth(tex2D(depthSampler, psComputeScreenCoord(psIn.projPos)).x);  
    float4 bufferDepth   = mul( float4( psIn.depth.xy, bgDepth, 1.0f), invProj);
    float4 particleDepth = mul( float4( psIn.depth.xy, psIn.depth.z/psIn.depth.w, 1.0f), invProj);
    
    // THIS FOLLOWING LINE SHOULD BE RIGHT
    // float depthDiff = bufferDepth.z/bufferDepth.w - particleDepth.z/particleDepth.w;
    // BUT THIS ONE WORKS ?????
    float depthDiff = particleDepth.z/particleDepth.w - bufferDepth.z/bufferDepth.w;
    
    // modulate alpha by depth
    float modAlpha = saturate(depthDiff * 5.0f) * alphaDensity * depthDensity * 2;
    color.a = dstColor.a * modAlpha;   
    color = psFog(psIn.projPos.z, color);
    color.rgb *= color.a;
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 1;
int AlphaRef : AlphaRef = 1;
//------------------------------------------------------------------------------

LightTechnique(Alpha0, "Alpha|LocalLights0", ColorPassVertexShader, ColorPassPixelShader, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", ColorPassVertexShader, ColorPassPixelShader, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", ColorPassVertexShader, ColorPassPixelShader, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", ColorPassVertexShader, ColorPassPixelShader, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", ColorPassVertexShader, ColorPassPixelShader, 4);
