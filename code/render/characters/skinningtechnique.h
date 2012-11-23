#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::SkinningTechnique
    
    The various character skinning techniques supported by N3.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Characters
{
class SkinningTechnique
{
public:
    enum Code
    {
        InvalidSkinningTechnique,
        SoftwareSkinning,       // some sort of software-skinning
        GPUSkinning,            // skinning in vertex shader, joint palette in vertex constants
        GPUTextureSkinning,     // skinning in vertex shader, joint palette in vertex texture
    };
};

} // namespace Characters
//------------------------------------------------------------------------------
    