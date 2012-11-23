//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "config.fxh"
#include "util.fxh"
#include "shared.fxh"

float4 rgbaMask : RgbaMask;
const float4 swingAmplitude : SwingAmplitude = float4(0.1, -0.05, 0.1, 0.0);
const float swingFrequency : SwingFrequency = 0.3;
const float3 swingPosVariation : SwingPosVariation = float3(0.1, 0.1, 0.1);
const float PI = 3.1415926535897932384626433832795;
float minDist : MinDist = 90.0f;
float maxDist : MaxDist = 110.0f;

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

texture normalMapGround : DiffMap3;
sampler GroundNormalSampler = sampler_state
{
    Texture = <normalMapGround>;
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
    vsOut.worldLightVecAndHeight.xyz = 0;
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
    
    float4 color = 0;
    return color;
}
//------------------------------------------------------------------------------
/**
    Pixel shader for the grass renderer.
*/
float4 psGrass(const vsOutputGrass psIn) : COLOR
{  
    
    return 0;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;

//------------------------------------------------------------------------------
/**
    Special Techniques for grass patches alpha blending with alpha test.
*/        
/* #define SimpleTechniqueATest(name, features, vertexShader, pixelShader) \
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
*/
SimpleTechnique(Alpha0, "Alpha", vsGrass, psGrass);

