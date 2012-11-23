//------------------------------------------------------------------------------
//  grassrendererfuncs.fx
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
struct vsInputPos
{
    float4 position      : POSITION;
}; 

struct vsInput
{
    float4 position      : POSITION;
    float2 uv0           : TEXCOORD0;
}; 

struct vsInputLightmap
{
    float4 position      : POSITION;
    float2 uv0           : TEXCOORD0;
    float2 uv1           : TEXCOORD1;
}; 

struct vsInputColorLightmapNormalWeight
{
    float4 position      : POSITION;
    float2 uv0           : TEXCOORD0;
    float2 uv1           : TEXCOORD1;
    float3 normal        : Normal;
};    

struct vsOutput
{
    float4 position      : POSITION;
    float2 uv0           : TEXCOORD0;
};    

struct vsOutputHeight
{
    float4 position : POSITION;
    float depth : TEXCOORD1;
};

struct psOutputLightmapNormalWeightColor
{
    float4 color    : COLOR0;
    float4 lightmap : COLOR1;
    float4 normal   : COLOR2;
    float4 weight   : COLOR3;
};

struct vsOutputColorLightmapNormalWeight
{
    float4 position      : POSITION;
    float4 uv01           : TEXCOORD0;
    float3 worldNormal   : TEXCOORD1;
};


texture grassMap : DiffMap4;
sampler GrassWeightSampler = sampler_state
{
    Texture = <grassMap>;
    AddressU  = Clamp;
    AddressV  = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;
};

//------------------------------------------------------------------------------
/**
    VertexShader for rendering the global Y coordinate. This is used
    for the heightmap rendering of the Mangalore grass renderer.
*/
vsOutputHeight vsHeightMap(const vsInputPos vsIn)
{
    vsOutputHeight vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    float4 globalCoords = mul(vsIn.position, model);
    vsOut.depth = globalCoords.y;
        
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    PixelShader for rendering the global Y coordinate and color to two rendertargets. This is used
    for the heightmap rendering of the Mangalore grass renderer.
*/
float4 psHeightMap(const vsOutputHeight psIn) : COLOR
{    
    return float4(psIn.depth, 0.0f, 0.0f, 1.0f);
}

//------------------------------------------------------------------------------
/**
*/
vsOutputColorLightmapNormalWeight vsColorLightmapNormalWeight(const vsInputColorLightmapNormalWeight vsIn)
{
    vsOutputColorLightmapNormalWeight vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    vsOut.uv01.xy = UnpackUv(vsIn.uv0);  
    vsOut.uv01.zw = UnpackUv(vsIn.uv1);
    vsOut.worldNormal = (normalize(mul(float4(UnpackNormal(vsIn.normal), 0.0), model).xyz) * 0.5) + 0.5;     
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Render lightmap and weight information for the multilayer shader.
*/
psOutputLightmapNormalWeightColor psColorLightmapNormalWeight(const vsOutputColorLightmapNormalWeight psIn)
{
    psOutputLightmapNormalWeightColor psOut;
    
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
    
    psOut.color = combinedColor;    
    psOut.lightmap = tex2D(lightmapSampler, psIn.uv01.zw) * 0.5; 
    psOut.normal = float4(psIn.worldNormal, 1.0f);    
    // increase contrast
    psOut.weight = tex2D(GrassWeightSampler, psIn.uv01.xy) * 4; 
    
    return psOut;
}

//------------------------------------------------------------------------------
/**
*/
vsOutput vsGrassPatchmapOnly(const vsInput vsIn)
{
    vsOutput vsOut;
    vsOut.position = mul(vsIn.position, mvp);
    vsOut.uv0 = UnpackUv(vsIn.uv0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**    
*/
float4 psGrassPatchmapOnly(const vsOutput psIn) : COLOR
{
    // increase contrast
    return tex2D(GrassWeightSampler, psIn.uv0) * 4;
}
