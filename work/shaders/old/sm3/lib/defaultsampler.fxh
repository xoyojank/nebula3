//------------------------------------------------------------------------------
/**
    Default sampler, use only one semantic per variable in one shader!
*/
const float MipLodBias = -0.5;

texture diffMap : DiffMap0;
sampler diffMapSampler = sampler_state
{
    Texture     = <diffMap>;
    AddressU    = Wrap;
    AddressV    = Wrap;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
    MipMapLodBias = <MipLodBias>;
};

texture bumpMap : BumpMap0;
sampler bumpMapSampler = sampler_state
{
    Texture     = <bumpMap>;
    AddressU    = Wrap;
    AddressV    = Wrap;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
    MipMapLodBias = <MipLodBias>;
};

texture specMap : SpecMap0;
sampler specMapSampler = sampler_state
{
    Texture     = <specMap>;
    AddressU    = Wrap;
    AddressV    = Wrap;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Linear;
    MipMapLodBias = <MipLodBias>;
};

texture lightMap : DiffMap1;
sampler lightMapSampler = sampler_state
{
    Texture     = <lightMap>;
    AddressU    = Clamp;
    AddressV    = Clamp;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Point;
    MipMapLodBias = <MipLodBias>;
};

texture environmentMap : CubeMap0;
sampler environmentSampler = sampler_state
{
    Texture     = <environmentMap>;
    AddressU    = Wrap;
    AddressV    = Wrap;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Point;
    MipMapLodBias = <MipLodBias>;
};

texture emissiveMap : EmsvMap0;
sampler emsvSampler = sampler_state
{
    Texture     = <emissiveMap>;
    AddressU    = Wrap;
    AddressV    = Wrap;
    MinFilter   = Linear;
    MagFilter   = Linear;
    MipFilter   = Point;
    MipMapLodBias = <MipLodBias>;
};

shared texture depthTexture : DepthBuffer;
sampler depthSampler = sampler_state
{
    Texture = <depthTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
    MipMapLodBias = 0;
};

texture colorTexture : ColorBuffer;
sampler colorSampler = sampler_state
{
    Texture = <colorTexture>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
    MipMapLodBias = <MipLodBias>;
};
