//------------------------------------------------------------------------------
//  resourcebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/resourcebase.h"

namespace Base
{
__ImplementClass(Base::ResourceBase, 'RSBS', Resources::Resource);

//------------------------------------------------------------------------------
/**
*/
ResourceBase::ResourceBase() :
    usage(UsageImmutable),
    access(AccessNone)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ResourceBase::~ResourceBase()
{
    // empty
}

} // namespace Base