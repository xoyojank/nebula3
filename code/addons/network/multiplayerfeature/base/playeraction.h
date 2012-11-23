#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::NetworkEventBase

    This class represents a master event created by the master to be handled by the slave,  like 
    attacking a target with a specified spell or moving around.

    Implement behaviour in subclass, and put the message to the objectview.
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Base
{
class NetworkEventBase : public Core::RefCounted
{
    __DeclareClass(NetworkEventBase);

public:
    /// constructor
    NetworkEventBase();
    /// destructor
    virtual ~NetworkEventBase();        

private:
   
};  
}; // namespace Base
//------------------------------------------------------------------------------