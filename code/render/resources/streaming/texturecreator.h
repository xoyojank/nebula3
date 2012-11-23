#pragma once
//------------------------------------------------------------------------------
/*
    @class Resources::TexturCreator

    A specialized creator for directX9-textures.

    (C) 2010 Radon Labs GmbH
    */

#if __WIN32__
#include "resources/d3d9streaming/d3d9texturecreator.h"
namespace Resources
{
    class TextureCreator : public Resources::D3D9TextureCreator
    {
        __DeclareClass(TextureCreator);
    };
} // namespace Resources
#else
#error "TextureCreator class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------