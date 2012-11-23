#ifndef LIGHTING_FXH
#define LIGHTING_FXH
//------------------------------------------------------------------------------
//  lighting.fxh
//
//  Lib functions for lighting.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "util.fxh"
#include "lighting_shared.fxh"

const float LightShadowBias = 0.01;
const float LightVSMEpsilon = 0.0001;

const float HalfLambertScale = 0.5f;
const float HalfLambertBias  = 0.5f;

const float RimIntensity = 0.9;//3.0;//
const float RimPower = 2.0;
const float CharBackLightShift = 0.5;
const float SpecMapModulation = 1.0f;
const float LBRAmount = 0.18f;

#define DEBUGSHADER 0   // <--------- DISABLE, do not use on Geforce6/7 cause it causes lighting bugs or it could be a different fxc version problem
#if DEBUGSHADER  
shared int debugShaderLayer : DebugShaderLayer = 0;
#endif
// node specific variables
float MatEmissiveIntensity : MatEmissiveIntensity = 0.0f;
float MatSpecularIntensity : MatSpecularIntensity = 1.0f;

//------------------------------------------------------------------------------
/**
    From GPUGems3: Light bleeding reduction
*/
float LightBleedingReduction(float p)
{
    // Lots of options here if we don't care about being an upper bound.
    // Use whatever falloff function works well for your scene.
    return linstep(LBRAmount, 1, p);
    //return smoothstep(LBRAmount, 1, p);
}

//------------------------------------------------------------------------------
/**
    Compute shadowing term for global light sources.
*/
float
GlobalLightShadowTerm(in float4 worldPos,            // surface position in world space
                      in float4 projPos)             // surface position in projection space
{
    return 1.0;
/*
    float shadowContrib = 1.0;
    if (globalLightCastShadows)
    {
        float4x4 lightProjMatrix;
        float4 uvOffsetAndScale;
        
        // choose the right view frustum split
        if (projPos.z < globalLightPSSMDistances[1])
        {
            lightProjMatrix = globalLightPSSMTransforms[0];
            uvOffsetAndScale = float4(0.0f, 0.0f, 0.25f, 1.0f);
        }
        else if (projPos.z < globalLightPSSMDistances[2])
        {        
            lightProjMatrix = globalLightPSSMTransforms[1];
            uvOffsetAndScale = float4(0.25f, 0.0f, 0.25f, 1.0f);            
        }
        else if (projPos.z < globalLightPSSMDistances[3])
        {
            lightProjMatrix = globalLightPSSMTransforms[2];
            uvOffsetAndScale = float4(0.5f, 0.0f, 0.25f, 1.0f);
        }
        else
        {
            lightProjMatrix = globalLightPSSMTransforms[3];
            uvOffsetAndScale = float4(0.75f, 0.0f, 0.25f, 1.0f);
        }

        // lookup shadow map        
        float4 projLightPos = mul(worldPos, lightProjMatrix);
        projLightPos.xy /= projLightPos.w;
        if (projLightPos.z > 0.0)
        {
            // compute shadow term
            float2 lightSpaceUv = (projLightPos.xy * float2(0.5, -0.5)) + 0.5f;
            float2 shadowMapUv = lightSpaceUv * uvOffsetAndScale.zw + uvOffsetAndScale.xy;

            // lookup VSM moments from shadow map
            // simulate linear sampling
            float2 vsmMoments = DecodePSSMDepth(PSSMShadowBufferSampler, shadowMapUv, pixelSize.xy);
            float distToLight = projLightPos.z - LightShadowBias;
            shadowContrib = distToLight <= vsmMoments.x;

            float E_x2 = vsmMoments.y;
            float Ex_2 = vsmMoments.x * vsmMoments.x;
            float variance = min(max(E_x2 - Ex_2, 0.0) + LightVSMEpsilon, 1.0);
            float m_d = (vsmMoments.x - distToLight);
            float p = variance / (variance + m_d * m_d);                    
            shadowContrib = max(shadowContrib, p);
        }
    }
    return shadowContrib;
*/    
}

