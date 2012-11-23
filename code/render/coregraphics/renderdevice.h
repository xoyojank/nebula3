#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::RenderDevice
  
    The central rendering object of the Nebula3 core graphics system. This
    is basically an encapsulation of the Direct3D device. The render device
    will presents its backbuffer to the display managed by the
    CoreGraphics::DisplayDevice singleton.
    
    (C) 2006 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9renderdevice.h"
namespace CoreGraphics
{
class RenderDevice : public Direct3D9::D3D9RenderDevice
{
    __DeclareClass(RenderDevice);
    __DeclareSingleton(RenderDevice);
public:
    /// constructor
    RenderDevice();
    /// destructor
    virtual ~RenderDevice();
};
} // namespace CoreGraphics
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360renderdevice.h"
namespace CoreGraphics
{
class RenderDevice : public Xbox360::Xbox360RenderDevice
{
    __DeclareClass(RenderDevice);
    __DeclareSingleton(RenderDevice);
public:
    /// constructor
    RenderDevice();
    /// destructor
    virtual ~RenderDevice();
};
} // namespace CoreGraphics
#elif __WII__
#include "coregraphics/wii/wiirenderdevice.h"
namespace CoreGraphics
{
class RenderDevice : public Wii::WiiRenderDevice
{
    __DeclareClass(RenderDevice);
    __DeclareSingleton(RenderDevice);
public:
    /// constructor
    RenderDevice();
    /// destructor
    virtual ~RenderDevice();
};
} // namespace CoreGraphics
#elif __PS3__
#include "coregraphics/ps3/ps3renderdevice.h"
namespace CoreGraphics
{
class RenderDevice : public PS3::PS3RenderDevice
{
    __DeclareClass(RenderDevice);
    __DeclareSingleton(RenderDevice);
public:
    /// constructor
    RenderDevice();
    /// destructor
    virtual ~RenderDevice();
};
} // namespace CoreGraphics
#else
#error "RenderDevice class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
