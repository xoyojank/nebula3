//------------------------------------------------------------------------------
//  b_alphatest.fx
//
//  Batch shader for alpha masked shapes.
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ZEnable           = True;
        ZWriteEnable      = True;  
        ZFunc             = LessEqual;           
        AlphaBlendEnable  = False;
        AlphaTestEnable   = True;
        AlphaRef          = 128;
        Alphafunc         = Greater;
    }
}

