#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MouseRenderDevice
  
    Platform-wrapper for mouse pointer rendering.
    
    (C) 2009 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__ || __PS3__)
#include "coregraphics/base/mouserenderdevicebase.h"
namespace CoreGraphics
{
class MouseRenderDevice : public Base::MouseRenderDeviceBase
{
    __DeclareClass(MouseRenderDevice);
    __DeclareSingleton(MouseRenderDevice);
public:
    /// constructor
    MouseRenderDevice();
    /// destructor
    virtual ~MouseRenderDevice();
};
} // namespace CoreGraphics
#elif __WII__
#include "coregraphics/wii/wiimouserenderdevice.h"
namespace CoreGraphics
{
class MouseRenderDevice : public Wii::WiiMouseRenderDevice
{
    __DeclareClass(MouseRenderDevice);
    __DeclareSingleton(MouseRenderDevice);
public:
    /// constructor
    MouseRenderDevice();
    /// destructor
    virtual ~MouseRenderDevice();
};
} // namespace CoreGraphics
#else
#error "CoreGraphics::MouseRenderDevice not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
