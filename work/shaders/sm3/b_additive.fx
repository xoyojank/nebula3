//------------------------------------------------------------------------------
//  b_additive.fx
//
//  Batch shader for additive translucent shapes.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ZEnable           = True;
        ZWriteEnable      = False;
        ZFunc             = LessEqual;
        AlphaBlendEnable  = true;
        AlphaTestEnable   = false;
        DestBlend         = One;
        SrcBlend          = One;
    }
}
