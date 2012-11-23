//------------------------------------------------------------------------------
//  skinnedmeshrenderer.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characters/skinnedmeshrenderer.h"

namespace Characters
{
#if (__WIN32__)
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', Win32::Win32SkinnedMeshRenderer);
#elif (__XBOX360__)
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', Xbox360::Xbox360SkinnedMeshRenderer);
#elif __WII__
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', Wii::WiiSkinnedMeshRenderer);
#elif __PS3__
__ImplementClass(Characters::SkinnedMeshRenderer, 'SMRR', PS3::PS3SkinnedMeshRenderer);
#else
#error "RenderDevice class not implemented on this platform!"
#endif

__ImplementSingleton(Characters::SkinnedMeshRenderer);

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRenderer::SkinnedMeshRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
    __DestructSingleton;
}

} // namespace Characters
