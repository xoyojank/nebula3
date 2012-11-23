//------------------------------------------------------------------------------
//  lightsources.fx
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "util.fxh"
#include "common.fxh"
#include "defaultsampler.fxh"

float4x4 lightProjTransform             : LightProjTransform;
float4x4 shadowTransform                : ShadowTransform;
float4x4 shadowProjTransform            : ShadowProjTransform;
float4 lightPosRange                    : LightPosRange;
float4 lightColor                       : LightColor;
float4 shadowOffsetScale                : ShadowOffsetScale;

float4 globalLightDir                   : GlobalLightDir;
float4 globalLightColor                 : GlobalLightColor;
float4 globalBackLightColor             : GlobalBackLightColor;
float4 globalAmbientLightColor          : GlobalAmbientLightColor;
float globalBackLightOffset             : GlobalBackLightOffset; 

const float specPower = 32.0f;
const float rimLighting = 0.0f;

#define CAN_FILTER_R32F                 1
const float3 luminanceValue = (0.299f, 0.587f, 0.114f);
const float exaggerateSpec = 1.8f;

//texture holds spotlight cone attenuation
sampler lightProjMapSampler = sampler_state 
{
    Texture     = <lightProjMap>;
    AddressU    = BORDER;
    AddressV    = BORDER;
    BorderColor = {0,0,0,0};
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
};

//texture holds shadow map (currently only of spotlights)
sampler shadowProjMapSampler = sampler_state 
{
    Texture     = <shadowProjMap>;
    AddressU    = Border;
    AddressV    = Border;
    BorderColor = 0xFFFFFFFF;
#if CAN_FILTER_R32F
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
#else    
    MinFilter   = Point;
    MagFilter   = Point;
    MipFilter   = Point;
#endif
};

sampler ssaoMapSampler = sampler_state 
{
    Texture     = <ssaoBuffer>;
    AddressU    = Clamp;
    AddressV    = Clamp;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
};

//---------------------------------------------------------------------------------------------------------------------------
/**
    Sample screen space ambient occlusion term with blurring
*/

// a 5x5 gaussian blur filter kernel (reduced to 13 significant samples)  
#define NUMSAMPLES 13     

                                                 
/* float2 sampleOffsets[NUMSAMPLES] = {{0,0},
                                    {1,0},
                                    {0,1},
                                    {1,1}}; */

float4
SampleAmbientOcclusionTerm(in float2 uv)
{    
    float3 sampleOffsetWeights[NUMSAMPLES] = {
                                    { -1.5,  0.5, 0.024882 },
                                    { -0.5, -0.5, 0.067638 },
                                    { -0.5,  0.5, 0.111515 },
                                    { -0.5,  1.5, 0.067638 },
                                    {  0.5, -1.5, 0.024882 },
                                    {  0.5, -0.5, 0.111515 },
                                    {  0.5,  0.5, 0.183858 },
                                    {  0.5,  1.5, 0.111515 },
                                    {  0.5,  2.5, 0.024882 },
                                    {  1.5, -0.5, 0.067638 },
                                    {  1.5,  0.5, 0.111515 },
                                    {  1.5,  1.5, 0.067638 },
                                    {  2.5,  0.5, 0.024882 },
                                };
                                    
    float2  curPixelDSF = tex2D(dsfObjectDepthSampler, uv).xy;
    float4 sample = float4(0.0, 0.0, 0.0, 0.0); 
    int i;
    for (i = 0; i < NUMSAMPLES; i++)
    {
        float2 sampleUV = uv + sampleOffsetWeights[i].xy * pixelSize.xy;
        float2 sampleDSF = tex2D(dsfObjectDepthSampler, sampleUV).xy;
        float usesample = 1 - any(saturate(abs(curPixelDSF - sampleDSF) - float2(0.001,0.001)));
        
        if (usesample)
        {
            float4 curSample = tex2D(ssaoMapSampler, sampleUV);
            sample += sampleOffsetWeights[i].z * curSample;     
        }
        else
        {
            // fix weights
            int leftSamples = NUMSAMPLES - i;
            float restWeight = sampleOffsetWeights[i].z / leftSamples;
            int j;
            for (j = i+1; j < leftSamples; j++)
            {
                sampleOffsetWeights[j].z += restWeight;
            }
        }        
    }       
    return 1 - sample;
}

//----------------------------------------------- GLOBAL --------------------------------------------------------------------
/**
    Global-light shader.
    The global light is rendered as a fullscreen quad.
*/
void
GlobalLightVertexShader(float4 position                    : POSITION,
                         float2 uv0                        : TEXCOORD0,     
                         out float4 oPos                   : POSITION,
                         out float2 oUv0                   : TEXCOORD0,
                         out float3 oViewSpacePos          : TEXCOORD1)
{
    oPos = position;
    oUv0 = uv0; 
    oViewSpacePos = float3(position.xy * focalLength.xy, -1.0f);
}

