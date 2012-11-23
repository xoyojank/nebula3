#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "internalmultiplayer/handle.h"
#include "internalmultiplayer/multiplayertype.h"
#include "internalmultiplayer/sessioninfo.h"
#include "internalmultiplayer/internalplayer.h"
#include "internalmultiplayer/netstream.h"
#include "internalmultiplayer/packetpriority.h"
#include "internalmultiplayer/packetchannel.h"
#include "multiplayer/base/multiplayernotificationhandlerbase.h"
#include "internalmultiplayer/base/parameterresolverbase.h"
#include "util/array.h"
#include "util/dictionary.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Multiplayer
{
//------------------------------------------------------------------------------
class PlayerMessage : public Messaging::Message
{
    __DeclareClass(PlayerMessage);
    __DeclareMsgId;
public:
    PlayerMessage() 
    { };
public:
    void SetPlayerHandle(const InternalMultiplayer::PlayerHandle& val)
    {
        n_assert(!this->handled);
        this->playerhandle = val;
    };
    const InternalMultiplayer::PlayerHandle& GetPlayerHandle() const
    {
        return this->playerhandle;
    };
private:
    InternalMultiplayer::PlayerHandle playerhandle;
};
//------------------------------------------------------------------------------
class NetworkEventBase : public Messaging::Message
{
    __DeclareClass(NetworkEventBase);
    __DeclareMsgId;
public:
    NetworkEventBase() 
    { };
public:
    void SetTimeStamp(int val)
    {
        n_assert(!this->handled);
        this->timestamp = val;
    };
    int GetTimeStamp() const
    {
        return this->timestamp;
    };
private:
    int timestamp;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteInt(this->GetTimeStamp());
        Messaging::Message::Encode(writer);
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetTimeStamp(reader->ReadInt());
        Messaging::Message::Decode(reader);
    };
};
//------------------------------------------------------------------------------
class CreatePlayers : public Messaging::Message
{
    __DeclareClass(CreatePlayers);
    __DeclareMsgId;
public:
    CreatePlayers() 
    { };
public:
    void SetMaxLocalPlayers(SizeT val)
    {
        n_assert(!this->handled);
        this->maxlocalplayers = val;
    };
    SizeT GetMaxLocalPlayers() const
    {
        return this->maxlocalplayers;
    };
private:
    SizeT maxlocalplayers;
};
//------------------------------------------------------------------------------
class AddLocalPlayer : public Messaging::Message
{
    __DeclareClass(AddLocalPlayer);
    __DeclareMsgId;
public:
    AddLocalPlayer() 
    { };
};
//------------------------------------------------------------------------------
class RemoveLocalPlayer : public Messaging::Message
{
    __DeclareClass(RemoveLocalPlayer);
    __DeclareMsgId;
public:
    RemoveLocalPlayer() 
    { };
public:
    void SetPlayerHandle(const InternalMultiplayer::PlayerHandle& val)
    {
        n_assert(!this->handled);
        this->playerhandle = val;
    };
    const InternalMultiplayer::PlayerHandle& GetPlayerHandle() const
    {
        return this->playerhandle;
    };
private:
    InternalMultiplayer::PlayerHandle playerhandle;
};
//------------------------------------------------------------------------------
class HostSession : public PlayerMessage
{
    __DeclareClass(HostSession);
    __DeclareMsgId;
public:
    HostSession() 
    { };
public:
    void SetLocalPlayers(const Util::Array<InternalMultiplayer::PlayerHandle>& val)
    {
        n_assert(!this->handled);
        this->localplayers = val;
    };
    const Util::Array<InternalMultiplayer::PlayerHandle>& GetLocalPlayers() const
    {
        return this->localplayers;
    };
private:
    Util::Array<InternalMultiplayer::PlayerHandle> localplayers;
public:
    void SetPublicSlots(ushort val)
    {
        n_assert(!this->handled);
        this->publicslots = val;
    };
    ushort GetPublicSlots() const
    {
        return this->publicslots;
    };
private:
    ushort publicslots;
public:
    void SetPrivateSlots(ushort val)
    {
        n_assert(!this->handled);
        this->privateslots = val;
    };
    ushort GetPrivateSlots() const
    {
        return this->privateslots;
    };
private:
    ushort privateslots;
public:
    void SetParameterResolver(const Ptr<Base::ParameterResolverBase>& val)
    {
        n_assert(!this->handled);
        this->parameterresolver = val;
    };
    const Ptr<Base::ParameterResolverBase>& GetParameterResolver() const
    {
        return this->parameterresolver;
    };
private:
    Ptr<Base::ParameterResolverBase> parameterresolver;
public:
    void SetMultiplayerType(const InternalMultiplayer::MultiplayerType::Code& val)
    {
        n_assert(!this->handled);
        this->multiplayertype = val;
    };
    const InternalMultiplayer::MultiplayerType::Code& GetMultiplayerType() const
    {
        return this->multiplayertype;
    };
private:
    InternalMultiplayer::MultiplayerType::Code multiplayertype;
};
//------------------------------------------------------------------------------
class StartGameSearch : public PlayerMessage
{
    __DeclareClass(StartGameSearch);
    __DeclareMsgId;
public:
    StartGameSearch() 
    { };
public:
    void SetLocalPlayers(const Util::Array<InternalMultiplayer::PlayerHandle>& val)
    {
        n_assert(!this->handled);
        this->localplayers = val;
    };
    const Util::Array<InternalMultiplayer::PlayerHandle>& GetLocalPlayers() const
    {
        return this->localplayers;
    };
private:
    Util::Array<InternalMultiplayer::PlayerHandle> localplayers;
public:
    void SetParameterResolver(const Ptr<Base::ParameterResolverBase>& val)
    {
        n_assert(!this->handled);
        this->parameterresolver = val;
    };
    const Ptr<Base::ParameterResolverBase>& GetParameterResolver() const
    {
        return this->parameterresolver;
    };
private:
    Ptr<Base::ParameterResolverBase> parameterresolver;
public:
    void SetMaxResults(SizeT val)
    {
        n_assert(!this->handled);
        this->maxresults = val;
    };
    SizeT GetMaxResults() const
    {
        return this->maxresults;
    };
private:
    SizeT maxresults;
public:
    void SetTimeout(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->timeout = val;
    };
    const Timing::Time& GetTimeout() const
    {
        return this->timeout;
    };
private:
    Timing::Time timeout;
};
//------------------------------------------------------------------------------
class JoinSession : public PlayerMessage
{
    __DeclareClass(JoinSession);
    __DeclareMsgId;
public:
    JoinSession() 
    { };
public:
    void SetLocalPlayers(const Util::Array<InternalMultiplayer::PlayerHandle>& val)
    {
        n_assert(!this->handled);
        this->localplayers = val;
    };
    const Util::Array<InternalMultiplayer::PlayerHandle>& GetLocalPlayers() const
    {
        return this->localplayers;
    };
private:
    Util::Array<InternalMultiplayer::PlayerHandle> localplayers;
public:
    void SetSessionInfo(const InternalMultiplayer::SessionInfo& val)
    {
        n_assert(!this->handled);
        this->sessioninfo = val;
    };
    const InternalMultiplayer::SessionInfo& GetSessionInfo() const
    {
        return this->sessioninfo;
    };
private:
    InternalMultiplayer::SessionInfo sessioninfo;
};
//------------------------------------------------------------------------------
class LeaveSession : public PlayerMessage
{
    __DeclareClass(LeaveSession);
    __DeclareMsgId;
public:
    LeaveSession() 
    { };
public:
    void SetLocalPlayers(const Util::Array<InternalMultiplayer::PlayerHandle>& val)
    {
        n_assert(!this->handled);
        this->localplayers = val;
    };
    const Util::Array<InternalMultiplayer::PlayerHandle>& GetLocalPlayers() const
    {
        return this->localplayers;
    };
private:
    Util::Array<InternalMultiplayer::PlayerHandle> localplayers;
};
//------------------------------------------------------------------------------
class DeleteSession : public Messaging::Message
{
    __DeclareClass(DeleteSession);
    __DeclareMsgId;
public:
    DeleteSession() 
    { };
};
//------------------------------------------------------------------------------
class SetPlayerReady : public PlayerMessage
{
    __DeclareClass(SetPlayerReady);
    __DeclareMsgId;
public:
    SetPlayerReady() 
    { };
public:
    void SetReady(bool val)
    {
        n_assert(!this->handled);
        this->ready = val;
    };
    bool GetReady() const
    {
        return this->ready;
    };
private:
    bool ready;
};
//------------------------------------------------------------------------------
class StartGame : public Messaging::Message
{
    __DeclareClass(StartGame);
    __DeclareMsgId;
public:
    StartGame() 
    { };
};
//------------------------------------------------------------------------------
class EndGame : public Messaging::Message
{
    __DeclareClass(EndGame);
    __DeclareMsgId;
public:
    EndGame() 
    { };
};
//------------------------------------------------------------------------------
class StartingGameFinished : public Messaging::Message
{
    __DeclareClass(StartingGameFinished);
    __DeclareMsgId;
public:
    StartingGameFinished() 
    { };
};
//------------------------------------------------------------------------------
class SendDataToPlayer : public PlayerMessage
{
    __DeclareClass(SendDataToPlayer);
    __DeclareMsgId;
public:
    SendDataToPlayer() 
    { };
public:
    void SetTargetPlayerHandle(const InternalMultiplayer::PlayerHandle& val)
    {
        n_assert(!this->handled);
        this->targetplayerhandle = val;
    };
    const InternalMultiplayer::PlayerHandle& GetTargetPlayerHandle() const
    {
        return this->targetplayerhandle;
    };
private:
    InternalMultiplayer::PlayerHandle targetplayerhandle;
public:
    void SetStream(const Ptr<InternalMultiplayer::NetStream>& val)
    {
        n_assert(!this->handled);
        this->stream = val;
    };
    const Ptr<InternalMultiplayer::NetStream>& GetStream() const
    {
        return this->stream;
    };
private:
    Ptr<InternalMultiplayer::NetStream> stream;
public:
    void SetReliable(bool val)
    {
        n_assert(!this->handled);
        this->reliable = val;
    };
    bool GetReliable() const
    {
        return this->reliable;
    };
private:
    bool reliable;
public:
    void SetSequential(bool val)
    {
        n_assert(!this->handled);
        this->sequential = val;
    };
    bool GetSequential() const
    {
        return this->sequential;
    };
private:
    bool sequential;
public:
    void SetOrdered(bool val)
    {
        n_assert(!this->handled);
        this->ordered = val;
    };
    bool GetOrdered() const
    {
        return this->ordered;
    };
private:
    bool ordered;
public:
    void SetDirect(bool val)
    {
        n_assert(!this->handled);
        this->direct = val;
    };
    bool GetDirect() const
    {
        return this->direct;
    };
private:
    bool direct;
public:
    void SetChannel(const InternalMultiplayer::PacketChannel::Code& val)
    {
        n_assert(!this->handled);
        this->channel = val;
    };
    const InternalMultiplayer::PacketChannel::Code& GetChannel() const
    {
        return this->channel;
    };
private:
    InternalMultiplayer::PacketChannel::Code channel;
public:
    void SetPriority(const InternalMultiplayer::PacketPriority::Code& val)
    {
        n_assert(!this->handled);
        this->priority = val;
    };
    const InternalMultiplayer::PacketPriority::Code& GetPriority() const
    {
        return this->priority;
    };
private:
    InternalMultiplayer::PacketPriority::Code priority;
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        n_assert(this->handled);
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class BroadcastData : public Messaging::Message
{
    __DeclareClass(BroadcastData);
    __DeclareMsgId;
public:
    BroadcastData() 
    { };
public:
    void SetStream(const Ptr<InternalMultiplayer::NetStream>& val)
    {
        n_assert(!this->handled);
        this->stream = val;
    };
    const Ptr<InternalMultiplayer::NetStream>& GetStream() const
    {
        return this->stream;
    };
private:
    Ptr<InternalMultiplayer::NetStream> stream;
public:
    void SetReliable(bool val)
    {
        n_assert(!this->handled);
        this->reliable = val;
    };
    bool GetReliable() const
    {
        return this->reliable;
    };
private:
    bool reliable;
public:
    void SetSequential(bool val)
    {
        n_assert(!this->handled);
        this->sequential = val;
    };
    bool GetSequential() const
    {
        return this->sequential;
    };
private:
    bool sequential;
public:
    void SetOrdered(bool val)
    {
        n_assert(!this->handled);
        this->ordered = val;
    };
    bool GetOrdered() const
    {
        return this->ordered;
    };
private:
    bool ordered;
public:
    void SetDirect(bool val)
    {
        n_assert(!this->handled);
        this->direct = val;
    };
    bool GetDirect() const
    {
        return this->direct;
    };
private:
    bool direct;
public:
    void SetChannel(const InternalMultiplayer::PacketChannel::Code& val)
    {
        n_assert(!this->handled);
        this->channel = val;
    };
    const InternalMultiplayer::PacketChannel::Code& GetChannel() const
    {
        return this->channel;
    };
private:
    InternalMultiplayer::PacketChannel::Code channel;
public:
    void SetPriority(const InternalMultiplayer::PacketPriority::Code& val)
    {
        n_assert(!this->handled);
        this->priority = val;
    };
    const InternalMultiplayer::PacketPriority::Code& GetPriority() const
    {
        return this->priority;
    };
private:
    InternalMultiplayer::PacketPriority::Code priority;
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        n_assert(this->handled);
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class SendDataToHost : public Messaging::Message
{
    __DeclareClass(SendDataToHost);
    __DeclareMsgId;
public:
    SendDataToHost() 
    { };
public:
    void SetStream(const Ptr<InternalMultiplayer::NetStream>& val)
    {
        n_assert(!this->handled);
        this->stream = val;
    };
    const Ptr<InternalMultiplayer::NetStream>& GetStream() const
    {
        return this->stream;
    };
private:
    Ptr<InternalMultiplayer::NetStream> stream;
public:
    void SetReliable(bool val)
    {
        n_assert(!this->handled);
        this->reliable = val;
    };
    bool GetReliable() const
    {
        return this->reliable;
    };
private:
    bool reliable;
public:
    void SetSequential(bool val)
    {
        n_assert(!this->handled);
        this->sequential = val;
    };
    bool GetSequential() const
    {
        return this->sequential;
    };
private:
    bool sequential;
public:
    void SetOrdered(bool val)
    {
        n_assert(!this->handled);
        this->ordered = val;
    };
    bool GetOrdered() const
    {
        return this->ordered;
    };
private:
    bool ordered;
public:
    void SetDirect(bool val)
    {
        n_assert(!this->handled);
        this->direct = val;
    };
    bool GetDirect() const
    {
        return this->direct;
    };
private:
    bool direct;
public:
    void SetChannel(const InternalMultiplayer::PacketChannel::Code& val)
    {
        n_assert(!this->handled);
        this->channel = val;
    };
    const InternalMultiplayer::PacketChannel::Code& GetChannel() const
    {
        return this->channel;
    };
private:
    InternalMultiplayer::PacketChannel::Code channel;
public:
    void SetPriority(const InternalMultiplayer::PacketPriority::Code& val)
    {
        n_assert(!this->handled);
        this->priority = val;
    };
    const InternalMultiplayer::PacketPriority::Code& GetPriority() const
    {
        return this->priority;
    };
private:
    InternalMultiplayer::PacketPriority::Code priority;
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        n_assert(this->handled);
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class AttachNotificationHandler : public Messaging::Message
{
    __DeclareClass(AttachNotificationHandler);
    __DeclareMsgId;
public:
    AttachNotificationHandler() 
    { };
public:
    void SetNotificationHandler(const Ptr<Base::MultiplayerNotificationHandlerBase>& val)
    {
        n_assert(!this->handled);
        this->notificationhandler = val;
    };
    const Ptr<Base::MultiplayerNotificationHandlerBase>& GetNotificationHandler() const
    {
        return this->notificationhandler;
    };
private:
    Ptr<Base::MultiplayerNotificationHandlerBase> notificationhandler;
};
//------------------------------------------------------------------------------
class RemoveNotificationHandler : public Messaging::Message
{
    __DeclareClass(RemoveNotificationHandler);
    __DeclareMsgId;
public:
    RemoveNotificationHandler() 
    { };
public:
    void SetNotificationHandler(const Ptr<Base::MultiplayerNotificationHandlerBase>& val)
    {
        n_assert(!this->handled);
        this->notificationhandler = val;
    };
    const Ptr<Base::MultiplayerNotificationHandlerBase>& GetNotificationHandler() const
    {
        return this->notificationhandler;
    };
private:
    Ptr<Base::MultiplayerNotificationHandlerBase> notificationhandler;
};
} // namespace Multiplayer
//------------------------------------------------------------------------------
