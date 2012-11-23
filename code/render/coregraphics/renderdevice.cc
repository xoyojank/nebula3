//------------------------------------------------------------------------------
//  renderdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/renderdevice.h"

namespace CoreGraphics
{
#if __WIN32__
__ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Direct3D9::D3D9RenderDevice);
#elif __XBOX360__
__ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Xbox360::Xbox360RenderDevice);
#elif __WII__
__ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Wii::WiiRenderDevice);
#elif __PS3__
__ImplementClass(CoreGraphics::RenderDevice, 'RDVC', PS3::PS3RenderDevice);
#else
#error "RenderDevice class not implemented on this platform!"
#endif

__ImplementSingleton(CoreGraphics::RenderDevice);

//------------------------------------------------------------------------------
/**
*/
RenderDevice::RenderDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RenderDevice::~RenderDevice()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
