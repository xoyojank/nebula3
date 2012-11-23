#pragma once
//------------------------------------------------------------------------------
/*
    @class Resources::TextureInfo

    TextureInfo is a platform wrapper for ResourceInfo classes specialized for textures like D3D9TextureInfo.

    (C) 2010 Radon Labs GmbH
*/

//------------------------------------------------------------------------------
//#if __WIN32__
#include "resources/d3d9streaming/d3d9textureinfo.h"
namespace Resources
{
class TextureInfo : public Resources::D3D9TextureInfo
{
}; // namespace Resources
}
//#else
//#error "TextureInfo class not implemented on this platform!"
//#endif
//------------------------------------------------------------------------------