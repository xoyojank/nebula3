//------------------------------------------------------------------------------
//  p_varianceshadowmap.fx
//
//  Pass shader for rendering Variance Shadow Maps.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = True;
        ZWriteEnable      = True;
        ZFunc             = LessEqual;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False;
        ScissorTestEnable = False;
        CullMode          = None;
    }
}
