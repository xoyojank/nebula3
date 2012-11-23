//------------------------------------------------------------------------------
//  emitterattrs.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "particles/emitterattrs.h"

namespace Particles
{

//------------------------------------------------------------------------------
/**
*/
EmitterAttrs::EmitterAttrs()
{
    Memory::Clear(this->floatValues, sizeof(this->floatValues));
    Memory::Clear(this->intValues, sizeof(this->intValues));
    Memory::Clear(this->boolValues, sizeof(this->boolValues));
}

} // namespace Particles