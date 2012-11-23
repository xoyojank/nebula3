//------------------------------------------------------------------------------
//  internalmultiplayer/raknet/raknetparameterresolver.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknetparameterresolver.h"

namespace RakNet
{
__ImplementClass(RakNetParameterResolver, 'RSEP', Base::ParameterResolverBase);

//------------------------------------------------------------------------------
/**
*/
RakNetParameterResolver::RakNetParameterResolver()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RakNetParameterResolver::~RakNetParameterResolver()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
const BitStream*
RakNetParameterResolver::EncodeToBitstream() const
{
    n_error("Implement in application specific subclass!");
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
RakNetParameterResolver::DecodeFromBitstream( BitStream* stream )
{
    n_error("Implement in application specific subclass!");
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetParameterResolver::CompareParameterSetWithBitstream( BitStream* stream )
{
    n_error("Implement in application spcific subclass!");
    return false;
}

}