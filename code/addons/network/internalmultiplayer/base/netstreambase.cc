//------------------------------------------------------------------------------
//  NetStreamBase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/base/netstreambase.h"

namespace Base
{
__ImplementClass(Base::NetStreamBase, 'NSTB', IO::Stream);

using namespace IO;
using namespace InternalMultiplayer;

//------------------------------------------------------------------------------
/**
*/
NetStreamBase::NetStreamBase() : isReliable(false)
{
}

//------------------------------------------------------------------------------
/**
*/
NetStreamBase::~NetStreamBase()
{  
}

//------------------------------------------------------------------------------
/**
*/
void 
NetStreamBase::WriteBits( const unsigned char* ptr, uint numBits )
{
    n_error("Not implemented!");
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
NetStreamBase::ReadBits( unsigned char* ptr, uint numBits )
{
    n_error("Not implemented!");
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
NetStreamBase::SetReadIndex(IndexT bitIndex)
{   
    n_error("Not implemented!");
}

//------------------------------------------------------------------------------
/**
*/
IndexT 
NetStreamBase::GetReadIndex() const
{            
    n_error("Not implemented!");
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
NetStreamBase::IgnoreBits(SizeT numBits)
{    
    n_error("Not implemented!");
}
}