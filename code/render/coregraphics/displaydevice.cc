//------------------------------------------------------------------------------
//  displaydevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/displaydevice.h"

namespace CoreGraphics
{
#if __WIN32__
__ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Direct3D9::D3D9DisplayDevice);
__ImplementSingleton(CoreGraphics::DisplayDevice);
#elif __XBOX360__
__ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Xbox360::Xbox360DisplayDevice);
__ImplementSingleton(CoreGraphics::DisplayDevice);
#elif __WII__
__ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Wii::WiiDisplayDevice);
__ImplementSingleton(CoreGraphics::DisplayDevice);
#elif __PS3__
__ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', PS3::PS3DisplayDevice);
__ImplementSingleton(CoreGraphics::DisplayDevice);
#else
#error "DisplayDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
DisplayDevice::DisplayDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DisplayDevice::~DisplayDevice()
{
    __DestructSingleton;
}

} // namespace CoreGraphics