//---------------------------------------------------------------------------------------------------------------------------
/**
*/
void
GlobalLightPixelShader(float2 uv0                       : TEXCOORD0,     
                       float3 viewSpacePos              : TEXCOORD1,
                       out float4 oColor                : COLOR)
{   
    // reconstruct view space normal from NormalDepthBuffer (note: pos is dropped)
        
    float4 normalPacked = tex2D(normalMapSampler, uv0);    
    float3 viewSpaceNormal = UnpackViewSpaceNormal(normalPacked);            
        
    // compute N.L diffuse component
    float NL = dot(globalLightDir.xyz, viewSpaceNormal);
    
    float3 diff = globalAmbientLightColor.xyz;
    diff += globalLightColor.xyz * saturate(NL);
    diff += globalBackLightColor.xyz * saturate(-NL + globalBackLightOffset);   

    // sample ambient oclussion term
    //float4 ssao = SampleAmbientOcclusionTerm(uv0);
    //diff *= ssao * ssao;
    
    // compute specular component N.H^shinyness
    float3 H = normalize(globalLightDir.xyz - viewSpacePos);
    float NH = saturate(dot(viewSpaceNormal, H));
    float diffLuminance = dot(diff, luminanceValue) * exaggerateSpec;
    float spec = pow(NH, specPower) * diffLuminance;    
    //spec *= ssao.r;
    
    // add optional rim lighting
    //float rimLighting = objectIdMaterial.w;
    diff += (float3(rimLighting, rimLighting, rimLighting) * saturate(NL - globalBackLightOffset));
    
    // rgb holds the diffuse light color, which will be added to prevoius lighten pixels
    // alpha channel holds N dot H , which must not be added in alpha blend process, we use max to get the brightest specular of multiple lights
    oColor = float4(diff, spec);
    
    // encode hdr, scale diffuse light
    oColor = EncodeHDR(oColor);
}

//---------------------------------------------------------------------------------------------------------------------------
/**
*/
float 
GetInvertedOcclusion(float receiverDepthInLightSpace,
                     float2 lightSpaceUv)
{
    if (lightSpaceUv.x <= 0.05f || lightSpaceUv.y <= 0.05f
        || lightSpaceUv.x >= 0.95f || lightSpaceUv.y >= 0.95f)
    {
        return 1.0f;
    }
    // offset and scale shadow lookup tex coordinates
    lightSpaceUv.xy *= shadowOffsetScale.zw;
    lightSpaceUv.xy += shadowOffsetScale.xy;
    
    // apply bias
    const float shadowBias = shadowConstants.z;
    float receiverDepth = shadowConstants.x * receiverDepthInLightSpace - shadowBias;

#if CAN_FILTER_R32F
    float occluder = tex2D(shadowProjMapSampler, lightSpaceUv); 
#else
    // emulate bilinear filtering (not needed if your GPU can filter FP32 textures)
    float2 unnormalized = lightSpaceUv.xy * shadowConstants.w;
    float2 fractional = frac(unnormalized);
    unnormalized = floor(unnormalized);
    
    // ----- BILINEAR SAMPLING
    float4 exponent;                   
    exponent.x = tex2D(shadowProjMapSampler, (unnormalized + float2( -0.5f, 0.5f ))/ shadowConstants.w ).x;
    exponent.y = tex2D(shadowProjMapSampler, (unnormalized + float2( 0.5f, 0.5f ))/ shadowConstants.w ).x;
    exponent.z = tex2D(shadowProjMapSampler, (unnormalized + float2( 0.5f, -0.5f ))/ shadowConstants.w ).x;
    exponent.w = tex2D(shadowProjMapSampler, (unnormalized + float2( -0.5f, -0.5f ))/ shadowConstants.w ).x;

    float4 occluder4 = exponent;       
    float occluder = (occluder4.w + (occluder4.x - occluder4.w) * fractional.y);
    occluder = occluder + ((occluder4.z + (occluder4.y - occluder4.z) * fractional.y) - occluder)*fractional.x;    
#endif
        
    float occluderReceiverDistance = occluder - receiverDepth;
    const float darkeningFactor = shadowConstants.y;
    float occlusion = saturate(exp(darkeningFactor * occluderReceiverDistance));  
                 
    return occlusion;
}

