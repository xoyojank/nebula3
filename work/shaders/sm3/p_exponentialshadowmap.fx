//------------------------------------------------------------------------------
//  p_exponentialshadowmap.fx
//
//  
//
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED;
        ZEnable           = true;
        ZWriteEnable      = true;
        ZFunc             = LessEqual;       
        FogEnable         = False;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False;        
        ScissorTestEnable = False;
        CullMode          = CW;
    }
}