#ifndef PARTICLES_H
#define PARTICLES_H
//------------------------------------------------------------------------------
//  particles.fxh
//  Helper functions for particle shaders.
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "util.fxh"
#include "deferred_shared.fxh"
float4   bboxCenter              : BBoxCenter;
float4   bboxSize                : BBoxSize;

int numAnimPhases           : AlphaRef;
float animFramesPerSecond   : Intensity1;
#define LifeTime0 0.4f

struct vsParticlesInput
{
    float2 corner     : TEXCOORD0;  // corner data (0,0) (1,0) (1,1) (0,1)
    float4 position   : POSITION0;  // Particle::position
    float4 stretchPos : POSITION1;  // Particle::stretchPosition
    float4 color      : COLOR0;     // Particle::color
    float4 uvMinMax   : TEXCOORD1;  // Particle::uvMinMax
    float4 rotSize    : TEXCOORD2;  // x: Particle::rotation, y: Particle::size, z: particle id, w: empty
};

struct vsParticlesOutput
{
    float4 position     : POSITION;
    float2 uv0          : TEXCOORD0;
    float4 worldPos     : TEXCOORD1;     // position in world space
    float4 projPos      : TEXCOORD2;     // position in projection space
    float3 normal       : TEXCOORD3;     // vertex normal in world space
    float3 tangent      : TEXCOORD4;     // vertex tangent in world space
    float3 binormal     : TEXCOORD5;     // vertex binormal in world space
    float  particleId   : TEXCOORD6;     // particle id for stipple particle rendering
    float3 viewSpacePos : TEXCOORD7;     // particle id for stipple particle rendering    
    float3 worldEyeVec  : COLOR0;        // normalized world space eye vector
    float4 color        : COLOR1;        // particle color
};

struct vsAdditiveParticlesOutput
{
    float4 position     : POSITION;
    float2 uv0          : TEXCOORD0;
    float4 worldPos     : TEXCOORD1;     // position in world space
    float4 projPos      : TEXCOORD2;     // position in projection space
    float3 normal       : TEXCOORD3;     // vertex normal in world space
    float3 tangent      : TEXCOORD4;     // vertex tangent in world space
    float3 binormal     : TEXCOORD5;     // vertex binormal in world space
    float3 viewSpacePos : TEXCOORD6;     // particle id for stipple particle rendering  
    float3 worldEyeVec  : COLOR0;        // normalized world space eye vector
    float4 color        : COLOR1;        // particle color
};

//------------------------------------------------------------------------------
/**
    ComputeCornerVertex
    Computes the expanded vertex position for the current corner. Takes
    rotation into account.
*/    
void
ComputeCornerVertex(uniform bool hemisphereNormal,
                    in float2 corner,
                    in float4 position,
                    in float4 stretchPos,
                    in float4 uvMinMax,
                    in float rotate,
                    in float size,
                    out float4 outWorldPos,
                    out float3 outWorldNormal,
                    out float3 outWorldTangent,
                    out float3 outWorldBinormal,
                    out float2 outUv)
{
    // build a 3x3 rotation matrix from the rotation
    float rotSin, rotCos;
    sincos(rotate, rotSin, rotCos);
    
    // compute 2d extruded corner position
    float2 extrude = ((corner * 2.0) - 1.0) * size;

    // rotate particle sprite
    float3x3 rotMatrix = {
        rotCos, -rotSin, 0.0,
        rotSin, rotCos,  0.0,
        0.0,    0.0,     1.0
    };
    extrude = mul(extrude, rotMatrix);
    
    // check if billboard 
    float4x4 transform;
    if (isBillboardOriented)
    {
        transform = invView;
    }
    else
    {
        transform = emitterTransform;
    }
    
    // transform to world space
    float4 worldExtrude = mul(float4(extrude, 0.0, 0.0), transform);
    
    // depending on corner, use position or stretchPos as center point   
    // also compute uv coordinates (v: texture tiling, x: anim phases)
    float du = frac(floor(time * animFramesPerSecond) / numAnimPhases);
    if (corner.x != 0)
    {
        outUv.x = (uvMinMax.z / numAnimPhases) + du;
    }
    else
    {
        outUv.x = (uvMinMax.x / numAnimPhases) + du;
    }
    if (corner.y != 0)
    {    
        outWorldPos = position + worldExtrude;    
        outUv.y = uvMinMax.w;
    }
    else
    {
        outWorldPos = stretchPos + worldExtrude;
        outUv.y = uvMinMax.y;
    }
    
    // setup normal, tangent, binormal in world space
    if (hemisphereNormal)
    {    
        outWorldNormal   = normalize(outWorldPos - bboxCenter.xyz);
        outWorldTangent  = cross(outWorldNormal, float3(0,1,0));
        outWorldBinormal = cross(outWorldNormal, outWorldTangent);
        outWorldTangent = cross(outWorldNormal, outWorldBinormal);
        
        outWorldNormal = mul(outWorldNormal, view).xyz;
        outWorldBinormal = mul(outWorldBinormal, view).xyz;
        outWorldTangent = mul(outWorldTangent, view).xyz;
    }
    else
    {
        float2 viewTangent  = mul(float2(-1.0, 0.0), rotMatrix);
        float2 viewBinormal = mul(float2(0.0, 1.0), rotMatrix);
        outWorldNormal   = transform[2].xyz;
        outWorldTangent  = mul(viewTangent, transform).xyz;
        outWorldBinormal = mul(viewBinormal, transform).xyz;
    }
}

