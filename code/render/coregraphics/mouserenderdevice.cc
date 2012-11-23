//------------------------------------------------------------------------------
//  mouserenderdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/mouserenderdevice.h"

namespace CoreGraphics
{
#if (__WIN32__ || __XBOX360__ || __PS3__)
__ImplementClass(CoreGraphics::MouseRenderDevice, 'MRDV', Base::MouseRenderDeviceBase);
__ImplementSingleton(CoreGraphics::MouseRenderDevice);
#elif __WII__
__ImplementClass(CoreGraphics::MouseRenderDevice, 'MRDV', Wii::WiiMouseRenderDevice);
__ImplementSingleton(CoreGraphics::MouseRenderDevice);
#else
#error "MouseRenderDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
MouseRenderDevice::MouseRenderDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
MouseRenderDevice::~MouseRenderDevice()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