//------------------------------------------------------------------------------
/**
    Compute shadowing term for spot light sources.
*/
float
SpotLightShadowTerm(int lightIndex,         // light index
                    float4 worldPos,        // surface position in world space
                    float4 projLightPos,    // surface position in light projection space
                    float2 lightSpaceUv)    // projection map lookup uv coords (computed from projLightPos)
{   
    float shadowContrib = 1.0;             
    if (lightCastShadows[lightIndex])
    {
        float4 shadowUvOffset = lightShadowBufferUvOffset[lightIndex];
        float2 shadowMapUv = lightSpaceUv * shadowUvOffset.zw + shadowUvOffset.xy;

        // lookup VSM moments from shadow map
        // simulate linear sampling
        float2 vsmMoments = DecodePSSMDepth(LightShadowBufferSampler, shadowMapUv, pixelSize.xy);
        
        float distToLight = projLightPos.z - LightShadowBias;
        shadowContrib = distToLight <= vsmMoments.x;
        float E_x2 = vsmMoments.y;
        float Ex_2 = vsmMoments.x * vsmMoments.x;
        float variance = min(max(E_x2 - Ex_2, 0.0) + LightVSMEpsilon, 1.0);
        float m_d = (vsmMoments.x - distToLight);
        float p = variance / (variance + m_d * m_d);                    
        shadowContrib = max(shadowContrib, p);
        shadowContrib = LightBleedingReduction(shadowContrib);
    }
    return shadowContrib;  
}                       

//------------------------------------------------------------------------------
/**
    Compute "HalfLambert" diffuse intensity.
    NOT USED ANYMORE!
*/
float
DiffuseIntensityHalfLambert(float3 worldNormal,     // surface normal in world space
                            float3 lightDir,         // light direction in world space
                            float shadowTerm)        // shadow term
{
    float halfLambert = saturate(dot(worldNormal, lightDir) * HalfLambertScale + HalfLambertBias);
    halfLambert *= halfLambert;
    float diffIntensity = shadowTerm * halfLambert;
    return diffIntensity;
}

//------------------------------------------------------------------------------
/**
    Compute diffuse intensity without half lambert.
*/
float
DiffuseIntensity(float3 worldNormal,     // surface normal in world space
                 float3 lightDir,         // light direction in world space
                 float shadowTerm)        // shadow term
{
    float diffIntensity = shadowTerm * dot(worldNormal, lightDir);
    return diffIntensity;
}

//------------------------------------------------------------------------------
/**
    Compute Phong specular intensity.
*/
float
SpecularIntensityPhong(float3 worldNormal,      // surface normal in world space
                       float3 worldEyeVec,      // eye vector in world space
                       float3 lightDir,         // light direction in world space
                       float specularPower)     // material specular power
{
    float3 halfVec = normalize(lightDir.xyz + worldEyeVec);
    float specIntensity = pow(saturate(dot(worldNormal, halfVec)), specularPower);
    return specIntensity;
}

//------------------------------------------------------------------------------
/**
    Compute a rim light intensity value.
*/
float RimLightIntensity(float3 worldNormal,     // surface normal in world space
                        float3 worldEyeVec)     // eye vector in world space
{
    float rimIntensity  = pow(abs(1.0f - abs(dot(worldNormal, worldEyeVec))), RimPower);    
    rimIntensity *= RimIntensity;
    return rimIntensity;
}

//------------------------------------------------------------------------------
/**
    Evaluate a global light.
*/
void
GlobalLight(in float4 worldPos,            // surface position in world space
            in float4 projPos,             // surface position in projection space
            in float3 worldNormal,         // surface normal in world space
            in float3 worldEyeVec,         // normalized eye vector in world space
            in float matSpecularPower,     // material specular power
            in float4 matSpecular,
            in out float4 diffuse,         // computed diffuse component
            in out float4 specular)        // computed specular component
{
    float shadowTerm = GlobalLightShadowTerm(worldPos, projPos);
    float diffIntensity = DiffuseIntensity(worldNormal, globalLightDir, shadowTerm);
    float specIntensity = SpecularIntensityPhong(worldNormal, worldEyeVec, globalLightDir, matSpecularPower);
    // front diffuse and specular
    diffuse += saturate(diffIntensity) * globalLightColor;
    specular += globalLightColor * specIntensity * diffIntensity;
    // back diffuse and specular
    float backLightSpecular = SpecularIntensityPhong(worldNormal, worldEyeVec, -globalLightDir, matSpecularPower);
    diffuse += saturate(-diffIntensity + backLightOffset) * (globalBackLightColor + globalBackLightColor * backLightSpecular * matSpecular * MatSpecularIntensity);
}        

