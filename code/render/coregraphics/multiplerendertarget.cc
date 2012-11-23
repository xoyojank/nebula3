//------------------------------------------------------------------------------
//  multiplerendertarget.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/multiplerendertarget.h"
#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MultipleRenderTarget, 'MRTG', Base::MultipleRenderTargetBase);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MultipleRenderTarget, 'MRTG', Base::MultipleRenderTargetBase);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::MultipleRenderTarget, 'MRTG', Base::MultipleRenderTargetBase);
}
#else
#error "MultipleRenderTarget class not implemented on this platform!"
#endif
