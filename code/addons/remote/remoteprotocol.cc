//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remoteprotocol.h"

namespace Remote
{
    __ImplementClass(Remote::AttachRequestHandler, 'arrq', Messaging::Message);
    __ImplementMsgId(AttachRequestHandler);
    __ImplementClass(Remote::RemoveRequestHandler, 'rrrq', Messaging::Message);
    __ImplementMsgId(RemoveRequestHandler);
    __ImplementClass(Remote::AttachCommandHandler, 'acoh', Messaging::Message);
    __ImplementMsgId(AttachCommandHandler);
    __ImplementClass(Remote::RemoveCommandHandler, 'rcoh', Messaging::Message);
    __ImplementMsgId(RemoveCommandHandler);
} // Remote

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