//------------------------------------------------------------------------------
/**
    Evaluate a global light with rim light effect.
*/
void
GlobalLightWithRim(in float4 worldPos,            // surface position in world space
                   in float4 projPos,             // surface position in projection space
                   in float3 worldNormal,         // surface normal in world space
                   in float3 worldEyeVec,         // normalized eye vector in world space
                   in float matSpecularPower,     // material specular power
                   in float4 matSpecular,
                   in float rimIntensity,        // rimIntensity computed before
                   in out float4 diffuse,         // computed diffuse component
                   in out float4 specular)        // computed specular component                   
{
    float shadowTerm = GlobalLightShadowTerm(worldPos, projPos);
    float diffIntensity = DiffuseIntensity(worldNormal, globalLightDir, shadowTerm);
    float specIntensity = SpecularIntensityPhong(worldNormal, worldEyeVec, globalLightDir, matSpecularPower);
    // front diffuse, specular and rim lighting
    diffuse += saturate(diffIntensity) * globalLightColor;    
    diffuse  += globalLightColor * saturate(diffIntensity) * rimIntensity;
    specular += globalLightColor * specIntensity * diffIntensity;
    // back diffuse and specular
    float backLightSpecular = SpecularIntensityPhong(worldNormal, worldEyeVec, -globalLightDir, matSpecularPower);   
    diffuse += saturate(-diffIntensity + CharBackLightShift) * (globalBackLightColor + globalBackLightColor * backLightSpecular * matSpecular * MatSpecularIntensity);    
}        

//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
SpotLight(in int lightIndex,            // light index
          in float4 worldPos,           // surface position in world space
          in float3 worldNormal,        // surface normal in world space
          in float3 worldEyeVec,        // normalized eye vector in world space
          in float matSpecularPower,    // material specular power
          in out float4 diffuse,        // computed diffuse component
          in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute distance attenuation
        float invDist = (1.0 - relDist);            
        float attenuate = invDist;// * invDist;

        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec);
        if (diffIntensity > 0.0f)
        {
            float4 projLightPos = mul(worldPos, lightProjTransform[lightIndex]);
            if (projLightPos.z > 0.0)
            {
                float2 lightSpaceUv = ((projLightPos.xy / projLightPos.w) * float2(0.5, -0.5)) + 0.5f;

                // modulate light by project map
                float4 lightModColor = lightColor[lightIndex] * tex2D(LightProjMapSampler, lightSpaceUv);
                if (any(lightModColor))
                {
                    // compute shadow term
                    float shadowContrib = SpotLightShadowTerm(lightIndex, worldPos, projLightPos, lightSpaceUv);
                    if (shadowContrib > 0)
                    { 
                        // specular intensity
                        float3 halfVec = normalize(lightVec + worldEyeVec);
                        float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;
                
                        // add diffuse and specular components
                        lightModColor *= shadowContrib;
                        
                        diffuse  += lightModColor * diffIntensity * attenuate;
                        specular += lightModColor * specIntensity;
                        
                    }
                }
            }
        }                
    }
}          

//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
SpotLightWithRim(in int lightIndex,            // light index
          in float4 worldPos,           // surface position in world space
          in float3 worldNormal,        // surface normal in world space
          in float3 worldEyeVec,        // normalized eye vector in world space
          in float matSpecularPower,    // material specular power
          in float rimIntensity,        // rimIntensity computed before
          in out float4 diffuse,        // computed diffuse component
          in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute distance attenuation
        float invDist = (1.0 - relDist);            
        float attenuate = invDist;// * invDist;

        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec);
        if (diffIntensity > 0.0f)
        {
            float4 projLightPos = mul(worldPos, lightProjTransform[lightIndex]);
            if (projLightPos.z > 0.0)
            {
                float2 lightSpaceUv = ((projLightPos.xy / projLightPos.w) * float2(0.5, -0.5)) + 0.5f;

                // modulate light by project map
                float4 lightModColor = lightColor[lightIndex] * tex2D(LightProjMapSampler, lightSpaceUv);
                if (any(lightModColor))
                {
                    // compute shadow term
                    float shadowContrib = 1.0f;//SpotLightShadowTerm(lightIndex, worldPos, projLightPos, lightSpaceUv);
                    if (shadowContrib > 0)
                    { 
                        // specular intensity
                        float3 halfVec = normalize(lightVec + worldEyeVec);
                        float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;
                
                        // add diffuse and specular components
                        lightModColor *= shadowContrib;
                        
                        diffuse  += lightModColor * diffIntensity * attenuate;
                        diffuse  += lightColor[lightIndex] * saturate(diffIntensity) * rimIntensity;
                        specular += lightModColor * specIntensity;                        
                    }
                }
            }
        }                
    }
}  

