//------------------------------------------------------------------------------
//  pe_globalillumination.fx
//
//  A simple copy posteffect shader.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "defaultsampler.fxh"
#include "common.fxh"
#include "util.fxh"
#include "posteffect.fxh"

#define NUMSAMPLES 8
const float3 sampleOffset[NUMSAMPLES] = { { 0.4257627, 0.5885916, -0.1406845 }, 
                                          { 0.1438946, 0.6170444, -0.6800503 }, 
                                          { -0.1962208, 0.0321684, -0.6526062 }, 
                                          { 0.0548365, 0.2052362, 0.4552045 }, 
                                          { 0.4907259, 0.4657889, 0.4335600 }, 
                                          { 0.0797605, 0.4923897, -0.6303595 }, 
                                          { -0.7118144, 0.2453643, -0.4224561 }, 
                                          { -0.3940238, 0.1213742, -0.4599157 } 
                                         #if NUMSAMPLES > 8  
                                          ,{ -0.6552871, 0.5546126, 0.5125412 }, 
                                          { -0.4623650, 0.1636056, 0.1465077 }, 
                                          { 0.0124081, 0.0793601, 0.9560267 }, 
                                          { 0.5855279, 0.4831683, 0.4109746 }, 
                                          { 0.4503158, 0.4606389, 0.7313206 }, 
                                          { -0.5777390, 0.3028068, 0.4708703 }, 
                                          { -0.7598490, 0.4023147, 0.2708642 }, 
                                          { -0.1658553, 0.3391691, -0.7376472 }
                                        #endif
                                        #if NUMSAMPLES > 16                                            
                                          ,{ 0.1642508, 0.5621502, 0.7792481 },                                           
                                          { 0.2629237, 0.1867949, -0.4270888 }, 
                                          { -0.5364115, 0.5726916, -0.1000343 }, 
                                          { 0.1220901, 0.3730250, 0.6413980 }, 
                                          { -0.6432275, 0.5362196, 0.2635924 }, 
                                          { -0.4693550, 0.2068251, 0.0691762 }, 
                                          { -0.8720138, 0.0961522, -0.3915050 }, 
                                          { -0.5365040, 0.2375247, 0.2988052 }
                                        #endif
                                          };
                           
//----------------------------------------------- GLOBAL --------------------------------------------------------------------
/**
    Global-light shader.
    The global light is rendered as a fullscreen quad.
*/
void
vsMainGI(float4 position                   : POSITION,
         float2 uv0                        : TEXCOORD0,     
         out float4 oPos                   : POSITION,
         out float2 oUv0                   : TEXCOORD0,
         out float3 oViewSpacePos          : TEXCOORD1)
{
    oPos = position;
    oUv0 = uv0; 
    // ray thru frustum corners
    oViewSpacePos = float3(position.xy * focalLength.xy, -1.0f);
}

//------------------------------------------------------------------------------
/**
*/
float4 psMainGI(float2 screenPixel  : VPOS,
                float2 uv0          : TEXCOORD0, 
                float3 viewSpacePos : TEXCOORD1) : COLOR
{
    
    // reconstruct view space position
    float3 viewVec = viewSpacePos;
        
    // unpack depth
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, uv0);    
    float depth = UnpackDepth(objectDepthPacked);    
    float3 surfacePos = viewVec * depth;

    // unpack normal
    float4 normalPacked = tex2D(normalMapSampler, uv0);    
    float3 viewSpaceNormal = UnpackViewSpaceNormal(normalPacked);
          
    float3 normal = viewSpaceNormal;
    float3 binormal = normalize(cross(viewSpaceNormal, float3(1,0,0)));
    float3 tangent = normalize(cross(binormal, viewSpaceNormal));
    
    float sampleDepth;               
    const float minRange = occlusionConstants.y;
    const float maxRange = occlusionConstants.z; 
    //float2 uvTile = screenPixel / 4;
    //float3 randomVec = tex2D(randomTableSampler, uvTile);
        
    float occlusion = 0; 
    const float sphereRadiusOccl = occlusionConstants.x;
    const int numOcclSamples = NUMSAMPLES;
    for (int i = 0; i < numOcclSamples; i++)
    {
        float3 offsetVector = sampleOffset[i];
        // offset surfacepos inside sphere with normal base and random vector
        float3 samplePoint = surfacePos + sphereRadiusOccl * tangent * offsetVector.x                                         
                                        + sphereRadiusOccl * normal * abs(offsetVector.y) // hold sample above current surface
                                        + sphereRadiusOccl * binormal * offsetVector.z;

        // project into screenSpace
        float4 projSamplePoint = mul(float4(samplePoint, 1), proj);
        projSamplePoint.xy /= projSamplePoint.ww;
        projSamplePoint.xy = projSamplePoint.xy * float2(0.5, -0.5) + 0.5;
        // lookup depth in this pixel            
        float4 sampleDepthPacked = tex2D(dsfObjectDepthSampler, projSamplePoint.xy);
        sampleDepth = UnpackDepth(sampleDepthPacked);
        
        samplePoint.z = abs(samplePoint.z);
        float diff = samplePoint.z - sampleDepth;
        if (diff > minRange && diff < maxRange)
        {
            occlusion +=  (1 - smoothstep(minRange, maxRange, diff));
        }
    }
    occlusion /= numOcclSamples; 
    
    float globalIlluFake = 0;
    /* const float sphereRadiusVS = occlusionConstants.w;
    const int numGISamples = NUMSAMPLES - numOcclSamples;
    for (int i = numOcclSamples; i < numGISamples; i++)
    {
        float3 offsetVector = reflect(sampleOffset[i], randomVec);
        // offset surfacepos inside sphere with normal base and random vector
        float3 samplePoint = surfacePos + sphereRadiusVS * tangent * offsetVector.x                                         
                                        + sphereRadiusVS * normal * abs(offsetVector.y) // hold sample above current surface
                                        + sphereRadiusVS * binormal * offsetVector.z;

        // project into screenSpace
        float4 projSamplePoint = mul(float4(samplePoint, 1), proj);
        projSamplePoint.xy /= projSamplePoint.ww;
        projSamplePoint.xy = projSamplePoint.xy * float2(0.5, -0.5) + 0.5;
        // lookup depth in this pixel            
        float4 sampleDepthPacked = tex2D(dsfObjectDepthSampler, projSamplePoint.xy);
        sampleDepth = UnpackDepth(sampleDepthPacked);
        
        samplePoint.z = abs(samplePoint.z);
        float diff = samplePoint.z - sampleDepth;
        if (diff > minRange && diff < maxRange)
        {
            globalIlluFake +=  1 - (diff / (maxRange - minRange)) + minRange/maxRange;
        }
    }    
    globalIlluFake /= numGISamples;  */    
    
    float4 result = saturate(max(occlusion, globalIlluFake));        
    return result;
}

//------------------------------------------------------------------------------
technique t0
{
    pass p0
    {
        ZWriteEnable     = False;
        ZEnable          = False;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = False;
        AlphaTestEnable  = False;
        CullMode         = None;
        StencilEnable    = False;
        VertexShader     = compile vs_3_0 vsMainGI();
        PixelShader      = compile ps_3_0 psMainGI();
    }
}
