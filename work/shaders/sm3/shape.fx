//------------------------------------------------------------------------------
//  shape.fx
//  Shape shader for debug visualization.
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

float4x4 mvp : ModelViewProjection;
float4 diffColor : MatDiffuse;

//------------------------------------------------------------------------------
/**
*/
float4
VertexShaderFunc(float4 pos : POSITION) : POSITION
{
    return mul(pos, mvp);
}

//------------------------------------------------------------------------------
/**
*/
float4
PixelShaderFunc() : COLOR
{
    return diffColor;
}

//------------------------------------------------------------------------------
/**
*/
technique Default
{
    pass p0
    {
        VertexShader = compile vs_3_0 VertexShaderFunc();
        PixelShader = compile ps_3_0 PixelShaderFunc();
    }
}
