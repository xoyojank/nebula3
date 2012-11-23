//------------------------------------------------------------------------------
//  b_shape.fx
//  Batch shader for debug visualization.
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
*/
technique Default
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = True; // anti aliasing must be disabled, otherwise depth surface will be cleared before any rendering on the framebuffer takes place
        ZWriteEnable      = False;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = True;
        SrcBlend          = SrcAlpha;
        DestBlend         = InvSrcAlpha;
        AlphaTestEnable   = False;
        ScissorTestEnable = False;
        CullMode          = CW;        
    }
}
