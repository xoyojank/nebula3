#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::SkinnedMeshRenderer
    
    Wrapper class for platform-specific skinned mesh rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "characters/win32/win32skinnedmeshrenderer.h"
namespace Characters
{
class SkinnedMeshRenderer : public Win32::Win32SkinnedMeshRenderer
{
    __DeclareClass(SkinnedMeshRenderer);
    __DeclareSingleton(SkinnedMeshRenderer);
public:
    /// constructor
    SkinnedMeshRenderer();
    /// destructor
    virtual ~SkinnedMeshRenderer();
};
}
#elif __XBOX360__
#include "characters/xbox360/xbox360skinnedmeshrenderer.h"
namespace Characters
{
    class SkinnedMeshRenderer : public Xbox360::Xbox360SkinnedMeshRenderer
    {
        __DeclareClass(SkinnedMeshRenderer);
        __DeclareSingleton(SkinnedMeshRenderer);
    public:
        /// constructor
        SkinnedMeshRenderer();
        /// destructor
        virtual ~SkinnedMeshRenderer();
    };
}
#elif __WII__
#include "characters/wii/wiiskinnedmeshrenderer.h"
namespace Characters
{
class SkinnedMeshRenderer : public Wii::WiiSkinnedMeshRenderer
{
    __DeclareClass(SkinnedMeshRenderer);
    __DeclareSingleton(SkinnedMeshRenderer);
public:
    /// constructor
    SkinnedMeshRenderer();
    /// destructor
    virtual ~SkinnedMeshRenderer();
};
}
#elif __PS3__
#include "characters/ps3/ps3skinnedmeshrenderer.h"
namespace Characters
{
class SkinnedMeshRenderer : public PS3::PS3SkinnedMeshRenderer
{
    __DeclareClass(SkinnedMeshRenderer);
    __DeclareSingleton(SkinnedMeshRenderer);
public:
    /// constructor
    SkinnedMeshRenderer();
    /// destructor
    virtual ~SkinnedMeshRenderer();
};
}
#endif
//------------------------------------------------------------------------------
    