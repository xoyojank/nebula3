#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShapeRenderer
    
    Render shapes for debug visualizations.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9shaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public Win360::D3D9ShapeRenderer
{
    __DeclareClass(ShapeRenderer);
    __DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();        
};
} // namespace CoreGraphics
#elif __WII__
#include "coregraphics/wii/wiishaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public Wii::WiiShapeRenderer
{
    __DeclareClass(ShapeRenderer);
    __DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();
};
} // namespace CoreGraphics
#elif __PS3__
#include "coregraphics/ps3/ps3shaperenderer.h"
namespace CoreGraphics
{
class ShapeRenderer : public PS3::PS3ShapeRenderer
{
    __DeclareClass(ShapeRenderer);
    __DeclareSingleton(ShapeRenderer);
public:
    /// constructor
    ShapeRenderer();
    /// destructor
    virtual ~ShapeRenderer();
};
} // namespace CoreGraphics
#else
#error "ShapeRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

