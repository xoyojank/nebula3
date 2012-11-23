//------------------------------------------------------------------------------
//  lightmapped.fx
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"
#include "util.fxh"

float brightness : Intensity0;
const float MipLodBias = -0.5;

texture lightMap : DiffMap1;
sampler lightmapSampler = sampler_state
{
    Texture     = <lightMap>;
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

struct vsVSMPassOutput
{
    float4 position : POSITION;
    float depth : TEXCOORD0;
};

struct vsColorPassInput
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 uv0      : TEXCOORD0;
    float2 uv1      : TEXCOORD1;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float4 uv01         : TEXCOORD0;
    float4 worldPos    : TEXCOORD2;     // position in world space
    float4 projPos     : TEXCOORD3;     // position in projection space
    float3 normal      : TEXCOORD4;     // vertex normal in world space
    float3 tangent     : TEXCOORD5;     // vertex tangent in world space
    float3 binormal    : TEXCOORD6;     // vertex binormal in world space
    float3 worldEyeVec : COLOR0;        // normalized world space eye vector
};

//------------------------------------------------------------------------------
/**
    Vertex shader function for rendering variance shadow map.
*/
vsVSMPassOutput
VSMPassVertexShader(float4 pos : POSITION)
{
    vsVSMPassOutput vsOut;
    vsOut.position = mul(pos, mvp);
    vsOut.depth = vsOut.position.z;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader function for rendering variance shadow map. Contains
    the light space depth in R, and squared depth in G.
*/
float4
VSMPassPixelShader(vsVSMPassOutput psIn) : COLOR
{
    return EncodePSSMDepth(psIn.depth.x);
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsColorPassOutput
ColorPassVertexShader(const vsColorPassInput vsIn)
{
    vsColorPassOutput vsOut;
    vsOut.position    = mul(vsIn.position, mvp);
    vsOut.projPos     = vsOut.position;
    vsOut.worldPos    = mul(vsIn.position, model);
    vsOut.worldEyeVec = normalize(eyePos - vsOut.worldPos);
    vsOut.normal      = normalize(mul(UnpackNormal(vsIn.normal), model));
    vsOut.tangent     = normalize(mul(UnpackNormal(vsIn.tangent), model));
    vsOut.binormal    = normalize(mul(UnpackNormal(vsIn.binormal), model));
    vsOut.uv01.xy = UnpackUv(vsIn.uv0);
    vsOut.uv01.zw = UnpackUv(vsIn.uv1);
    return vsOut;
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

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{   
    // compute the tile UVs for all 8 layers
    float4 uvLayer01 = psIn.uv01.xyxy * MLPUVStretch0.xxyy;
    float4 uvLayer23 = psIn.uv01.xyxy * MLPUVStretch0.zzww;
    float4 uvLayer45 = psIn.uv01.xyxy * MLPUVStretch1.xxyy;
    
    // read weights
    float4 layer0123Weights = tex2D(Layer0123WeightSampler, psIn.uv01.xy);
    float4 layer4567Weights = tex2D(Layer4567WeightSampler, psIn.uv01.xy);    
    
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
    float4 combinedColor = lerp(layerColor[0], layerColor[1], weight0123.y);
    combinedColor = lerp(combinedColor, layerColor[2], weight0123.z);
    combinedColor = lerp(combinedColor, layerColor[3], weight0123.w);
    combinedColor = lerp(combinedColor, layerColor[4], weight4567.x);
    combinedColor = lerp(combinedColor, layerColor[5], weight4567.y);
    
    // FIXME: compute combined specular intensity!   
    float layer234567WeightSum = saturate(dot(layer4567Weights, float4(1.0f, 1.0f, 1.0f, 1.0f)) + dot(layer0123Weights.zw, float2(1.0f, 1.0f)));
    float specIntensity = saturate(lerp(MLPSpecIntensity0.x, MLPSpecIntensity0.y, weight0123.y) - layer234567WeightSum);
    
    float3 combinedNormal = psMultiLayerSampleNormal(uvLayer01, uvLayer23, layer0123Weights, layer234567WeightSum);
    float3x3 tangentToWorld = float3x3(psIn.tangent, psIn.binormal, psIn.normal);
    float3 worldNormal = mul(combinedNormal, tangentToWorld);     
    
    // modulate lightmapcolor
    float4 lightmapColor = tex2D(lightmapSampler, psIn.uv01.zw);
    float4 matDiffuse = combinedColor * lightmapColor * brightness;
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, float4(specIntensity, specIntensity, specIntensity, 1.0f), MatSpecularPower,  float4(0.0f, 0.0f, 0.0f, 1.0f));    
    color = psFog(psIn.projPos.z, color);
    color = EncodeHDR(color);
    
    return color;
}

//------------------------------------------------------------------------------
/**
*/
float4 
AlphaColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{   
    // compute the tile UVs for all 8 layers
    float4 uvLayer01 = psIn.uv01.xyxy * MLPUVStretch0.xxyy;
    float4 uvLayer23 = psIn.uv01.xyxy * MLPUVStretch0.zzww;
    float4 uvLayer45 = psIn.uv01.xyxy * MLPUVStretch1.xxyy;
    
    // read weights
    float4 layer0123Weights = tex2D(Layer0123WeightSampler, psIn.uv01.xy);
    float4 layer4567Weights = tex2D(Layer4567WeightSampler, psIn.uv01.xy);    
    
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
    float4 combinedColor = lerp(layerColor[0], layerColor[1], weight0123.y);
    combinedColor = lerp(combinedColor, layerColor[2], weight0123.z);
    combinedColor = lerp(combinedColor, layerColor[3], weight0123.w);
    combinedColor = lerp(combinedColor, layerColor[4], weight4567.x);
    combinedColor = lerp(combinedColor, layerColor[5], weight4567.y);
    
    // FIXME: compute combined specular intensity!   
    float layer234567WeightSum = saturate(dot(layer4567Weights, float4(1.0f, 1.0f, 1.0f, 1.0f)) + dot(layer0123Weights.zw, float2(1.0f, 1.0f)));
    float specIntensity = saturate(lerp(MLPSpecIntensity0.x, MLPSpecIntensity0.y, weight0123.y) - layer234567WeightSum);
    
    float3 combinedNormal = psMultiLayerSampleNormal(uvLayer01, uvLayer23, layer0123Weights, layer234567WeightSum);
    float3x3 tangentToWorld = float3x3(psIn.tangent, psIn.binormal, psIn.normal);
    float3 worldNormal = mul(combinedNormal, tangentToWorld);     
    
    // modulate lightmapcolor
    float4 lightmapColor = tex2D(lightmapSampler, psIn.uv01.zw);
    float4 matDiffuse = combinedColor * lightmapColor * brightness;
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, float4(specIntensity, specIntensity, specIntensity, 1.0f), MatSpecularPower,  float4(0.0f, 0.0f, 0.0f, 1.0f));    
    color = psFog(psIn.projPos.z, color);
    color.a = combinedColor.a;
    // premultiplied alpha
    color.rgb *= color.a;
    color = EncodeHDR(color);
    
    return color;
}

//------------------------------------------------------------------------------
int CullMode : CullMode = 2;
int AlphaRef : AlphaRef = 128;
//------------------------------------------------------------------------------
SimpleTechnique(Depth, "Depth", DepthPassVertexShader, DepthPassPixelShader);
SimpleTechnique(VSMDepth, "VSMDepth", VSMPassVertexShader, VSMPassPixelShader);

LightTechnique(Solid0, "Solid|LocalLights0", ColorPassVertexShader, ColorPassPixelShader, 0);
LightTechnique(Solid1, "Solid|LocalLights1", ColorPassVertexShader, ColorPassPixelShader, 1);
LightTechnique(Solid2, "Solid|LocalLights2", ColorPassVertexShader, ColorPassPixelShader, 2);
LightTechnique(Solid3, "Solid|LocalLights3", ColorPassVertexShader, ColorPassPixelShader, 3);
LightTechnique(Solid4, "Solid|LocalLights4", ColorPassVertexShader, ColorPassPixelShader, 4);

LightTechnique(Alpha0, "Alpha|LocalLights0", ColorPassVertexShader, AlphaColorPassPixelShader, 0);
LightTechnique(Alpha1, "Alpha|LocalLights1", ColorPassVertexShader, AlphaColorPassPixelShader, 1);
LightTechnique(Alpha2, "Alpha|LocalLights2", ColorPassVertexShader, AlphaColorPassPixelShader, 2);
LightTechnique(Alpha3, "Alpha|LocalLights3", ColorPassVertexShader, AlphaColorPassPixelShader, 3);
LightTechnique(Alpha4, "Alpha|LocalLights4", ColorPassVertexShader, AlphaColorPassPixelShader, 4);

//------------------------------------------------------------------------------
/**
    Techniques for ground under grass.
*/ 
#include "grassrendererfuncs.fxh"
SimpleTechnique(GrassHeight, "GrassHeight", vsHeightMap, psHeightMap);
SimpleTechnique(GrassColorLightmapNormalWeight, "GrassColorLightmapNormalWeight", vsColorLightmapNormalWeight, psColorLightmapNormalWeight);
SimpleTechnique(GrassPatchmap, "GrassPatchmap", vsGrassPatchmapOnly, psGrassPatchmapOnly);