//------------------------------------------------------------------------------
//  network/multiplayerfeature/base/playeractoin.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/base/playeraction.h"

namespace Base
{
__ImplementClass(NetworkEventBase, 'PLAB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
NetworkEventBase::NetworkEventBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
NetworkEventBase::~NetworkEventBase()
{
    // empty
}

} // namespace Base
