//------------------------------------------------------------------------------
//  pe_compose.fx
//
//  Simple frame composition post effect shader.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "common.fxh"
#include "posteffect.fxh"
#include "util.fxh"
#include "posteffectsampler.fxh"
#include "defaultsampler.fxh"

//------------------------------------------------------------------------------
//  Get a depth-of-field blurred sample
//------------------------------------------------------------------------------
float4 psDepthOfField(sampler sourceTexture, float2 uv)
{
    // get an averaged depth value        
    float4 objectDepthPacked = tex2D(dsfObjectDepthSampler, uv);    
    float depth = UnpackDepth(objectDepthPacked); 
    
    // compute focus-blur param (0 -> no blur, 1 -> max blur)
    float focusDist = dofDistances.x;
    float focusLength = dofDistances.y;
    float filterRadius = dofDistances.z;    
    float focus = saturate(abs(depth - focusDist) / focusLength);
    
    // perform a gaussian blur around uv
    float4 sample = 0.0f;
    float dofWeight = 1.0f / MaxDofSamples;
    float2 uvMul = focus * filterRadius * pixelSize.xy;
    int i;
    for (i = 0; i < MaxDofSamples; i++)
    {
        sample += tex2D(sourceTexture, uv + (DofSamples[i] * uvMul));
    }
    sample *= dofWeight;
    return sample;
} 

//------------------------------------------------------------------------------
/**
*/
float4 psMain(const vsInOutPostEffect psIn) : COLOR
{
    // perform depth-of-field blurring
#if ENABLE_DEPTHOFFIELD
    float4 c = DecodeHDR(psDepthOfField(colorSampler, psIn.uv0));
#else
    float4 c = DecodeHDR(tex2D(colorSampler, psIn.uv0));
#endif                
       
    float4 bloom = tex2D(bloomSampler, psIn.uv0);
    c += bloom;    
    float4 grey = dot(c.xyz, luminance.xyz);
    float4 color = balance * lerp(grey, c, saturation);
    
#if ENABLE_SCALEFORM
    float4 gui = tex2D(guiSampler, psIn.uv0);
    color = float4(lerp(color.rgb, gui.rgb, gui.a), color.a);
#endif
    color.rgb *= fadeValue;
    
    return color;
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
        VertexShader     = compile vs_3_0 vsMain();
        PixelShader      = compile ps_3_0 psMain();
    }
}
