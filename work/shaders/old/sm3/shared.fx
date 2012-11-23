//------------------------------------------------------------------------------
//  shared.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting_shared.fxh"

shared float4x4 view            : View;
shared float4x4 invView         : InvView;
shared float4x4 viewProj        : ViewProjection;
shared float4x4 invProj         : InvProjection;

shared float    saturation      : Saturation;
shared float4   balance         : Balance;
shared float4   luminance       : Luminance;
shared float4   halfPixelSize   : HalfPixelSize;
shared float    time            : Time;
shared float4   fogDistances    : FogDistances      = {0.0, 200.0, 0.0, 200.0};
shared float4   fogColor        : FogColor          = {0.5, 0.5, 0.5, 0.0};
shared float    fadeValue       : FadeValue         = {1.0f};

shared int      debugShaderLayer    : DebugShaderLayer = 0;

// bloom stuff
shared float    hdrBrightPassThreshold  : HdrBrightPassThreshold    = {1.0};
shared float4   hdrBloomColor           : HdrBloomColor             = {1.0, 1.0, 1.0, 1.0};
shared float    hdrBloomScale           : HdrBloomScale             = {1.0};
shared float3   dofDistances            : DoFDistances;
shared texture  depthBuffer             : DepthBuffer;
shared texture  jointMap                : JointTexture;

// particle stuff
shared bool     isBillboardOriented     : Billboard                 = true;
shared float4x4 emitterTransform        : EmitterTransform;

technique t0
{
    pass p0 { }
}