//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
PointLight(in int lightIndex,            // light index
           in float4 worldPos,           // surface position in world space
           in float3 worldNormal,        // surface normal in world space
           in float3 worldEyeVec,        // normalized eye vector in world space
           in float matSpecularPower,    // material specular power
           in out float4 diffuse,        // computed diffuse component
           in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute linear distance attenuation
        float attenuate = (1.0 - relDist);            
        
        // exp2 falloff?
        // float attenuate *= attenuate;
        
        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec) * attenuate;
        if (diffIntensity > 0.0f)
        {
            // specular intensity
            float3 halfVec = normalize(lightVec + worldEyeVec);
            float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;

            // add diffuse and specular components
            diffuse  += lightColor[lightIndex] * saturate(diffIntensity);
            specular += lightColor[lightIndex] * specIntensity;
        }
    }
}         

//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
PointLightWithRim(in int lightIndex,            // light index
           in float4 worldPos,           // surface position in world space
           in float3 worldNormal,        // surface normal in world space
           in float3 worldEyeVec,        // normalized eye vector in world space
           in float matSpecularPower,    // material specular power
           in float rimIntensity,        // rimIntensity computed before
           in out float4 diffuse,        // computed diffuse component
           in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute linear distance attenuation
        float attenuate = (1.0 - relDist);            
        
        // exp2 falloff?
        // float attenuate *= attenuate;
        
        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec) * attenuate;
        if (diffIntensity > 0.0f)
        {
            // specular intensity
            float3 halfVec = normalize(lightVec + worldEyeVec);
            float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;

            // add diffuse and specular components
            diffuse  += lightColor[lightIndex] * saturate(diffIntensity);
            diffuse  += lightColor[lightIndex] * saturate(diffIntensity) * rimIntensity;
            specular += lightColor[lightIndex] * specIntensity;
        }
    }
}  

//------------------------------------------------------------------------------
/**
    Pixel shader single-pass lighting function, performs lighting
    in world space.
*/
float4
psLight(int numLocalLights,           // number of local lights
        float4 worldPos,              // surface position in world space
        float4 projPos,               // surface position in projection space
        float3 worldNormal,           // surface normal in world space
        float3 worldEyeVec,           // normalized eye vector in world space
        float4 matDiffuse,            // material diffuse component
        float4 matSpecular,           // material specular component
        float matSpecularPower,       // material specular power
        float4 matEmissive)           // material emissive component
{
    float4 diffuse = globalAmbientLightColor;
    float4 specular = 0;
    matSpecular *= SpecMapModulation;
    // normalize normal
    worldNormal = normalize(worldNormal);
    // evaluate global light
    GlobalLight(worldPos, projPos, worldNormal, worldEyeVec, matSpecularPower, matSpecular, diffuse, specular);
    
    // evaluate local lights
    int lightIndex;
    for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
    {
        SpotLight(lightIndex, worldPos, worldNormal, worldEyeVec, matSpecularPower, diffuse, specular);
    }
    float4 color = matEmissive * MatEmissiveIntensity + (diffuse * matDiffuse) + (specular * matSpecular * MatSpecularIntensity);
    return color;
}