//---------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- POINT ---------------------------------------------------------------------
/**
    Point-light shader.
    Point lights are rendered as backface-culled spheres.
*/
void
PointLightVertexShader(float4 position                      : POSITION,
                         out float4 oPos                    : POSITION,
                         out float3 oViewSpacePos           : TEXCOORD0)
{
    oPos = mul(position, mvp);
    oViewSpacePos = mul(position, modelView).xyz;
}    
 
//---------------------------------------------------------------------------------------------------------------------------
/**
*/
void
PointLightPixelShader(float2 screenPos    : VPOS,
                      float3 viewSpacePos : TEXCOORD0,
                      out float4 oColor   : COLOR,
                      uniform bool enableShadowMapping)
{      
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float3 viewVec = normalize(viewSpacePos);
    
    float4 normalPacked = tex2D(normalMapSampler, screenUv);    
    float3 viewSpaceNormal = UnpackViewSpaceNormal(normalPacked);
    
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float depth = UnpackDepth(objectDepthPacked);    
    float3 surfacePos = viewVec * depth;
        
    // compute attenuation and light vector 
    // (NOTE range is actually 1.0 / range)
    float3 lightDir = (lightPosRange.xyz - surfacePos);

    // compute attenuation, kill pixel if black    
    float att = saturate(1.0 - length(lightDir) * lightPosRange.w);
    att *= att;
    clip(att - 0.004);    // < 1.0 / 256.0
    lightDir = normalize(lightDir);
    
    // compute N.L diffuse component
    float NL = saturate(dot(lightDir, viewSpaceNormal));
    float3 diff = lightColor.xyz * NL * att;
    
    // compute specular component N.H^shinyness
    float3 H = normalize(lightDir - viewVec);
    float NH = saturate(dot(viewSpaceNormal, H));
    float diffLuminance = dot(diff, luminanceValue) * exaggerateSpec;
    float spec = pow(NH, specPower) * diffLuminance;
    
    // rgb holds the diffuse light color, which will be added to prevoius lighten pixels
    // alpha channel holds N dot H , which must not be added in alpha blend process, we use max to get the brightest specular of multiple lights
    oColor = float4(diff, spec);
    
    // shadow occlusion  
    float shadowFactor = 1.0f;    
    if (enableShadowMapping)
    {
        float4 shadowProjLightPos = mul(float4(surfacePos, 1.0f), shadowProjTransform);
        float2 shadowLookup = (shadowProjLightPos.xy / shadowProjLightPos.ww) * float2(0.5f, -0.5f) + 0.5f; 
        #if SHADOW_DEPTH_PROJSPACE // depth is in projection light space
            // compute pixel position in light space
            float receiverDepth = shadowProjLightPos.z / shadowProjLightPos.w;
        #else // depth is in world light space 
            float4 surfacePosInLightSpace = mul(float4(surfacePos, 1.0f), shadowTransform);
            float receiverDepth = length(surfacePosInLightSpace.xyz);
        #endif
        shadowFactor = GetInvertedOcclusion(receiverDepth,
                                            shadowLookup);
        // blend out shadow near light border
        shadowFactor = lerp(1.0f, shadowFactor, shadowIntensity);
    }               
    // encode hdr, scale diffuse light    
    oColor = EncodeHDR(oColor * shadowFactor);
}

//---------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- SPOT ----------------------------------------------------------------------
/**
    Spot-light shader.
    Spot lights are rendered as backface-culled pyramids.
*/
void
SpotLightVertexShader(float4 position          : POSITION,
                      out float4 oPos          : POSITION,
                      out float3 oViewSpacePos : TEXCOORD0)
{
    oPos = mul(position, mvp);       
    oViewSpacePos = mul(position, modelView).xyz;
}  
  
