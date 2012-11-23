//------------------------------------------------------------------------------
//  transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/transformdevice.h"

namespace CoreGraphics
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Win360::D3D9TransformDevice);
#elif __WII__
__ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Wii::WiiTransformDevice);
#elif __PS3__
__ImplementClass(CoreGraphics::TransformDevice, 'TRDV', PS3::PS3TransformDevice);
#else
#error "TransformDevice class not implemented on this platform!"
#endif
__ImplementSingleton(CoreGraphics::TransformDevice);

//------------------------------------------------------------------------------
/**
*/
TransformDevice::TransformDevice()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TransformDevice::~TransformDevice()
{
    __DestructSingleton;
}

} // namespace CoreGraphics
