#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------
//  common.fxh
//
//  Definitions and functions needed by all shaders.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

#define CompileTargetVS vs_3_0
#define CompileTargetPS ps_3_0
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
        VertexShader    = compile CompileTargetVS vertexShader(); \
        PixelShader     = compile CompileTargetPS pixelShader();\
        CullMode        = <CullMode>; \
    } \
}

#define SimpleTechniqueNoCulling(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile CompileTargetVS vertexShader(); \
        PixelShader     = compile CompileTargetPS pixelShader(); \
        CullMode        = None; \
    } \
}

#define AlphaTechnique(name, features, vertexShader, pixelShader, useLight) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader    = compile CompileTargetVS vertexShader(); \
        PixelShader     = compile CompileTargetPS pixelShader(useLight);\
        CullMode        = <CullMode>; \
    } \
}

//------------------------------------------------------------------------------
/**
    Shaderparameter: Nebula3 nodes set shader variables by semantic.
                     Shared shader variables must be declared in shared.fx shader,
                     these variables are directly set by the nebula3 transformdevice
                     
*///------------------------------------------------------------------------------
float MatEmissiveIntensity  : MatEmissiveIntensity = 0.0f;
float MatSpecularIntensity  : MatSpecularIntensity = 1.0f;
float MatSpecularPower      : MatSpecularPower = 32;
float BumpScale             : BumpScale = 0.0f;
float4 velocity             : Velocity = float4(0.0, 0.0, 0.0, 0.0); // UVAnimaton2D, Water movement

float4 MLPUVStretch0        : MLPUVStretch0 = {1,1,1,1};         // stretch for layers 0,1,2,3
float4 MLPUVStretch1        : MLPUVStretch1 = {1,1,1,1};         // stretch for layers 4,5,6,7
float4 MLPSpecIntensity0    : MLPSpecIntensity0 = {1,1,1,1};     // like MLPUVStretch...
float4 MLPSpecIntensity1    : MLPSpecIntensity1 = {1,1,1,1};     // like MLPUVStretch...
float4x4 textureTransform0  : TextureTransform0 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; // for uv animation

#define SHADOW_DEPTH_PROJSPACE 1
#define ENABLE_SCALEFORM 0
#define ENABLE_DEPTHOFFIELD 0

//------------------------------------------------------------------------------
#endif