//------------------------------------------------------------------------------
/**
    ParticleVertexShader
    Vertex shader for particles.
*/
vsParticlesOutput
ParticleVertexShader(const vsParticlesInput vsIn)
{
    vsParticlesOutput vsOut;
    float4 worldPos;
    ComputeCornerVertex(true,
                        vsIn.corner,
                        vsIn.position,
                        vsIn.stretchPos,
                        vsIn.uvMinMax,
                        vsIn.rotSize.x,
                        vsIn.rotSize.y,
                        worldPos,
                        vsOut.normal,
                        vsOut.tangent,
                        vsOut.binormal,
                        vsOut.uv0);    
    vsOut.worldEyeVec = normalize(eyePos - worldPos);
    vsOut.worldPos = worldPos;
    vsOut.position = mul(worldPos, viewProj);
    vsOut.viewSpacePos = mul(worldPos, view);
    vsOut.projPos  = vsOut.position;
    vsOut.color    = vsIn.color;
    vsOut.particleId = vsIn.rotSize.z;
    //vsOut.particleId = (length(vsIn.position) / length(bboxSize)) * 4.0f;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    ParticleVertexShader
    Vertex shader for particles.
*/
vsAdditiveParticlesOutput
ParticleAdditiveVertexShader(const vsParticlesInput vsIn)
{
    vsAdditiveParticlesOutput vsOut;
    float4 worldPos;
    ComputeCornerVertex(false,
                        vsIn.corner,
                        vsIn.position,
                        vsIn.stretchPos,
                        vsIn.uvMinMax,
                        vsIn.rotSize.x,
                        vsIn.rotSize.y,
                        worldPos,
                        vsOut.normal,
                        vsOut.tangent,
                        vsOut.binormal,
                        vsOut.uv0);
    vsOut.worldEyeVec = normalize(eyePos - worldPos);
    vsOut.worldPos = worldPos;
    vsOut.position = mul(worldPos, viewProj);
    vsOut.viewSpacePos = mul(worldPos, view);
    vsOut.projPos  = vsOut.position;
    vsOut.color    = vsIn.color;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    ParticleAlphaPixelShader
    Pixel shader for alpha-blended particles.
*/
float4
ParticleAlphaPixelShader(const vsParticlesOutput psIn, in float2 screenPos : VPOS) : COLOR
{
    float4 diffColor = tex2D(diffMapSampler, psIn.uv0); // * psIn.color;
    float4 black = float4(0.0, 0.0, 0.0, 0.0);
    
    //int index = fmod(psIn.color.a * numAlphaLayers, numAlphaLayers);
    int index = fmod(psIn.particleId, numAlphaLayers);
    float4 lightValues = SampleLightBufferAlpha(screenPos.xy, float2(ObjectId, psIn.particleId), index);

    // modulate diffuse texture with lightvalues from lightSampler and objects' emissive and specular maps    
    float4 color = psLightMaterial(lightValues,          // rt texture with lightvalues                            
                             diffColor,            // objects albedo diffuse texture color
                             black,            // objects emissive texture color
                             0,        // objects emissive intensity
                             black,            // objects specular texture color
                             0);        // objects specular intensity   
                             
    // soft particle: blend out near background surface  
    float2 screenUv = screenPos.xy * pixelSize.xy;
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float backGroundViewDepth = UnpackDepth(objectDepthPacked);    

    float particleViewDepth = length(psIn.viewSpacePos);
    float softAlphaMod = lerp(saturate((backGroundViewDepth - particleViewDepth) * 0.5f), 1.0f, abs(psIn.worldEyeVec.y));
    
    color = psFog(psIn.projPos.z, color);
    color.a = diffColor.a * softAlphaMod;    
    color = EncodeHDR(color);
    return color;
}

//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- ALPHA NORMAL DEPTH---------------------------------------------------

//------------------------------------------------------------------------------
/**
    ParticleVertexShader
    Vertex shader for particles.
*/
void
ParticleNormalDepthVertexShader(const vsParticlesInput vsIn,
                                out float4 oPos          : POSITION,
                                out float2 oUv0          : TEXCOORD0,
                                out float3 oTangent      : TEXCOORD1,
                                out float3 oNormal       : TEXCOORD2,
                                out float3 oBinormal     : TEXCOORD3,
                                out float4 oViewSpacePos : TEXCOORD4,
                                out float  oParticleId   : TEXCOORD5,
                                out float4 oColor        : COLOR0)     // particle color
{
    float4 worldPos;
    ComputeCornerVertex(true,
                        vsIn.corner,
                        vsIn.position,
                        vsIn.stretchPos,
                        vsIn.uvMinMax,
                        vsIn.rotSize.x,
                        vsIn.rotSize.y,
                        worldPos,
                        oNormal,
                        oTangent,
                        oBinormal,
                        oUv0);
    // push particle away from viewer on fadeout
    float3 worldEyeVec = normalize(eyePos - worldPos);
    // project on emitter-viewer plane
    float3 centerInView = mul(bboxCenter.xyz, view);
    oViewSpacePos = mul(worldPos, view);
    oViewSpacePos.z = centerInView.z;    
    oPos = mul(oViewSpacePos, proj);
    oColor = vsIn.color;
    oParticleId = vsIn.rotSize.z;
    //oParticleId = (length(vsIn.position) / length(bboxSize)) * 4.0f;
}

//------------------------------------------------------------------------------
void
ParticleNormalDepthPixelShader(float2 uv0             : TEXCOORD0,
                               float3 tangent         : TEXCOORD1,
                               float3 normal          : TEXCOORD2,
                               float3 binormal        : TEXCOORD3,
                               float3 viewSpacePos    : TEXCOORD4,
                               float  particleId      : TEXCOORD5,
                               float4 color           : COLOR0,
                               float2 screenPos       : VPOS,
                               out float4 oColor      : COLOR0,
                               out float4 oDSF        : COLOR1)
{    
    // the object id determines one of three stipple masks
    
    float alpha = tex2D(diffMapSampler, uv0.xy).a;
    //int index = fmod(color.a * numAlphaLayers, numAlphaLayers);
    int index = fmod(particleId, numAlphaLayers);
    float2 patternOffset = stippleMasks[index];
    float2 writePixel;
    writePixel.x = (fmod(screenPos.x + patternOffset.x, 2) * fmod(screenPos.y + patternOffset.y, 2)) - 0.5f;
    // alphatest    
    writePixel.y = alpha - 0.0001;
    clip(writePixel);
    
    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal));    

    // sample normal map
    float3 tNormal;
    tNormal.xy = (tex2D(bumpMapSampler, uv0).ag * 2.0) - 1.0;    
    tNormal.z = sqrt(1.0 - dot(tNormal.xy, tNormal.xy));
    
    // transform normal into view space
    float3 viewSpaceNormal = mul(tNormal, tangentViewMatrix);
    
    // write normal x and y as 16 bit components to oColor
    oColor = PackViewSpaceNormal(viewSpaceNormal);
    
    // x: objectId
    // y: NormalGroupId
    // z: depth upper 8 bit
    // w: depth lower 8 bit
    oDSF = PackObjectDepth(ObjectId, particleId, length(viewSpacePos));    
}

