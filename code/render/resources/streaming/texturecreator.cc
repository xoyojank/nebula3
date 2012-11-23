//------------------------------------------------------------------------------
//  texturecreator.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "texturecreator.h"

#if __WIN32__ 
namespace Resources
{
    __ImplementClass(Resources::TextureCreator, 'TXCT', Resources::D3D9TextureCreator);
}
#else
#error "TextureCreator class not implemented on this platform!"
#endif