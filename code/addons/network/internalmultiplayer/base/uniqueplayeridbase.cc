//------------------------------------------------------------------------------
//  uniqueplayeridbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/base/uniqueplayeridbase.h"

namespace Base
{
//------------------------------------------------------------------------------
/**
*/
UniquePlayerIdBase::UniquePlayerIdBase()
{

}

//------------------------------------------------------------------------------
/**
*/
UniquePlayerIdBase::~UniquePlayerIdBase()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
UniquePlayerIdBase::Encode(const Ptr<InternalMultiplayer::BitWriter>& writer) const
{
    n_error("UniquePlayerIdBase::Encode called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
UniquePlayerIdBase::Decode(const Ptr<InternalMultiplayer::BitReader>& reader)
{
    n_error("UniquePlayerIdBase::Decode called!");
}
} // namespace InternalMultiplayer
