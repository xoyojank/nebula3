#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::TextRenderer
  
    A simple text renderer for drawing text on screen.
    Only for debug purposes.
    
    (C) 2008 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9textrenderer.h"
namespace CoreGraphics
{
class TextRenderer : public Direct3D9::D3D9TextRenderer
{
    __DeclareClass(TextRenderer);
    __DeclareSingleton(TextRenderer);
public:
    /// constructor
    TextRenderer();
    /// destructor
    virtual ~TextRenderer();
};
} // namespace CoreGraphics
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360textrenderer.h"
namespace CoreGraphics
{
class TextRenderer : public Xbox360::Xbox360TextRenderer
{
    __DeclareClass(TextRenderer);
    __DeclareSingleton(TextRenderer);
public:
    /// constructor
    TextRenderer();
    /// destructor
    virtual ~TextRenderer();
};
} // namespace CoreGraphics
#elif __WII__
#include "coregraphics/wii/wiitextrenderer.h"
namespace CoreGraphics
{
class TextRenderer : public Wii::WiiTextRenderer
{
    __DeclareClass(TextRenderer);
    __DeclareSingleton(TextRenderer);
public:
    /// constructor
    TextRenderer();
    /// destructor
    virtual ~TextRenderer();
};
} // namespace CoreGraphics
#elif __PS3__
#if !PUBLIC_BUILD && !DISTRIBUTE_ASSERT
#include "coregraphics/ps3/ps3textrenderer.h"
namespace CoreGraphics
{
class TextRenderer : public PS3::PS3TextRenderer
{
    __DeclareClass(TextRenderer);
    __DeclareSingleton(TextRenderer);
public:
    /// constructor
    TextRenderer();
    /// destructor
    virtual ~TextRenderer();
};
} // namespace CoreGraphics
#else
#include "coregraphics/base/textrendererbase.h"
namespace CoreGraphics
{
class TextRenderer : public Base::TextRendererBase
{
    __DeclareClass(TextRenderer);
    __DeclareSingleton(TextRenderer);
public:
    /// constructor
    TextRenderer();
    /// destructor
    virtual ~TextRenderer();
};
} // namespace CoreGraphics
#endif
#else
#error "TextRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

