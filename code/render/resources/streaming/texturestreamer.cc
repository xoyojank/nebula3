//------------------------------------------------------------------------------
//  texturestreamer.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "texturestreamer.h"

#if __WIN32__ 
namespace Resources
{
    __ImplementClass(Resources::TextureStreamer, 'TXST', Resources::D3D9TextureStreamer);
}
#else
#error "TextureStreamer class not implemented on this platform!"
#endif