//------------------------------------------------------------------------------
/**
    Pixel shader single-pass lighting function for skinned characters, 
    adds a rim light effect at the character's silhouette
*/
float4
psLightRim(int numLocalLights,           // number of local lights
           float4 worldPos,              // surface position in world space
           float4 projPos,               // surface position in projection space
           float3 worldNormal,           // surface normal in world space
           float3 worldEyeVec,           // normalized eye vector in world space
           float4 matDiffuse,            // material diffuse component
           float4 matSpecular,           // material specular component
           float matSpecularPower,       // material specular power
           float4 matEmissive)           // material emissive component
{
    float4 diffuse = globalAmbientLightColor;
    float4 specular = 0;
    matSpecular *= SpecMapModulation;
    // normalize normal
    worldNormal = normalize(worldNormal);
    float rimIntensity = RimLightIntensity(worldNormal, worldEyeVec);    
    // exagerate Rim
    diffuse += float4(rimIntensity, rimIntensity, rimIntensity, 0);
    
    // evaluate global light
    GlobalLightWithRim(worldPos, projPos, worldNormal, worldEyeVec, matSpecularPower, matSpecular, rimIntensity, diffuse, specular);
    
    // evaluate local lights
    int lightIndex;
    for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
    {
        SpotLightWithRim(lightIndex, worldPos, worldNormal, worldEyeVec, matSpecularPower, rimIntensity, diffuse, specular);
    }    
    float4 color = (diffuse * matDiffuse) + (specular * matSpecular * MatSpecularIntensity);
	color.rgb += matEmissive.rgb * MatEmissiveIntensity;
    
#if DEBUGSHADER  
    if (debugShaderLayer == 1)
    {
        return diffuse;
    }
    else if (debugShaderLayer == 2)
    {
        return specular;
    }
    else if (debugShaderLayer == 3)
    {
        return matDiffuse;
    }    
    else if (debugShaderLayer == 4)
    {
        return matEmissive * MatEmissiveIntensity;
    }   
    else if (debugShaderLayer == 5)
    {
        return float4(rimIntensity, rimIntensity, rimIntensity, 1.0f);
    }
#endif
    return color;
}


//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
SpotLightNoShadows(in int lightIndex,            // light index
          in float4 worldPos,           // surface position in world space
          in float3 worldNormal,        // surface normal in world space
          in float3 worldEyeVec,        // normalized eye vector in world space
          in float matSpecularPower,    // material specular power
          in out float4 diffuse,        // computed diffuse component
          in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute distance attenuation
        float invDist = (1.0 - relDist);            
        float attenuate = invDist;// * invDist;

        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec);
        if (diffIntensity > 0.0f)
        {
            float4 projLightPos = mul(worldPos, lightProjTransform[lightIndex]);
            if (projLightPos.z > 0.0)
            {
                float2 lightSpaceUv = ((projLightPos.xy / projLightPos.w) * float2(0.5, -0.5)) + 0.5f;

                // modulate light by project map
                float4 lightModColor = lightColor[lightIndex] * tex2D(LightProjMapSampler, lightSpaceUv);
                if (any(lightModColor))
                {
                    // specular intensity
                    float3 halfVec = normalize(lightVec + worldEyeVec);
                    float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;
                                    
                    diffuse  += lightModColor * diffIntensity * attenuate;
                    specular += lightModColor * specIntensity;   
                }
            }
        }                
    }
}    

//------------------------------------------------------------------------------
/**
    Pixel shader single-pass lighting function, performs lighting
    in world space.
*/
float4
psLightNoShadows(int numLocalLights,           // number of local lights
        float4 worldPos,              // surface position in world space
        float4 projPos,               // surface position in projection space
        float3 worldNormal,           // surface normal in world space
        float3 worldEyeVec,           // normalized eye vector in world space
        float4 matDiffuse,            // material diffuse component
        float4 matSpecular,           // material specular component
        float matSpecularPower,       // material specular power
        float4 matEmissive)           // material emissive component
{
    float4 diffuse = globalAmbientLightColor;
    float4 specular = 0;
    matSpecular *= SpecMapModulation;
    // normalize normal
    worldNormal = normalize(worldNormal);
    // evaluate global light
    GlobalLight(worldPos, projPos, worldNormal, worldEyeVec, matSpecularPower, matSpecular, diffuse, specular);
    
    // evaluate local lights
    int lightIndex;
    for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
    {
        SpotLightNoShadows(lightIndex, worldPos, worldNormal, worldEyeVec, matSpecularPower, diffuse, specular);
    }
    float4 color = matEmissive * MatEmissiveIntensity + (diffuse * matDiffuse) + (specular * matSpecular * MatSpecularIntensity);
    return color;
}
//------------------------------------------------------------------------------
#endif