//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "config.fxh"
#include "util.fxh"

float4 rgbaMask : RgbaMask;
shared float4x4 modelView : ModelView;
shared float4x4 viewProjection : ViewProjection;
const float4 swingAmplitude : SwingAmplitude = float4(0.1, -0.05, 0.1, 0.0);
const float swingFrequency : SwingFrequency = 0.3;
const float3 swingPosVariation : SwingPosVariation = float3(0.1, 0.1, 0.1);
const float PI = 3.1415926535897932384626433832795;
float minDist : MinDist = 90.0f;
float maxDist : MaxDist = 110.0f;
shared float time : Time;

texture grassMap : DiffMap0;
sampler GrassSampler = sampler_state
{
    Texture = <grassMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;
};

texture groundColorMap : DiffMap1;
sampler GroundColorSampler = sampler_state
{
    Texture = <groundColorMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
};

texture lightMap : DiffMap2;
sampler GroundLightmapSampler = sampler_state
{
    Texture = <lightMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
};

texture normalMap : DiffMap3;
sampler GroundNormalSampler = sampler_state
{
    Texture = <normalMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
};

texture weightMap : DiffMap4;
sampler GroundWeightSampler = sampler_state
{
    Texture = <weightMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
};

struct vsInputGrass
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
    float2 uv1      : TEXCOORD1;        // dynamic height and weight
};

struct vsOutputGrassDepth
{
    float4 position      : POSITION;
    float4 uv01          : TEXCOORD0;
    float2 depthAlpha    : TEXCOORD1;  // depth value, vertex alpha
};

struct vsOutputGrass
{
    float4 position : POSITION;
    float4 uv01     : TEXCOORD0;
    float4 projPos  : TEXCOORD1;
    float4 worldLightVecAndHeight : TEXCOORD2;
    float2 alphaFog : TEXCOORD3;
    float4 worldPos : TEXCOORD4;
};

//------------------------------------------------------------------------------
/**
    Apply a swing on the provided vertex.
*/
float4 ApplyVertexSwing(float4 pos)
{
    return pos + (pos.y * swingAmplitude * sin(dot(pos.xyz, swingPosVariation) + time * swingFrequency * PI));
}

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
    Vertex shader: generate depth values for static geometry with uv 
    coordinates.
