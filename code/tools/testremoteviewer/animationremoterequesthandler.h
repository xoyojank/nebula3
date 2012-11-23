#pragma once
#ifndef REMOTE_ANIMATIONREMOTEREQUESTHANDLER_H
#define REMOTE_ANIMATIONREMOTEREQUESTHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Remote::AnimationRemoteRequestHandler
    
    This implements the a handler for the remote animation component.
    Answer all remote requests concerning animation.
    
    (C) 2008 Radon Labs GmbH
*/
#include "remote/remoterequesthandler.h"
#include "render/apprender/renderapplication.h"

//------------------------------------------------------------------------------
namespace Remote
{
class AnimationRemoteRequestHandler : public RemoteRequestHandler
{
    __DeclareClass(AnimationRemoteRequestHandler);
public:
    /// Constructor
    AnimationRemoteRequestHandler();
    /// handle a remote request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<RemoteRequest>& request);
};

} // namespace RemoteRequestHandler
//------------------------------------------------------------------------------
#endif
    