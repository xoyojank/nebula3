#ifndef UTIL_FXH
#define UTIL_FXH
//------------------------------------------------------------------------------
//  util.fxh
//
//  Shader utility functions.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "config.fxh"

//------------------------------------------------------------------------------
/**
    Sample a texel with manual box blur
*/
float4 
SampleSimpleBoxBlur(sampler texSampler, float2 uv, float2 pixelSize, float2 boxSize)
{
    float4 sum = 0;
    for(float x = -boxSize.x; x <= boxSize.x; x++)
    {
        for(float y = -boxSize.y; y <= boxSize.y; y++)
        {
            float2 boxUv = uv + float2(x * pixelSize.x, y * pixelSize.y);
            sum += tex2D(texSampler, boxUv);
        }
    }
    sum /= (boxSize.x * 2 + 1) * (boxSize.y * 2 + 1);
    return sum;
}

//------------------------------------------------------------------------------
/**
    Sample a texel with manual bilinear filtering (for texture formats which
    don't support filtering).
    
    FIXME: on 360 we can actually do the same without knowing texSize, saves
    the coordinate computations!
*/
float4
SampleBilinear(sampler texSampler, float2 uv, float2 pixelSize)
{
#if USE_HARDWARE_SAMPLING
    return tex2D(texSampler, uv);
#else    
    float2 uv01 = float2(uv.x + pixelSize.x, uv.y);
    float2 uv10 = float2(uv.x, uv.y + pixelSize.y);
    float2 uv11 = uv + pixelSize;
    
    float4 c00 = tex2D(texSampler, uv);
    float4 c01 = tex2D(texSampler, uv01);
    float4 c10 = tex2D(texSampler, uv10);
    float4 c11 = tex2D(texSampler, uv11);

    float2 ratios = frac(uv / pixelSize);    
    float4 c = lerp(lerp(c00, c01, ratios.x), lerp(c10, c11, ratios.x), ratios.y);
    return c;    
#endif    
}

//------------------------------------------------------------------------------
/**
    Encode 2 values in the range 0..1 into a 4-channel vector. Used
    for encoding PSSM-depth values into a 32-bit-rgba value.
*/
float4
Encode2(float2 inVals)
{
    return float4(inVals.x, frac(inVals.x * 256.0), inVals.y, frac(inVals.y * 256.0));
}

//------------------------------------------------------------------------------
/**
    Decode 2 values encoded by Encode2().
*/
float2
Decode2(float4 inVals)
{
    return float2(inVals.x + (inVals.y / 256.0), inVals.z + (inVals.w / 256.0));
}

//------------------------------------------------------------------------------
/**
    Encode PSSM depth values from the projection space depth (0..1)
*/
float4
EncodePSSMDepth(float depth)
{
    //return float4(depth, depth*depth, 0.0, 0.0);
    
    // partial derivates of depth
    float dx = ddx(depth);
    float dy = ddy(depth);
    
    // compute second moment over the pixel extents
    float momentY = depth * depth + 0.25 * (dx * dx + dy * dy);
    return float4(depth, momentY, 0.0, 0.0);
}

//------------------------------------------------------------------------------
/**
    Decode PSSM depth values from a shadow map texture.
*/
float2
DecodePSSMDepth(sampler texSampler, float2 uv, float2 pixelSize)
{
    return SampleBilinear(texSampler, uv, pixelSize).xy;
    //float2 boxSize = (1.0, 1.0);
    //return SampleSimpleBoxBlur(texSampler, uv, texSize, boxSize);
}

//------------------------------------------------------------------------------
/**
    Sample a surface normal from a DXT5nm-compressed texture, return 
    TangentSurfaceNormal.
*/
float3
SampleNormal(sampler bumpSampler, float2 uv)
{
    float3 n;
    #ifdef __XBOX360__
        // DXN compression
        n.xy = (tex2D(bumpSampler, uv).xy * 2.0) - 1.0;
        n.z = saturate(1.0 - dot(n.xy, n.xy));
    #else
        // DX5N compression
        n.xy = (tex2D(bumpSampler, uv).ag * 2.0) - 1.0;    
        n.z = saturate(1.0 - dot(n.xy, n.xy));
    #endif
    return n;
}

//------------------------------------------------------------------------------
/**
    Sample tangent surface normal from bump map and transform into 
    world space.
*/
float3
psWorldSpaceNormalFromBumpMap(sampler bumpMapSampler, float2 uv, float3 worldNormal, float3 worldTangent, float3 worldBinormal)
{
    float3x3 tangentToWorld = float3x3(worldTangent, worldBinormal, worldNormal);
    float3 tangentNormal = SampleNormal(bumpMapSampler, uv);
    return mul(tangentNormal, tangentToWorld);
}

//------------------------------------------------------------------------------
/**
    Unpack a UB4N packed normal.
*/
float3
UnpackNormal(float3 packedNormal)
{
    return (packedNormal * 2.0) - 1.0;
}

//------------------------------------------------------------------------------
/**
    Unpack a 4.12 packed texture coord.
*/
float2
UnpackUv(float2 packedUv)
{
    return (packedUv / 8192.0);
}   

//------------------------------------------------------------------------------
/**
    Unpack a skin weight vertex component. Since the packing looses some
    precision we need to re-normalize the weights.
*/
float4
UnpackWeights(float4 weights)
{
    return (weights / dot(weights, float4(1.0, 1.0, 1.0, 1.0)));
}

//------------------------------------------------------------------------------
/**
    Scale down pseudo-HDR-value into RGB8.
*/
float4
EncodeHDR(in float4 rgba)
{
    return rgba * float4(0.5, 0.5, 0.5, 1.0);
}

/*
float4 EncodeHDR(in float4 rgba)
{
    const float colorSpace = 0.8;
    const float maxHDR = 8;
    const float hdrSpace = 1 - colorSpace;
    const float hdrPow = 10;
    const float hdrRt = 0.1;
    
    float3 col = clamp(rgba.rgb,0,1) * colorSpace;
    float3 hdr = pow(clamp(rgba.rgb,1,10),hdrRt)-1;
    float4 result;
    hdr = clamp(hdr, 0, hdrSpace);
    result.rgb = col + hdr;
    result.a = rgba.a;
    return result;
}
*/

//------------------------------------------------------------------------------
/**
    Scale up pseudo-HDR-value encoded by EncodeHDR() in shaders.fx.
*/   
float4 
DecodeHDR(in float4 rgba)
{
    return rgba * float4(2.0f, 2.0f, 2.0f, 1.0f);
}   

/*
float4 DecodeHDR(in float4 rgba)
{
    const float colorSpace = 0.8;
    const float maxHDR = 8;
    const float hdrSpace = 1 - colorSpace;
    //const float hdrPow = log(maxHDR)/log(hdrSpace);
    const float hdrPow = 10;
    //const float hdrRt = 1/hdrPow;
    const float hdrRt = 0.1;
        
    float3 col = clamp(rgba.rgb,0,colorSpace) * (1/colorSpace);
    float3 hdr = pow(clamp(rgba.rgb - colorSpace, 0, hdrSpace)+1,hdrPow)-1;
    float4 result;
    result.rgb = col + hdr;
    result.a = rgba.a;
    return result;
}
*/

// From GPUGems3: "Don't know why this isn't in the standard library..."
float linstep(float min, float max, float v)
{
    return clamp((v - min) / (max - min), 0, 1);
}
//------------------------------------------------------------------------------
#endif
