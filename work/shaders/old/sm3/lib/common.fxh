#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------
//  common.fxh
//
//  Definitions and functions needed by all shaders.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
    Techniques: Cullmode and AlphaRef must be declared in calling shader,
                its value isn't set by every shadernodeinstance 
                (depends on nebula2 shadersettings in maya), so don't forget 
                to set it to a correct default values  

*///------------------------------------------------------------------------------
#define SimpleTechnique(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile vs_3_0 vertexShader(); \
        PixelShader     = compile ps_3_0 pixelShader(); \
        CullMode        = <CullMode>; \
        AlphaRef        = <AlphaRef>; \
    } \
}

#define SimpleTechniqueNoCulling(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile vs_3_0 vertexShader(); \
        PixelShader     = compile ps_3_0 pixelShader(); \
        CullMode        = None; \
        AlphaRef        = <AlphaRef>; \
    } \
}

#define LightTechnique(name, features, vertexShader, pixelShader, numLights) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile vs_3_0 vertexShader(); \
        PixelShader     = compile ps_3_0 pixelShader(numLights); \
        CullMode        = <CullMode>; \
        AlphaRef        = <AlphaRef>; \
    } \
}

#define SimpleTechniqueATest(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile vs_3_0 vertexShader(); \
        PixelShader     = compile ps_3_0 pixelShader(); \
        CullMode        = <CullMode>; \
        AlphaRef        = 120; \
    } \
}

//------------------------------------------------------------------------------
/**
    Shaderparameter: Nebula3 nodes set shader variables by semantic.
                     Shared shader variables must be declared in shared.fx shader,
                     these variables are directly set by the nebula3 transformdevice
                     
*///------------------------------------------------------------------------------
float4x4 mvp                : ModelViewProjection;
float4x4 imv                : InvModelView;
float4x4 model              : Model;

float MatSpecularPower      : MatSpecularPower = 32;
float BumpScale             : BumpScale = 0.0f;
float4 velocity             : Velocity = float4(0.0, 0.0, 0.0, 0.0); // UVAnimaton2D, Water movement

shared float4 halfPixelSize : HalfPixelSize;
shared float4x4 view        : View;
shared float4x4 invProj     : InvProjection;
shared float4 fogDistances  : FogDistances = {0.0, 10000.0, 0.0, 1.0};
shared float4 fogColor      : FogColor =     {0.5, 0.5, 0.5, 0.0};

float4 MLPUVStretch0        : MLPUVStretch0 = {1,1,1,1};         // stretch for layers 0,1,2,3
float4 MLPUVStretch1        : MLPUVStretch1 = {1,1,1,1};         // stretch for layers 4,5,6,7
float4 MLPSpecIntensity0    : MLPSpecIntensity0 = {1,1,1,1};     // like MLPUVStretch...
float4 MLPSpecIntensity1    : MLPSpecIntensity1 = {1,1,1,1};     // like MLPUVStretch...
float4x4 textureTransform0  : TextureTransform0 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // for uv animation

//------------------------------------------------------------------------------
/**
    Pixel shader part to compute texture coordinate shadow texture lookups.
*/
float2 psComputeScreenCoord(float4 pos)
{
    float2 screenCoord = ((pos.xy / pos.ww) * float2(0.5, -0.5)) + float2(0.5f, 0.5f);
    screenCoord += halfPixelSize.xy;
    return screenCoord;
}

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
/**
    structs for depth pass
*/
struct vsDepthPassOutput
{
    float4 position    : POSITION;
    float  depth       : TEXCOORD0;    
};

//------------------------------------------------------------------------------
/**
    structs for vsm depth pass
*/
struct vsVSMDepthPassOutput
{
    float4 position    : POSITION;
    float2 depth       : TEXCOORD0;    
};

//------------------------------------------------------------------------------
/**
    EncodeDepth()    
    
    Need nothing to do! (obsolete)
*/
float
EncodeDepth(float d)
{
    return d;
}

//------------------------------------------------------------------------------
/**
    DecodeDepth()
    
    Do nothing since we use a 32Bit - float buffer format (obsolete)
*/
float
DecodeDepth(float codedDepth)
{
    return codedDepth;
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for the depth pass.
    Passes depth to pixelshader.    
*/
vsDepthPassOutput
DepthPassVertexShader(float4 pos : POSITION)
{
    vsDepthPassOutput vsOut;
    vsOut.position = mul(pos, mvp);
    vsOut.depth    = vsOut.position.z / vsOut.position.w;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for the depth pass.
    Passes depth to pixelshader.    
*/
vsVSMDepthPassOutput
VSMDepthPassVertexShader(float4 pos : POSITION)
{
    vsVSMDepthPassOutput vsOut;
    vsOut.position = mul(pos, mvp);
    vsOut.depth.x  = vsOut.position.z;
    vsOut.depth.y  = mul(pos, model).z;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 DepthPassPixelShader(const vsDepthPassOutput psIn) : COLOR
{
    return float4(psIn.depth, 0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
float4 DummyPixelShader() : COLOR
{
    return 0;
}

//------------------------------------------------------------------------------
#endif

