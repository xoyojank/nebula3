//------------------------------------------------------------------------------
//  b_solid.fx
//
//  Batch shader for solid shapes.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ZEnable           = True;
        ZWriteEnable      = True;
        ZFunc             = LessEqual;
        AlphaBlendEnable  = False;
        AlphaTestEnable   = False; 
    }
}
