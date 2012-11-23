//------------------------------------------------------------------------------
/**
    Default sampler, use only one semantic per variable in one shader!
*/

texture sourceTexture : SourceBuffer;
sampler sourceSampler = sampler_state
{
    Texture = <sourceTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
};

texture bloomTexture : BloomBuffer;
sampler bloomSampler = sampler_state
{
    Texture = <bloomTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = None;
};
