//------------------------------------------------------------------------------
//  p_depth.fx
//
//  Pass-shader for the depth pass.
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA; // write depth as color for later use in other shaders
        ZEnable           = True;
        ZWriteEnable      = True;
        ZFunc             = LessEqual;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False;
        AlphaFunc         = GreaterEqual;
        ScissorTestEnable = False;
        CullMode          = CW;
    }
}
       