//------------------------------------------------------------------------------
/**
    ParticleAdditivePixelShader
    Pixel shader for additive-blended particles.
*/
float4
ParticleAdditivePixelShader(const vsAdditiveParticlesOutput psIn, in float2 screenPos : VPOS) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0) * psIn.color;
    // addtive particles will just fade out into fog, doesn't lerp to fogColor
    float fogIntensity = clamp((fogDistances.y - psIn.projPos.z) / (fogDistances.y - fogDistances.x), fogColor.a, 1.0);
    
    // soft particle: blend out near background surface  
    float2 screenUv = screenPos.xy * pixelSize.xy;
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float backGroundViewDepth = UnpackDepth(objectDepthPacked);    
    
    float particleViewDepth = length(psIn.viewSpacePos);
    //float softAlphaMod = saturate((backGroundViewDepth - particleViewDepth));
    float softAlphaMod = lerp(saturate((backGroundViewDepth - particleViewDepth) * 0.5f), 1.0f, abs(psIn.worldEyeVec.y));
    
    color *= fogIntensity * softAlphaMod;
    color = EncodeHDR(color);    
    return color;
}

//------------------------------------------------------------------------------
/**    
    Pixel shader for unlit alpha-blended particles.
*/
float4
ParticleUnlitPixelShader(const vsAdditiveParticlesOutput psIn, in float2 screenPos : VPOS) : COLOR
{
    float4 diffColor = tex2D(diffMapSampler, psIn.uv0) * psIn.color;
    float4 black = float4(0.0, 0.0, 0.0, 0.0);
                             
    // soft particle: blend out near background surface  
    float2 screenUv = screenPos.xy * pixelSize.xy;
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, screenUv);    
    float backGroundViewDepth = UnpackDepth(objectDepthPacked);    

    float particleViewDepth = length(psIn.viewSpacePos);
    //float softAlphaMod = saturate((backGroundViewDepth - particleViewDepth));
    float softAlphaMod = lerp(saturate((backGroundViewDepth - particleViewDepth) * 0.5f), 1.0f, abs(psIn.worldEyeVec.y));
    
    float4 color = psFog(psIn.projPos.z, diffColor);
    color.a = diffColor.a * softAlphaMod;    
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
#endif                                
                        
                        
                        
 
    
    
    
    
    

