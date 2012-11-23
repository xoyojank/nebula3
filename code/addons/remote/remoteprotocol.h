#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "remote/remoterequesthandler.h"
#include "remote/remotecommandhandler.h"

//------------------------------------------------------------------------------
namespace Remote
{
//------------------------------------------------------------------------------
class AttachRequestHandler : public Messaging::Message
{
    __DeclareClass(AttachRequestHandler);
    __DeclareMsgId;
public:
    AttachRequestHandler() 
    { };
public:
    void SetRequestHandler(const Ptr<Remote::RemoteRequestHandler>& val)
    {
        n_assert(!this->handled);
        this->requesthandler = val;
    };
    const Ptr<Remote::RemoteRequestHandler>& GetRequestHandler() const
    {
        return this->requesthandler;
    };
private:
    Ptr<Remote::RemoteRequestHandler> requesthandler;
};
//------------------------------------------------------------------------------
class RemoveRequestHandler : public Messaging::Message
{
    __DeclareClass(RemoveRequestHandler);
    __DeclareMsgId;
public:
    RemoveRequestHandler() 
    { };
public:
    void SetRequestHandler(const Ptr<Remote::RemoteRequestHandler>& val)
    {
        n_assert(!this->handled);
        this->requesthandler = val;
    };
    const Ptr<Remote::RemoteRequestHandler>& GetRequestHandler() const
    {
        return this->requesthandler;
    };
private:
    Ptr<Remote::RemoteRequestHandler> requesthandler;
};
//------------------------------------------------------------------------------
class AttachCommandHandler : public Messaging::Message
{
    __DeclareClass(AttachCommandHandler);
    __DeclareMsgId;
public:
    AttachCommandHandler() 
    { };
public:
    void SetCommandHandler(const Ptr<Remote::RemoteCommandHandler>& val)
    {
        n_assert(!this->handled);
        this->commandhandler = val;
    };
    const Ptr<Remote::RemoteCommandHandler>& GetCommandHandler() const
    {
        return this->commandhandler;
    };
private:
    Ptr<Remote::RemoteCommandHandler> commandhandler;
};
//------------------------------------------------------------------------------
class RemoveCommandHandler : public Messaging::Message
{
    __DeclareClass(RemoveCommandHandler);
    __DeclareMsgId;
public:
    RemoveCommandHandler() 
    { };
public:
    void SetCommandHandler(const Ptr<Remote::RemoteCommandHandler>& val)
    {
        n_assert(!this->handled);
        this->commandhandler = val;
    };
    const Ptr<Remote::RemoteCommandHandler>& GetCommandHandler() const
    {
        return this->commandhandler;
    };
private:
    Ptr<Remote::RemoteCommandHandler> commandhandler;
};
} // namespace Remote
//------------------------------------------------------------------------------
