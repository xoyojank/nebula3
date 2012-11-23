//------------------------------------------------------------------------------
//  static.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "util.fxh"
#include "shared.fxh"

// sky stuff
float Contrast : Intensity0 = 1.0f;
float Brightness : Intensity1 = 0.0f;
float TextureBlendFactor : Intensity2 = 0.0f;
const float MipLodBias = -0.5;

//------------------------------------------------------------------------------
/**
    Sampler
*/
texture skyMap0 : DiffMap0;
sampler skySampler0 = sampler_state
{
    Texture = <skyMap0>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
    MipMapLodBias = <MipLodBias>;
};

texture skyMap1 : DiffMap1;
sampler skySampler1 = sampler_state
{
    Texture = <skyMap1>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
    MipMapLodBias = <MipLodBias>;
};

//------------------------------------------------------------------------------
/**
    In / out structs
*/

struct vsInputSkyboxColor
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

struct vsOutputSkyboxColor
{
    float4 position : POSITION;
    float3 uv0Fog   : TEXCOORD0;
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsOutputSkyboxColor
ColorPassVertexShader(const vsInputSkyboxColor vsIn)
{   
    vsOutputSkyboxColor vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    vsOut.uv0Fog.xy = UnpackUv(vsIn.uv0);
    float vFogNearPlane = 0.0;
    float vFogFarPlane = fogDistances.z; 
    vsOut.uv0Fog.z = clamp(1.0 - ((vFogFarPlane - vsIn.position.y) / (vFogFarPlane - vFogNearPlane)), fogColor.a, 1.0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader for sky box. Allows for 2 sky textures which are blended
    with an intensity factor:
    
    Intensity0:     contrast
    Intensity1:     brightness
    Intensity2:     Blend factor (0: fully use texture0)
*/
float4 
ColorPassPixelShader(const vsOutputSkyboxColor psIn) : COLOR
{
    float4 color0 = tex2D(skySampler0, psIn.uv0Fog.xy);
    float4 color1 = tex2D(skySampler1, psIn.uv0Fog.xy);
    float contrast = Contrast;
    float brightness = Brightness;
    
    // blend the 2 textures
    float4 color = lerp(color0, color1, TextureBlendFactor);
    
    // add contrast and brightness
    const float4 grey = float4(0.5, 0.5, 0.5, 0.0);
    color = ((color - grey) * contrast) + grey;
    color += brightness;
    
    // add fogging
    color = float4(lerp(fogColor.rgb, color.rgb, psIn.uv0Fog.z), color.a);    
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
    Skybox is unlit without need of writing depth!
*/
//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;
//------------------------------------------------------------------------------
SimpleTechnique(Solid0, "Solid", ColorPassVertexShader, ColorPassPixelShader);