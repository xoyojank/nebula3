//------------------------------------------------------------------------------
//  message.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "messaging/message.h"
#include "messaging/port.h"

namespace Messaging
{
__ImplementClass(Messaging::Message, 'MSG_', Core::RefCounted);
__ImplementMsgId(Message);

//------------------------------------------------------------------------------
/**
*/
Message::Message() :
    handled(0),
    deferred(false),
    deferredHandled(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Message::Encode(const Ptr<IO::BinaryWriter>& writer)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Message::Decode(const Ptr<IO::BinaryReader>& reader)
{
    // empty
}

} // namespace Message