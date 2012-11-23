#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::InternalMultiplayerHandler
    
    Runs in the InternalMultiplayer thread context, setup the InternalMultiplayerServer
    and processes messages to the InternalMultiplayer thread.
    The messages are handled by the Platform-specific InternalMultiplayerServer.

    (C) 2009 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "framesync/framesynctimer.h"
#include "threading/safeflag.h"
#include "threading/event.h"
#include "internalmultiplayer/internalmultiplayerserver.h"         
#include "http/httpserverproxy.h"

//------------------------------------------------------------------------------
namespace InternalMultiplayer
{
class InternalMultiplayerHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(InternalMultiplayerHandler);
public:
    /// constructor
    InternalMultiplayerHandler();
    /// destructor
    virtual ~InternalMultiplayerHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work
    virtual void DoWork();

private:
    bool isInternalMultiplayerRuntimeValid;
    Ptr<FrameSync::FrameSyncTimer> frameSyncTimer;
    Ptr<InternalMultiplayerServer> internalMultiplayerServer;
#if __NEBULA3_HTTP__
    Ptr<Http::HttpServerProxy> httpServerProxy;
#endif
};
} // namespace InternalMultiplayerHandler
//------------------------------------------------------------------------------

