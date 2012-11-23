//------------------------------------------------------------------------------
//  particlerenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particlesysteminstance.h"

namespace Particles
{
#if __WIN32__
__ImplementClass(Particles::ParticleSystemInstance, 'PASI', Direct3D9::D3D9ParticleSystemInstance);
#elif __XBOX360__
__ImplementClass(Particles::ParticleSystemInstance, 'PASI', Xbox360::Xbox360ParticleSystemInstance);
#elif __WII__
__ImplementClass(Particles::ParticleSystemInstance, 'PASI', Wii::WiiParticleSystemInstance);
#elif __PS3__
__ImplementClass(Particles::ParticleSystemInstance, 'PASI', PS3::PS3ParticleSystemInstance);
#else
#error "Particles::ParticleRenderer not implemented on this platform!"
#endif


} // namespace Particles