*/
vsOutputGrassDepth vsGrassDepth(const vsInputGrass vsIn)
{
    vsOutputGrassDepth vsOut;

    // shift the position a bit underground
    float4 inPos = vsIn.position - float4(0.0, 0.05, 0.0, 0.0);
    
    float4 modelPos = ApplyVertexSwing(inPos) + float4(0.0f, vsIn.uv1.x / 256.0, 0.0f, 0.0f);
    float fade = saturate((length(mul(modelPos, modelView).xyz) - minDist) / (maxDist - minDist));
    
    // skew grass a bit to prevent "cross artefacts" when looking vertically onto the grass
    modelPos.xz += imv._m10_m12 * inPos.y;
    vsOut.position = mul(modelPos, mvp);
    vsOut.uv01.xy = UnpackUv(vsIn.uv0);
    
    // compute uv coordinates for ground map lookups
    float4 groundMapUvs = mul(vsIn.position, textureTransform0);
    vsOut.uv01.zw = groundMapUvs.xz;

    // compute depth and distance alpha fade    
    vsOut.depthAlpha.x = vsOut.position.z;
    vsOut.depthAlpha.y = (1.0f - fade) * vsIn.uv1.y;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader: generate depth values for static geometry with
    alpha test.
*/
float4 psGrassDepth(const vsOutputGrassDepth psIn) : COLOR
{
    float alpha = tex2D(GrassSampler, psIn.uv01.xy).a;
    float2 uv = psIn.uv01.zw;    
    float4 rgbaTest = rgbaMask;
    float4 weight = tex2D(GroundWeightSampler, uv);
    alpha *= dot(rgbaTest, weight);   // multiply alpha with per-pixel weight value
    
    // fade out earler than color pass to avoid dark pixels, dependend on fog distance    
    float dist = 1 - saturate((fogDistances.y - psIn.depthAlpha.x) / (fogDistances.y - fogDistances.x));
    //alpha *= psIn.depthAlpha.y * (1.0f - 0.3 * dist);
    return float4(psIn.depthAlpha.x, 0.0f, 0.0f, alpha);
}

//------------------------------------------------------------------------------
/**
    Vertex shader for the grass renderer.
*/
vsOutputGrass vsGrass(const vsInputGrass vsIn)
{
    vsOutputGrass vsOut;
    
    // shift the position a bit underground
    float4 inPos = vsIn.position - float4(0.0, 0.05, 0.0, 0.0);

    float4 modelPos = ApplyVertexSwing(inPos) + float4(0.0f, vsIn.uv1.x / 256.0, 0.0f, 0.0f);
    float fade = saturate((length(mul(modelPos, modelView).xyz) - minDist) / (maxDist - minDist));
    
    // skew grass a bit to prevent "cross artefacts" when looking vertically onto the grass
    modelPos.xz += imv._m10_m12 * inPos.y;    
    vsOut.position = mul(modelPos, mvp);
    vsOut.projPos = vsOut.position;     
    vsOut.worldPos = mul(modelPos, model);
    vsOut.uv01.xy = UnpackUv(vsIn.uv0);

    // compute uv coordinates for ground map lookups
    float4 groundMapUvs = mul(vsIn.position, textureTransform0);
    vsOut.uv01.zw = groundMapUvs.xz;
    
    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME  
    // compute light vector and half vector in global space
    float3 globalPos = mul(modelPos, model);
    float3 eyeVec = normalize(eyePos - globalPos);
//     int lightIndex;
//     for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
//     {
//         // point light
//         vsOut.worldLightVecAndHeight.xyz += LightPos - globalPos;
//     }
//     vsOut.worldLightVecAndHeight.xyz = normalize(vsOut.worldLightVecAndHeight.xyz);
    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME  
    
    // directional light
    vsOut.worldLightVecAndHeight.xyz = globalLightDir;
    vsOut.worldLightVecAndHeight.w = vsIn.position.y;
    vsOut.alphaFog.x = (1.0f - fade) * vsIn.uv1.y;
    vsOut.alphaFog.y = length(eyePos - modelPos); // FIXME use modelEyePos
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader single-pass lighting function, performs lighting
    in world space.
*/
float4
psLightGrass(int numLocalLights,           // number of local lights
            float4 worldPos,              // surface position in world space
            float4 projPos,               // surface position in projection space
            float3 worldNormal,           // surface normal in world space
            float4 matDiffuse,           // material emissive component
            float  pointLightWeight)
{
    float4 diffuse = globalAmbientLightColor;
    float4 specular = 0;

    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME    
    // compute without specular, check shader compiler if its already deleting spec term computation
    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME
    
    float4 nullVec = (0,0,0,0);
    
    // evaluate global light    
    GlobalLight(worldPos, projPos, worldNormal, nullVec, 0, 0, diffuse, specular);
    
    // evaluate local lights
    int lightIndex;
    float4 pointLightColor = 0;
    for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
    {
        PointLight(lightIndex, worldPos, worldNormal,  nullVec, 0, pointLightColor, specular);
    }
    diffuse += pointLightColor * pointLightWeight;
    float4 color = diffuse * matDiffuse;
    return color;
}
//------------------------------------------------------------------------------
/**
    Pixel shader for the grass renderer.
*/
float4 psGrass(const vsOutputGrass psIn, uniform int numLights) : COLOR
{
    float4 weight = tex2D(GroundWeightSampler, psIn.uv01.zw);
    //return weight;
    return float4(dot(rgbaMask, weight), 0, 0, 1);
    
    float4 diffColor = tex2D(GrassSampler, psIn.uv01.xy);
    diffColor.a *= psIn.alphaFog.x * dot(rgbaMask, tex2D(GroundWeightSampler, psIn.uv01.zw));
    clip(diffColor.a - 0.05);

    // get ground color, ground lightmap color and ground normal map    
    float4 groundColor = tex2D(GroundColorSampler, psIn.uv01.zw);
    float4 groundLightmap = tex2D(GroundLightmapSampler, psIn.uv01.zw) * 2.0;
    float4 groundNormal = (tex2D(GroundNormalSampler, psIn.uv01.zw) * 2.0) - 1.0;

    float4 finalColor = lerp(groundColor, diffColor, saturate(psIn.worldLightVecAndHeight.w * 3.0));
    finalColor.rgb *= groundLightmap.rgb;
    finalColor = psLightGrass(numLights, psIn.worldPos, psIn.projPos, groundNormal, finalColor, finalColor.a);// psIn.alphaFog.y);
    finalColor.a = diffColor.a;
    
    finalColor = EncodeHDR(finalColor);
    
    return finalColor;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;

//------------------------------------------------------------------------------
/**
    Special Techniques for grass patches alpha blending with alpha test.
*/        
#define SimpleTechniqueATest(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile vs_3_0 vertexShader(); \
        PixelShader     = compile ps_3_0 pixelShader(); \
        CullMode        = <CullMode>; \
        AlphaRef        = 180; \
        AlphaTestEnable = True; \
        AlphaFunc       = GreaterEqual; \
    } \
}

// #define LightTechniqueATest(name, features, vertexShader, pixelShader, numLights) \
// technique name < string Mask = features; > \
// { \
//     pass p0 \
//     { \
//         VertexShader    = compile vs_3_0 vertexShader(); \
//         PixelShader     = compile ps_3_0 pixelShader(numLights); \
//         CullMode        = <CullMode>; \
//         AlphaRef        = 180; \
//         AlphaTestEnable = True; \
//     } \
// }
SimpleTechniqueATest(DepthGrass, "DepthGrass", vsGrassDepth, psGrassDepth);
SimpleTechniqueATest(VSMDepth, "VSMDepth", DepthPassVertexShader, VSMPassPixelShader);

LightTechnique(Alpha0, "Alpha|LocalLights0", vsGrass, psGrass, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", vsGrass, psGrass, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", vsGrass, psGrass, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", vsGrass, psGrass, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", vsGrass, psGrass, 4);
