//------------------------------------------------------------------------------
//  lightmapped.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "util.fxh"
#include "shadowmapping.fxh"

float brightness : Intensity0;

texture lightMapTex : DiffMap1;
sampler lightmapSampler = sampler_state
{
    Texture     = <lightMapTex>;
    AddressU    = Clamp;
    AddressV    = Clamp;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Point;
    MipMapLodBias = <MipLodBias>;
};

texture diffMap0 : DiffMap0;
sampler Layer0123WeightSampler = sampler_state
{
    Texture = <diffMap0>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;    
    MipMapLodBias = <MipLodBias>;
};

texture diffMap3 : DiffMap3;
sampler Layer4567WeightSampler = sampler_state
{
    Texture = <diffMap3>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;    
    MipMapLodBias = <MipLodBias>;
};

texture noiseMap0 : NoiseMap0;
sampler Layer0ColorSampler = sampler_state
{
    Texture = <noiseMap0>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;  
    MipMapLodBias = <MipLodBias>;  
};

texture noiseMap1 : NoiseMap1;
sampler Layer1ColorSampler = sampler_state
{
    Texture = <noiseMap1>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;  
    MipMapLodBias = <MipLodBias>;  
};

texture noiseMap2 : NoiseMap2;
sampler Layer2ColorSampler = sampler_state
{
    Texture = <noiseMap2>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;  
    MipMapLodBias = <MipLodBias>;  
};

texture noiseMap3 : NoiseMap3;
sampler Layer3ColorSampler = sampler_state
{
    Texture = <noiseMap3>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;   
    MipMapLodBias = <MipLodBias>; 
};

texture specMap0 : SpecMap0;
sampler Layer4ColorSampler = sampler_state
{
    Texture = <specMap0>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear; 
    MipMapLodBias = <MipLodBias>;   
};

texture specMap1 : SpecMap1;
sampler Layer5ColorSampler = sampler_state
{
    Texture = <specMap1>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear; 
    MipMapLodBias = <MipLodBias>;   
};

texture specMap2 : SpecMap2;
sampler Layer6ColorSampler = sampler_state
{
    Texture = <specMap2>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;  
    MipMapLodBias = <MipLodBias>;  
};

texture specMap3 : SpecMap3;
sampler Layer7ColorSampler = sampler_state
{
    Texture = <specMap3>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear; 
    MipMapLodBias = <MipLodBias>;   
};

texture bumpMap0 : BumpMap0;
sampler Layer0BumpSampler = sampler_state
{
    Texture = <bumpMap0>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;  
    MipMapLodBias = <MipLodBias>;  
};

texture bumpMap1 : BumpMap1;
sampler Layer1BumpSampler = sampler_state
{
    Texture = <bumpMap1>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;  
    MipMapLodBias = <MipLodBias>;  
};

texture bumpMap2 : BumpMap2;
sampler Layer2BumpSampler = sampler_state
{
    Texture = <bumpMap2>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;   
    MipMapLodBias = <MipLodBias>; 
};

texture bumpMap3 : BumpMap3;
sampler Layer3BumpSampler = sampler_state
{
    Texture = <bumpMap3>;
    AddressU  = Wrap;
    AddressV  = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;  
    MipMapLodBias = <MipLodBias>;  
};

/* shared texture lightTexture : LightBuffer;
sampler lightSampler = sampler_state
{
    Texture = <lightTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
    MipMapLodBias = <MipLodBias>;
}; */

//---------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------NORMAL DEPTH----------------------------------------------------------
/**
    The NormalDepth pass, this fills the color buffer in xy with
    2 surface normal components, and zw with the depth value from 0 to 1 (0 is
    near plane, 1 is far plane) encoded in 16 bit.
*/
void
NormalDepthMultilayerPassVertexShader(float4 position         : POSITION,
                                      float4 normal           : NORMAL,
                                      float2 uv0              : TEXCOORD0,
                                      float2 uv1              : TEXCOORD1,
                                      float4 tangent          : TANGENT,
                                      float4 binormal         : BINORMAL,
                                      out float4 oPos         : POSITION,
                                      out float4 oUv01        : TEXCOORD0,
                                      out float3 oTangent     : TEXCOORD1,
                                      out float3 oNormal      : TEXCOORD2,
                                      out float3 oBinormal    : TEXCOORD3,
                                      out float3 viewSpacePos : TEXCOORD4)
{
    oPos = mul(position, mvp);
    oUv01.xy = UnpackUv(uv0).xy;
    oUv01.zw = UnpackUv(uv1).xy;
    viewSpacePos = mul(position, modelView).xyz;
    
    // setup matrix components to transform tangent space normals 
    // into view space
    oTangent  = mul(imv, UnpackNormal4(tangent)).xyz;
    oNormal   = mul(imv, UnpackNormal4(normal)).xyz;
    oBinormal = mul(imv, UnpackNormal4(binormal)).xyz;
} 

