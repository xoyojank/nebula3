//------------------------------------------------------------------------------
//  p_color.fx
//
//  Pass-shader for the color pass.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

shared texture depthBuffer : DepthBuffer;
technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = True;
        ZWriteEnable      = False;
        ZFunc             = Equal;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False;
        AlphaFunc         = GreaterEqual;
        ScissorTestEnable = False;
        CullMode          = CW;
    }
}
       
