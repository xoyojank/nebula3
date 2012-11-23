#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleRenderer
    
    Platform-wrapper for particle rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "particles/d3d9/d3d9particlerenderer.h"
namespace Particles
{
class ParticleRenderer : public Direct3D9::D3D9ParticleRenderer
{
    __DeclareClass(ParticleRenderer);
    __DeclareSingleton(ParticleRenderer);
public:
    /// constructor
    ParticleRenderer();
    /// destructor
    virtual ~ParticleRenderer();
};
}
#elif __XBOX360__
#include "particles/xbox360/xbox360particlerenderer.h"
namespace Particles
{
class ParticleRenderer : public Xbox360::Xbox360ParticleRenderer
{
    __DeclareClass(ParticleRenderer);
    __DeclareSingleton(ParticleRenderer);
public:
    /// constructor
    ParticleRenderer();
    /// destructor
    virtual ~ParticleRenderer();
};
}
#elif __WII__
#include "particles/wii/wiiparticlerenderer.h"
namespace Particles
{
class ParticleRenderer : public Wii::WiiParticleRenderer
{
    __DeclareClass(ParticleRenderer);
    __DeclareSingleton(ParticleRenderer);
public:
    /// constructor
    ParticleRenderer();
    /// destructor
    virtual ~ParticleRenderer();
};
}
#elif __PS3__
#include "particles/ps3/ps3particlerenderer.h"
namespace Particles
{
class ParticleRenderer : public PS3::PS3ParticleRenderer
{
    __DeclareClass(ParticleRenderer);
    __DeclareSingleton(ParticleRenderer);
public:
    /// constructor
    ParticleRenderer();
    /// destructor
    virtual ~ParticleRenderer();
};
}
#else
#error "Particles::ParticleRenderer not implemented on this platform!"
#endif
//------------------------------------------------------------------------------


    