//------------------------------------------------------------------------------
/**
    Normal map sampler for multilayered DXT5 compressed normal map.
*/
float3
psMultiLayerSampleNormal(float4 uvLayer01, float4 uvLayer23, float4 layer0123Weights, float layer234567WeightSum)
{
    float3 normal;
    float3 flatNormal = float3(0.0f, 0.0f, 1.0f);
    normal.xy = (lerp(tex2D(Layer0BumpSampler, uvLayer01.xy).ag, tex2D(Layer1BumpSampler, uvLayer01.zw).ag, layer0123Weights.y) * 2.0) - 1.0;    
    normal.z  = sqrt(1.0 - dot(normal.xy, normal.xy));
    normal = lerp(normal, flatNormal, layer234567WeightSum); 
    return normal;
}  

//---------------------------------------------------------------------------------------------------------------------
void
NormalDepthMultilayerPassPixelShader(float4 uv01            : TEXCOORD0,
                                     float3 tangent         : TEXCOORD1,
                                     float3 normal          : TEXCOORD2,
                                     float3 binormal        : TEXCOORD3,
                                     float3 viewSpacePos    : TEXCOORD4,
                                     out float4 oColor      : COLOR)
{
    // compute the tile UVs for all 8 layers
    float4 uvLayer01 = uv01.xyxy * MLPUVStretch0.xxyy;
    float4 uvLayer23 = uv01.xyxy * MLPUVStretch0.zzww;
    
    // read weights
    float4 layer0123Weights = tex2D(Layer0123WeightSampler, uv01.xy);
    float4 layer4567Weights = tex2D(Layer4567WeightSampler, uv01.xy);   
    float layer234567WeightSum = saturate(dot(layer4567Weights, float4(1.0f, 1.0f, 1.0f, 1.0f)) + dot(layer0123Weights.zw, float2(1.0f, 1.0f)));
    
    float3 combinedNormal = psMultiLayerSampleNormal(uvLayer01, uvLayer23, layer0123Weights, layer234567WeightSum);
          
    // transform normal into view space
    float3x3 tangentViewMatrix = float3x3(normalize(tangent), normalize(binormal), normalize(normal)); 
    float3 viewSpaceNormal = mul(combinedNormal, tangentViewMatrix);

    // write output color:
    //  xy: view space normal components    
    //  zw: 15 bit encoded depth, 1 bit z-sign
    oColor = PackViewSpaceNormal(viewSpaceNormal);
}

