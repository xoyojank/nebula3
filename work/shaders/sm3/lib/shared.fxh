//------------------------------------------------------------------------------
//  shared.fxh
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------

#ifndef SHARED_H
#define SHARED_H

#include "posteffect.fxh"

shared float4x4 view            : View;
shared float4x4 invView         : InvView;
shared float4x4 viewProj        : ViewProjection;
shared float4x4 proj            : Projection;
shared float4x4 invProj         : InvProjection;
shared float4   eyePos          : EyePos;
shared float4   focalLength     : FocalLength;
shared float4x4 modelView       : ModelView;
shared float4x4 mvp             : ModelViewProjection;
shared float4x4 model           : Model;
shared float4x4 imv             : InvModelView;

shared float4   halfPixelSize           : HalfPixelSize;
shared float4   pixelSize               : PixelSize;
shared float    time                    : Time;
shared float4   fogDistances            : FogDistances      = {0.0, 10000.0, 0.0, 1.0};
shared float4   fogColor                : FogColor          = {0.5, 0.5, 0.5, 0.0};

shared texture  depthBuffer             : DepthBuffer;
shared texture  depthTexture            : DepthBuffer;
shared texture  jointMap                : JointTexture;
shared texture  lightTexture            : LightBuffer;
shared texture  LightBuffer             : LightBuffer;
shared texture  NormalBuffer            : NormalBuffer;
shared texture  normalMap               : NormalBuffer;
//shared texture  dsfObjectDepthSampler   : DSFObjectDepthBuffer;
shared texture  dsfObjectDepthMap       : DSFObjectDepthBuffer;
shared texture  alphaNormalDepthMap     : AlphaNormalDepthBuffer;
shared texture  alphaNormalDepthMapAlpha : AlphaNormalDepthBuffer;
shared texture  alphaDsfBuffer          : AlphaDSFObjectIdBuffer;
shared texture  alphaLightTexture       : AlphaLightBuffer;
shared texture  ssaoBuffer              : SSAOBuffer;
shared texture  shadowProjMap           : ShadowProjMap;
shared texture  lightProjMap            : LightProjMap;
shared texture  shadowBuffer            : LightShadowBuffer;
shared texture  guiBuffer               : GuiBuffer;
shared float    ObjectId                : ObjectId = 0.0f;
shared int      debugShaderLayer        : DebugShaderLayer = 0;
shared float4   occlusionConstants      : OcclusionConstants = {0.05f, 0.03f, 0.5f, 1.0f}; // radius, min, max, radius for wider gi fake
shared float4   shadowConstants         : ShadowConstants = {100.0f, 100.0f, 0.003f, 512.0f};// Depth Scale, Darkening factor, depth bias, shadowmap size
shared float    shadowIntensity         : ShadowIntensity = 1.0f;

// particle stuff
shared bool     isBillboardOriented     : Billboard                 = true;
shared float4x4 emitterTransform        : EmitterTransform;


//------------------------------------------------------------------------------
/**
    Compute fogging given a sampled fog intensity value from the depth
    pass and a fog color.
*/
float4 psFog(float fogDepth, float4 color)
{
    float fogIntensity = clamp((fogDistances.y - fogDepth) / (fogDistances.y - fogDistances.x), fogColor.a, 1.0);
    return float4(lerp(fogColor.rgb, color.rgb, fogIntensity), color.a);
}

//------------------------------------------------------------------------------
#endif // SHARED_H
