#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexLayout
    
    Describe the layout of vertices in a vertex buffer.
    
    (C) 2006 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9vertexlayout.h"
namespace CoreGraphics
{
class VertexLayout : public Win360::D3D9VertexLayout
{
    __DeclareClass(VertexLayout);
};
}
#elif __WII__
#include "coregraphics/wii/wiivertexlayout.h"
namespace CoreGraphics
{
class VertexLayout : public Wii::WiiVertexLayout
{
    __DeclareClass(VertexLayout);
};
}
#elif __PS3__
#include "coregraphics/base/vertexlayoutbase.h"
namespace CoreGraphics
{
class VertexLayout : public Base::VertexLayoutBase
{
    __DeclareClass(VertexLayout);
};
}
#else
#error "VertexLayout class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