//------------------------------------------------------------------------------
/**
*/
void psMultilayerColorSpecIntensity(in float4 uv01, out float4 combinedColor, out float4 combinedSpecColor)
{
    // compute the tile UVs for all 8 layers
    float4 uvLayer01 = uv01.xyxy * MLPUVStretch0.xxyy;
    float4 uvLayer23 = uv01.xyxy * MLPUVStretch0.zzww;
    float4 uvLayer45 = uv01.xyxy * MLPUVStretch1.xxyy;
    
    // read weights
    float4 layer0123Weights = tex2D(Layer0123WeightSampler, uv01.xy);
    float4 layer4567Weights = tex2D(Layer4567WeightSampler, uv01.xy);    
    
    // read tile colors
    float4 layerColor[6];
    layerColor[0] = tex2D(Layer0ColorSampler, uvLayer01.xy);
    layerColor[1] = tex2D(Layer1ColorSampler, uvLayer01.zw);
    layerColor[2] = tex2D(Layer2ColorSampler, uvLayer23.xy);
    layerColor[3] = tex2D(Layer3ColorSampler, uvLayer23.zw);
    layerColor[4] = tex2D(Layer4ColorSampler, uvLayer45.xy);
    layerColor[5] = tex2D(Layer5ColorSampler, uvLayer45.zw);
        
    // compute resulting weights (weight * tile alpha)
    float4 weight0123 = float4(layerColor[0].w, layerColor[1].w, layerColor[2].w, layerColor[3].w) * layer0123Weights;
    float4 weight4567 = float4(layerColor[4].w, layerColor[5].w, 0.0, 0.0) * layer4567Weights;
            
    // get combined tile color
    combinedColor = lerp(layerColor[0], layerColor[1], weight0123.y);
    combinedColor = lerp(combinedColor, layerColor[2], weight0123.z);
    combinedColor = lerp(combinedColor, layerColor[3], weight0123.w);
    combinedColor = lerp(combinedColor, layerColor[4], weight4567.x);
    combinedColor = lerp(combinedColor, layerColor[5], weight4567.y);
    
    // FIXME: compute combined specular intensity!   
    float layer234567WeightSum = saturate(dot(layer4567Weights, float4(1.0f, 1.0f, 1.0f, 1.0f)) + dot(layer0123Weights.zw, float2(1.0f, 1.0f)));
    float combinedSpecIntensity = saturate(lerp(MLPSpecIntensity0.x, MLPSpecIntensity0.y, weight0123.y) - layer234567WeightSum);
    combinedSpecColor = float4(combinedSpecIntensity, combinedSpecIntensity, combinedSpecIntensity,1.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Deferred Lighting //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------- COLOR ---------------------------------------------------------------
/**
    The opaque color pass.    
*/
void
ColorVertexShader(float4 position     : POSITION,
                  float2 uv0          : TEXCOORD0,                    
                  float2 uv1          : TEXCOORD1,
                  out float4 oPos     : POSITION,
                  out float4 oProjPos : TEXCOORD0,
                  out float4 oUv01    : TEXCOORD1)
{
    oPos = mul(position, mvp);
    oProjPos = oPos;
    oUv01.xy = UnpackUv(uv0).xy;  
    oUv01.zw = UnpackUv(uv1).xy;
}     

//---------------------------------------------------------------------------------------------------------------------
void
ColorPixelShader(float4 projPos           : TEXCOORD0,
                 float4 uv01              : TEXCOORD1,
                 float2 screenPos         : VPOS,  
                 out float4 oColor        : COLOR)
{     
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
    
    float4 combinedDiffColor, combinedSpecularColor;
    psMultilayerColorSpecIntensity(uv01, combinedDiffColor, combinedSpecularColor);  
    // modulate by lightmapcolor
    float4 lightmapColor = tex2D(lightmapSampler, uv01.zw);
    combinedDiffColor = combinedDiffColor * lightmapColor * brightness;  

    // modulate diffuse texture with lightvalues from lightbuffer and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,           // rt texture with lightvalues                            
                             combinedDiffColor,     // objects albedo diffuse texture color
                             0,                     // objects emissive texture color not used
                             0,                     // objects emissive intensity
                             combinedSpecularColor, // objects specular texture color
                             MatSpecularIntensity);  // objects specular intensity   
    
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // hrd stuff
    oColor = EncodeHDR(oColor);
}    

//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- COLOR ALPHA ---------------------------------------------------------
/**
    The color pass with alpha flag  
*/
void
ColorAlphaPixelShader(float4 projPos           : TEXCOORD0,
                      float4 uv01              : TEXCOORD1,                         
                      float2 screenPos         : VPOS,  
                      out float4 oColor        : COLOR)
{    
    float2 screenUv = psComputeScreenCoord(screenPos.xy, pixelSize.xy);
    float4 lightValues = tex2D(lightSampler, screenUv);
    
    float4 combinedDiffColor, combinedSpecularColor;
    psMultilayerColorSpecIntensity(uv01, combinedDiffColor, combinedSpecularColor);    
    // modulate by lightmapcolor
    float4 lightmapColor = tex2D(lightmapSampler, uv01.zw);
    combinedDiffColor = combinedDiffColor * lightmapColor * brightness;  

    // modulate diffuse texture with lightvalues from lightbuffer and objects' emissive and specular maps    
    oColor = psLightMaterial(lightValues,           // rt texture with lightvalues                            
                             combinedDiffColor,     // objects albedo diffuse texture color
                             0,                     // objects emissive texture color not used
                             0,                     // objects emissive intensity
                             combinedSpecularColor, // objects specular texture color
                             MatSpecularIntensity);  // objects specular intensity
    
    // add per pixel fog
    oColor = psFog(projPos.z, oColor);
    // FIXME: alphaBlendFactor uses INTENSITY0 which is alread yused by brightness for lightmap
    // modulate alpha by alphaBlendFactor
    //oColor.a *= alphaBlendFactor;
    // hrd stuff
    oColor = EncodeHDR(oColor);
}    

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------- VARIATIONS ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// FIXME: move cullmode and alpharef to better position if possible!
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 120;

//---------------------------- Deferred Lighting -----------------------------------------------------------------------     
//---------------------------------------------------------------------------------------------------------------------
/**
    Techniques for non-deformed geometry.
*/ 
SimpleTechnique(NormalDepth, "NormalDepth", NormalDepthMultilayerPassVertexShader, NormalDepthMultilayerPassPixelShader);
SimpleTechnique(ESMDepth, "ESMDepth", ShadowMappingVertexShader, ShadowMappingPixelShader);
SimpleTechnique(Solid0, "Solid", ColorVertexShader, ColorPixelShader);
SimpleTechnique(Alpha0, "Alpha", ColorVertexShader, ColorAlphaPixelShader);

//------------------------------------------------------------------------------
/**
    Techniques for ground under grass.
*/ 
#include "grassrendererfuncs.fxh"
SimpleTechnique(GrassHeight, "GrassHeight", vsHeightMap, psHeightMap);
SimpleTechnique(GrassColorLightmapNormalWeight, "GrassColorLightmapNormalWeight", vsColorLightmapNormalWeight, psColorLightmapNormalWeight);
SimpleTechnique(GrassPatchmap, "GrassPatchmap", vsGrassPatchmapOnly, psGrassPatchmapOnly);