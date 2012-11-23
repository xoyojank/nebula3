#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::DisplayDevice
  
    A DisplayDevice object represents the display where the RenderDevice
    presents the rendered frame. 
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9displaydevice.h"
namespace CoreGraphics
{
class DisplayDevice : public Direct3D9::D3D9DisplayDevice
{
    __DeclareClass(DisplayDevice);
    __DeclareSingleton(DisplayDevice);
public:
    /// constructor
    DisplayDevice();
    /// destructor
    virtual ~DisplayDevice();
};
} // namespace CoreGraphics
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360displaydevice.h"
namespace CoreGraphics
{
class DisplayDevice : public Xbox360::Xbox360DisplayDevice
{
    __DeclareClass(DisplayDevice);
    __DeclareSingleton(DisplayDevice);
public:
    /// constructor
    DisplayDevice();
    /// destructor
    virtual ~DisplayDevice();
};
}
#elif __WII__
#include "coregraphics/wii/wiidisplaydevice.h"
namespace CoreGraphics
{
class DisplayDevice : public Wii::WiiDisplayDevice
{
    __DeclareClass(DisplayDevice);
    __DeclareSingleton(DisplayDevice);
public:
    /// constructor
    DisplayDevice();
    /// destructor
    virtual ~DisplayDevice();
};
}
#elif __PS3__
#include "coregraphics/ps3/ps3displaydevice.h"
namespace CoreGraphics
{
class DisplayDevice : public PS3::PS3DisplayDevice
{
    __DeclareClass(DisplayDevice);
    __DeclareSingleton(DisplayDevice);
public:
    /// constructor
    DisplayDevice();
    /// destructor
    virtual ~DisplayDevice();
};
}
#else
#error "CoreGraphics::DisplayDevice not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

