//------------------------------------------------------------------------------
//  p_color.fx
//
//  Pass-shader for the color pass.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

#include "shared.fxh"

technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = true;
        ZWriteEnable      = true;
        ZFunc             = LessEqual;       
        FogEnable         = False;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False;
        AlphaFunc         = GreaterEqual;
        ScissorTestEnable = False;
        CullMode          = CW;
    }
}
       
