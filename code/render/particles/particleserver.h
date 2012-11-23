#pragma once
//------------------------------------------------------------------------------
/**
    @class Particles::ParticleServer
    
    Setup the particles subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "particles/particlerenderer.h"

//------------------------------------------------------------------------------
namespace Particles
{
class ParticleServer : public Core::RefCounted
{
    __DeclareClass(ParticleServer);
    __DeclareSingleton(ParticleServer);
public:
    /// constructor
    ParticleServer();
    /// destructor
    virtual ~ParticleServer();
    
    /// setup particle subsystem
    void Open();
    /// discard particle subsystem
    void Close();
    /// return true if particle subsystem has been setup
    bool IsOpen() const;

private:
    bool isOpen;
    Ptr<ParticleRenderer> particleRenderer;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleServer::IsOpen() const
{
    return this->isOpen;
}

} // namespace Particles
//------------------------------------------------------------------------------

    