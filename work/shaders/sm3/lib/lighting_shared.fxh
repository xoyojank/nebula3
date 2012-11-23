#ifndef LIGHTINGSHARED_FXH
#define LIGHTINGSHARED_FXH
//------------------------------------------------------------------------------
//  lighting_shared.fxh
//
//  Shared variables for lighting.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#define MaxNumLocalLights 4
#define PSSMNumSplits 4

shared float4 eyePos : EyePos;

shared texture lightProjMap : LightProjMap;
sampler lightProjMapSampler  = sampler_state 
{
    Texture     = <lightProjMap>;
    AddressU    = Clamp;
    AddressV    = Clamp;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
};

#define LightShadowBufferSampler shadowBufferSampler
shared texture shadowBuffer : LightShadowBuffer;
sampler shadowBufferSampler = sampler_state
{
    Texture = <shadowBuffer>;
    AddressU  = Clamp;
    AddressV  = Clamp;
#if USE_HARDWARE_SAMPLING    
    MinFilter = Linear;
    MagFilter = Linear;
#else    
    MinFilter = Point;
    MagFilter = Point;
#endif
    MipFilter = Point;
};

#define PSSMShadowBufferSampler pssmShadowBufferSampler
shared texture pssmShadowBuffer : GlobalLightPSSMShadowBuffer;
sampler pssmShadowBufferSampler = sampler_state
{
    Texture = <pssmShadowBuffer>;
    AddressU = Clamp;
    AddressV = Clamp;
#if USE_HARDWARE_SAMPLING    
    MinFilter = Linear;
    MagFilter = Linear;
#else    
    MinFilter = Point;
    MagFilter = Point;
#endif
    MipFilter = Point;
};

shared float4   globalLightDir                              : GlobalLightDirection;         // direction of global light source
shared float4   globalLightColor                            : GlobalLightColor;             // global light color
shared float4   globalAmbientLightColor                     : GlobalAmbientLightColor;      // global ambient light color
shared float4   globalBackLightColor                        : GlobalBackLightColor;         // backlight color of global light source
shared float    backLightOffset                             : GlobalBackLightOffset;        // backlight offset, for moving backlight range
shared bool     globalLightCastShadows                      : GlobalLightCastShadows;       // global light shadow casting flag
shared float    globalLightPSSMDistances[PSSMNumSplits + 1] : GlobalLightPSSMDistances;     // PSSM split distances
shared float4x4 globalLightPSSMTransforms[PSSMNumSplits]    : GlobalLightPSSMTransforms;    // PSSM light projection transforms

shared float4   lightShadowBufferSize                       : LightShadowBufferSize;        // width and height of the shadow buffer
shared float4   lightPos[MaxNumLocalLights]                 : LightPosition;                // world space light position
shared float    lightInvRange[MaxNumLocalLights]            : LightInvRange;                // inverse light range
shared float4x4 lightProjTransform[MaxNumLocalLights]       : LightProjTransform;           // transform from world to light projection space
shared float4   lightColor[MaxNumLocalLights]               : LightColor;                   // light colors
shared float4   lightProjMapUvOffset[MaxNumLocalLights]     : LightProjMapUvOffset;         // uv-rect of light in projection map
shared int      lightType[MaxNumLocalLights]                : LightType;                    // light types
shared bool     lightCastShadows[MaxNumLocalLights]         : LightCastShadows;             // true if shadow casting light source
shared float4   lightShadowBufferUvOffset[MaxNumLocalLights]: LightShadowBufferUvOffset;    // per-light offset rectangle into shadow buffer

//------------------------------------------------------------------------------
#endif
