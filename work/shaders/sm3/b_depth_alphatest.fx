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
        // just enable alphatest
        AlphaTestEnable  = True;
        ZFunc            = LessEqual;
    }
}

