//------------------------------------------------------------------------------
//  texture.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/texture.h"

#if __WIN32__ 
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Direct3D9::D3D9Texture);
}
#elif __XBOX360__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Xbox360::Xbox360Texture);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Wii::WiiTexture);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Base::TextureBase);
}
#else
#error "Texture class not implemented on this platform!"
#endif
