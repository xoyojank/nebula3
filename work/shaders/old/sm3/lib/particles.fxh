#ifndef PARTICLES_H
#define PARTICLES_H
//------------------------------------------------------------------------------
//  particles.fxh
//  Helper functions for particle shaders.
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
shared float4x4 invView     : InvView;
shared float4x4 viewProj    : ViewProjection;
shared float time           : Time;

shared bool     isBillboardOriented     : Billboard = true;
shared float4x4 emitterTransform        : EmitterTransform;

int numAnimPhases           : AlphaRef;
float animFramesPerSecond   : Intensity1;

struct vsParticlesInput
{
    float2 corner     : TEXCOORD0;  // corner data (0,0) (1,0) (1,1) (0,1)
    float4 position   : POSITION0;  // Particle::position
    float4 stretchPos : POSITION1;  // Particle::stretchPosition
    float4 color      : COLOR0;     // Particle::color
    float4 uvMinMax   : TEXCOORD1;  // Particle::uvMinMax
    float4 rotSize    : TEXCOORD2;  // x: Particle::rotation, y: Particle::size, zw: empty
};

struct vsParticlesOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;
    float4 worldPos    : TEXCOORD1;     // position in world space
    float4 projPos     : TEXCOORD2;     // position in projection space
    float3 normal      : TEXCOORD3;     // vertex normal in world space
    float3 tangent     : TEXCOORD4;     // vertex tangent in world space
    float3 binormal    : TEXCOORD5;     // vertex binormal in world space
    float3 worldEyeVec : COLOR0;        // normalized world space eye vector
    float4 color       : COLOR1;        // particle color
};

//------------------------------------------------------------------------------
/**
    ComputeCornerVertex
    Computes the expanded vertex position for the current corner. Takes
    rotation into account.
*/    
void
ComputeCornerVertex(in float2 corner,
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
    if(isBillboardOriented)
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
    float2 viewTangent  = mul(float2(-1.0, 0.0), rotMatrix);
    float2 viewBinormal = mul(float2(0.0, 1.0), rotMatrix);
    outWorldNormal   = transform[2].xyz;
    outWorldTangent  = mul(viewTangent, transform).xyz;
    outWorldBinormal = mul(viewBinormal, transform).xyz;
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
    ComputeCornerVertex(vsIn.corner,
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
ParticleAlphaPixelShader(const vsParticlesOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0) * psIn.color;
    float4 black = float4(0.0, 0.0, 0.0, 0.0);
    float4 color = psLightNoShadows(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, black, 1.0f, matDiffuse);
    color = psFog(psIn.projPos.z, color);
    // premultiplied alpha
    color.a = matDiffuse.a;
    color.rgb *= color.a;
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
/**
    ParticleAdditivePixelShader
    Pixel shader for additive-blended particles.
*/
float4
ParticleAdditivePixelShader(const vsParticlesOutput psIn, uniform int numLights) : COLOR
{
    float4 color = tex2D(diffMapSampler, psIn.uv0) * psIn.color;
    // addtive particles will be just fade out into fog, doesn't lerp to fogColor
    float fogIntensity = clamp((fogDistances.y - psIn.projPos.z) / (fogDistances.y - fogDistances.x), fogColor.a, 1.0);
    color *= fogIntensity;
    color = EncodeHDR(color);
    return color;
}

//------------------------------------------------------------------------------
#endif                                
                        
                        
                        
 
    
    
    
    
    

