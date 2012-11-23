//------------------------------------------------------------------------------
//  p_depth.fx
//
//  Pass-shader for the depth pass.
//
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------

shared texture lightProjMap   : LightProjMap;
shared texture normalMap      : NormalBuffer;
shared texture dsfObjectDepthMap : DSFObjectDepthBuffer;
shared texture  ssaoBuffer : SSAOBuffer;
technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA; // write depth as color for later use in other shaders
        ZEnable           = True;
        ZWriteEnable      = false;
        ZFunc             = Greater;
        StencilEnable     = False;        
        AlphaBlendEnable  = true;
        DestBlend         = ONE;
        SrcBlend          = ONE;
        AlphaTestEnable   = False;        
        ScissorTestEnable = False;
        CullMode          = CCW;
    }
}
       
