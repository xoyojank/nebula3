//------------------------------------------------------------------------------
//  particleserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/particleserver.h"

namespace Particles
{
__ImplementClass(Particles::ParticleServer, 'PRSV', Core::RefCounted);
__ImplementSingleton(Particles::ParticleServer);

//------------------------------------------------------------------------------
/**
*/
ParticleServer::ParticleServer() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ParticleServer::~ParticleServer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleServer::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;

    // setup the particle renderer singleton
    this->particleRenderer = ParticleRenderer::Create();
    this->particleRenderer->Setup();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleServer::Close()
{
    n_assert(this->IsOpen());

    // destroy the particle renderer singleton
    this->particleRenderer->Discard();
    this->particleRenderer = 0;

    this->isOpen = false;
}

} // namespace Particles