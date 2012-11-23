//------------------------------------------------------------------------------
//  b_alpha.fx
//
//  Batch shader for translucent shapes.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ZEnable           = True;
        ZWriteEnable      = True;             
        AlphaBlendEnable  = false;
        AlphaTestEnable   = true;
        ZFunc             = LessEqual;
        DestBlend         = InvSrcAlpha;
        SrcBlend          = One; // Premultiplied in pixel shader
    }
}