//---------------------------------------------------------------------------------------------------------------------------
/**
*/
float4
SpotLightPixelShader(float2 screenPos    : VPOS,
                     float3 viewSpacePos : TEXCOORD0,
                     uniform bool enableShadowMapping) : COLOR
{   
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float3 viewVec = normalize(viewSpacePos);
        
    float4 normalPacked = tex2D(normalMapSampler, screenUv);    
    float3 viewSpaceNormal = UnpackViewSpaceNormal(normalPacked);
    
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float depth = UnpackDepth(objectDepthPacked);    
    float3 surfacePos = viewVec * depth;
    
         
    // compute attenuation and light vector 
    // (NOTE range is actually 1.0 / range)
    float3 lightDir = (lightPosRange.xyz - surfacePos);
     
    // compute attenuation, kill pixel if black    
    float att = saturate(1.0 - length(lightDir) * lightPosRange.w);
    //att *= att; //use only linear attenuation
    clip(att - 0.004);    // < 1.0 / 256.0
    lightDir = normalize(lightDir);
    
    // compute pixel position in light space
    float4 projLightPos = mul(float4(surfacePos, 1.0f), lightProjTransform);
    clip(projLightPos.z - 0.001);
    float mipSelect = 0;// abs(surfacePos.z) / 250.0f;
    float4 lightSpaceUv = float4(((projLightPos.xy / projLightPos.ww) * float2(0.5f, -0.5f)) + 0.5f , 0, mipSelect);
    // modulate light by project map        
    float3 lightModColor = tex2Dlod(lightProjMapSampler, lightSpaceUv);
     
    // compute N.L diffuse component
    float NL = saturate(dot(lightDir, viewSpaceNormal));
    float3 diff = lightColor.xyz * NL * att;
    
    // compute specular component N.H^shinyness
    float3 H = normalize(lightDir - viewVec);
    float NH = saturate(dot(viewSpaceNormal, H));
    float diffLuminance = dot(diff, luminanceValue) * exaggerateSpec;
    float spec = pow(NH, specPower)* diffLuminance;
    
    // rgb holds the diffuse light color, which will be added to prevoius lighten pixels
    // alpha channel holds N dot H , which must not be added in alpha blend process, we use max to get the brightest specular of multiple lights
    float4 oColor = float4(lightModColor * diff, lightModColor.r * spec);
        
    // shadow occlusion  
    float shadowFactor = 1.0f;    
    if (enableShadowMapping)
    {
        float4 shadowProjLightPos = mul(float4(surfacePos, 1.0f), shadowProjTransform);
        float2 shadowLookup = (shadowProjLightPos.xy / shadowProjLightPos.ww) * float2(0.5f, -0.5f) + 0.5f; 
        #if SHADOW_DEPTH_PROJSPACE // depth is in projection light space
            float receiverDepth = projLightPos.z / projLightPos.w;
        #else // depth is in world light space 
            float4 surfacePosInLightSpace = mul(float4(surfacePos, 1.0f), shadowTransform);
            float receiverDepth = length(surfacePosInLightSpace.xyz);
        #endif
        shadowFactor = GetInvertedOcclusion(receiverDepth,
                                            shadowLookup);
        // blend out shadow near light border
        shadowFactor = lerp(1.0f, shadowFactor, shadowIntensity);
    }               
    // encode hdr, scale diffuse light    
    float4 encodedColor = EncodeHDR(oColor * shadowFactor);
    return encodedColor;
}

//------------------------------------------------ TECHNIQUES ---------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
/**
*/
technique GlobalLight <string Mask = "GlobalLight";>
{ 
    pass p0 
    { 
        ZWriteEnable     = false;
        ZEnable          = false;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = false;
        AlphaTestEnable  = false;
        CullMode         = none;
        StencilEnable    = false;   
        VertexShader     = compile CompileTargetVS GlobalLightVertexShader(); 
        PixelShader      = compile CompileTargetPS GlobalLightPixelShader();     
    } 
}

technique PointLight <string Mask = "PointLight";>
{ 
    pass p0 
    { 
        ZEnable          = false;
        ZFunc            = GREATER;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = true;
        DestBlend        = ONE;
        SrcBlend         = ONE;
        CullMode         = CCW;     
        VertexShader     = compile CompileTargetVS PointLightVertexShader(); 
        PixelShader      = compile CompileTargetPS PointLightPixelShader(false);     
    } 
}

technique PointLightShadows <string Mask = "PointLightShadows";>
{ 
    pass p0 
    { 
        ZEnable          = true;
        ZFunc            = GREATER;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = true;
        DestBlend        = ONE;
        SrcBlend         = ONE;       
        CullMode         = CCW;     
        VertexShader     = compile CompileTargetVS PointLightVertexShader(); 
        PixelShader      = compile CompileTargetPS PointLightPixelShader(true);     
    } 
}
    
technique SpotLight <string Mask = "SpotLight";>
{ 
    pass p0 
    { 
        ZEnable          = true;
        ZFunc            = GREATER;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = true;
        DestBlend        = ONE;
        SrcBlend         = ONE;
        CullMode         = CCW;     
        VertexShader     = compile CompileTargetVS SpotLightVertexShader(); 
        PixelShader      = compile CompileTargetPS SpotLightPixelShader(false);     
    } 
}

technique SpotLightShadows <string Mask = "SpotLightShadows";>
{ 
    pass p0 
    { 
        ZEnable          = true;
        ZFunc            = GREATER;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = true;
        DestBlend        = ONE;
        SrcBlend         = ONE;
        CullMode         = CCW;     
        VertexShader     = compile CompileTargetVS SpotLightVertexShader(); 
        PixelShader      = compile CompileTargetPS SpotLightPixelShader(true);     
    } 
}
//---------------------------------------------------------------------------------------------------------------------------
