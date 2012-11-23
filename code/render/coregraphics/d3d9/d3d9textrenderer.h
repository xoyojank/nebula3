#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9TextRenderer
  
    Implements a simple text renderer for Direct3D9. This is only intended
    for outputting debug text, not for high-quality text rendering!

    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/textrendererbase.h"
#include "coregraphics/d3d9/d3d9resource.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9TextRenderer : public Base::TextRendererBase, public D3D9Resource
{
    __DeclareClass(D3D9TextRenderer);
    __DeclareSingleton(D3D9TextRenderer);
public:
    /// constructor
    D3D9TextRenderer();
    /// destructor
    virtual ~D3D9TextRenderer();

    /// open the device
    void Open();
    /// close the device
    void Close();
    /// draw the accumulated text
    void DrawTextElements();

private:
	/// called when d3d9 device is lost
	virtual void OnLostDevice();
	/// called when d3d9 devcie is reset
	virtual void OnResetDevice();

private:
    ID3DXFont* d3dFont;
    ID3DXSprite* d3dSprite;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
