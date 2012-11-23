#pragma once
//------------------------------------------------------------------------------
/**
    @class Resources::TextureStreamer
  
    Resource loader for loading texture data from a Nebula3 stream. Supports
    synchronous and asynchronous loading. Platform Wrapper.
    
    (C) 2010 Radon Labs GmbH
*/

#if __WIN32__
#include "resources/d3d9streaming/d3d9texturestreamer.h"
namespace Resources
{
class TextureStreamer : public Resources::D3D9TextureStreamer
{
    __DeclareClass(TextureStreamer);
};
} // namespace Resources
#else
#error "TextureStreamer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------