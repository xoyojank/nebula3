#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "internalmultiplayer/handle.h"
#include "internalmultiplayer/internalplayer.h"
#include "internalmultiplayer/netstream.h"
#include "internalmultiplayer/sessioninfo.h"
#include "internalmultiplayer/playerinfo.h"
#include "util/array.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Multiplayer
{
//------------------------------------------------------------------------------
class ReturnNotification : public Messaging::Message
{
    __DeclareClass(ReturnNotification);
    __DeclareMsgId;
public:
    ReturnNotification() 
    { };
};
//------------------------------------------------------------------------------
class LocalPlayerAdded : public Messaging::Message
{
    __DeclareClass(LocalPlayerAdded);
    __DeclareMsgId;
public:
    LocalPlayerAdded() 
    { };
public:
    void SetPlayerInfo(const InternalMultiplayer::PlayerInfo& val)
    {
        n_assert(!this->handled);
        this->playerinfo = val;
    };
    const InternalMultiplayer::PlayerInfo& GetPlayerInfo() const
    {
        return this->playerinfo;
    };
private:
    InternalMultiplayer::PlayerInfo playerinfo;
};
//------------------------------------------------------------------------------
class LocalPlayerRemoved : public Messaging::Message
{
    __DeclareClass(LocalPlayerRemoved);
    __DeclareMsgId;
public:
    LocalPlayerRemoved() 
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
class CreatePlayersSucceeded : public ReturnNotification
{
    __DeclareClass(CreatePlayersSucceeded);
    __DeclareMsgId;
public:
    CreatePlayersSucceeded() 
    { };
public:
    void SetPlayerInfo(const Util::Dictionary<IndexT, InternalMultiplayer::PlayerInfo>& val)
    {
        n_assert(!this->handled);
        this->playerinfo = val;
    };
    const Util::Dictionary<IndexT, InternalMultiplayer::PlayerInfo>& GetPlayerInfo() const
    {
        return this->playerinfo;
    };
private:
    Util::Dictionary<IndexT, InternalMultiplayer::PlayerInfo> playerinfo;
};
//------------------------------------------------------------------------------
class CreatePlayersFailed : public ReturnNotification
{
    __DeclareClass(CreatePlayersFailed);
    __DeclareMsgId;
public:
    CreatePlayersFailed() 
    { };
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class HostSessionSucceeded : public ReturnNotification
{
    __DeclareClass(HostSessionSucceeded);
    __DeclareMsgId;
public:
    HostSessionSucceeded() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
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
class HostSessionFailed : public ReturnNotification
{
    __DeclareClass(HostSessionFailed);
    __DeclareMsgId;
public:
    HostSessionFailed() 
    { };
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class StartGameSearchFinished : public ReturnNotification
{
    __DeclareClass(StartGameSearchFinished);
    __DeclareMsgId;
public:
    StartGameSearchFinished() 
    { };
public:
    void SetResults(const Util::Array<InternalMultiplayer::SessionInfo>& val)
    {
        n_assert(!this->handled);
        this->results = val;
    };
    const Util::Array<InternalMultiplayer::SessionInfo>& GetResults() const
    {
        return this->results;
    };
private:
    Util::Array<InternalMultiplayer::SessionInfo> results;
};
//------------------------------------------------------------------------------
class JoinSessionSucceeded : public ReturnNotification
{
    __DeclareClass(JoinSessionSucceeded);
    __DeclareMsgId;
public:
    JoinSessionSucceeded() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
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
class JoinSessionFailed : public ReturnNotification
{
    __DeclareClass(JoinSessionFailed);
    __DeclareMsgId;
public:
    JoinSessionFailed() 
    { };
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class StartGameSucceeded : public ReturnNotification
{
    __DeclareClass(StartGameSucceeded);
    __DeclareMsgId;
public:
    StartGameSucceeded() 
    { };
};
//------------------------------------------------------------------------------
class StartGameFailed : public ReturnNotification
{
    __DeclareClass(StartGameFailed);
    __DeclareMsgId;
public:
    StartGameFailed() 
    { };
};
//------------------------------------------------------------------------------
class EndGameSucceeded : public ReturnNotification
{
    __DeclareClass(EndGameSucceeded);
    __DeclareMsgId;
public:
    EndGameSucceeded() 
    { };
};
//------------------------------------------------------------------------------
class EndGameFailed : public ReturnNotification
{
    __DeclareClass(EndGameFailed);
    __DeclareMsgId;
public:
    EndGameFailed() 
    { };
};
//------------------------------------------------------------------------------
class LeaveSessionSucceeded : public ReturnNotification
{
    __DeclareClass(LeaveSessionSucceeded);
    __DeclareMsgId;
public:
    LeaveSessionSucceeded() 
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
class LeaveSessionFailed : public ReturnNotification
{
    __DeclareClass(LeaveSessionFailed);
    __DeclareMsgId;
public:
    LeaveSessionFailed() 
    { };
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class DeleteSessionSucceded : public ReturnNotification
{
    __DeclareClass(DeleteSessionSucceded);
    __DeclareMsgId;
public:
    DeleteSessionSucceded() 
    { };
};
//------------------------------------------------------------------------------
class DeleteSessionFailed : public ReturnNotification
{
    __DeclareClass(DeleteSessionFailed);
    __DeclareMsgId;
public:
    DeleteSessionFailed() 
    { };
public:
    void SetErrorMsg(const Util::String& val)
    {
        n_assert(!this->handled);
        this->errormsg = val;
    };
    const Util::String& GetErrorMsg() const
    {
        return this->errormsg;
    };
private:
    Util::String errormsg;
};
//------------------------------------------------------------------------------
class PlayerSignInChanged : public Messaging::Message
{
    __DeclareClass(PlayerSignInChanged);
    __DeclareMsgId;
public:
    PlayerSignInChanged() 
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
public:
    void SetSignInState(const InternalMultiplayer::InternalPlayer::SignInState& val)
    {
        n_assert(!this->handled);
        this->signinstate = val;
    };
    const InternalMultiplayer::InternalPlayer::SignInState& GetSignInState() const
    {
        return this->signinstate;
    };
private:
    InternalMultiplayer::InternalPlayer::SignInState signinstate;
};
//------------------------------------------------------------------------------
class PlayerJoinedSession : public Messaging::Message
{
    __DeclareClass(PlayerJoinedSession);
    __DeclareMsgId;
public:
    PlayerJoinedSession() 
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
public:
    void SetPlayerName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->playername = val;
    };
    const Util::StringAtom& GetPlayerName() const
    {
        return this->playername;
    };
private:
    Util::StringAtom playername;
public:
    void SetIsPlayerReady(bool val)
    {
        n_assert(!this->handled);
        this->isplayerready = val;
    };
    bool GetIsPlayerReady() const
    {
        return this->isplayerready;
    };
private:
    bool isplayerready;
public:
    void SetUnqiueIdBlob(const Util::Blob& val)
    {
        n_assert(!this->handled);
        this->unqiueidblob = val;
    };
    const Util::Blob& GetUnqiueIdBlob() const
    {
        return this->unqiueidblob;
    };
private:
    Util::Blob unqiueidblob;
};
//------------------------------------------------------------------------------
class PlayerLeftSession : public Messaging::Message
{
    __DeclareClass(PlayerLeftSession);
    __DeclareMsgId;
public:
    PlayerLeftSession() 
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
class DisconnectFromServer : public Messaging::Message
{
    __DeclareClass(DisconnectFromServer);
    __DeclareMsgId;
public:
    DisconnectFromServer() 
    { };
};
//------------------------------------------------------------------------------
class GameStarting : public Messaging::Message
{
    __DeclareClass(GameStarting);
    __DeclareMsgId;
public:
    GameStarting() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
};
//------------------------------------------------------------------------------
class GameStarted : public Messaging::Message
{
    __DeclareClass(GameStarted);
    __DeclareMsgId;
public:
    GameStarted() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
};
//------------------------------------------------------------------------------
class GameEnding : public Messaging::Message
{
    __DeclareClass(GameEnding);
    __DeclareMsgId;
public:
    GameEnding() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
};
//------------------------------------------------------------------------------
class GameEnded : public Messaging::Message
{
    __DeclareClass(GameEnded);
    __DeclareMsgId;
public:
    GameEnded() 
    { };
public:
    void SetSessionHandle(const InternalMultiplayer::SessionHandle& val)
    {
        n_assert(!this->handled);
        this->sessionhandle = val;
    };
    const InternalMultiplayer::SessionHandle& GetSessionHandle() const
    {
        return this->sessionhandle;
    };
private:
    InternalMultiplayer::SessionHandle sessionhandle;
};
//------------------------------------------------------------------------------
class PlayerReadyChanged : public Messaging::Message
{
    __DeclareClass(PlayerReadyChanged);
    __DeclareMsgId;
public:
    PlayerReadyChanged() 
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
public:
    void SetReadyState(bool val)
    {
        n_assert(!this->handled);
        this->readystate = val;
    };
    bool GetReadyState() const
    {
        return this->readystate;
    };
private:
    bool readystate;
};
//------------------------------------------------------------------------------
class DataReceived : public Messaging::Message
{
    __DeclareClass(DataReceived);
    __DeclareMsgId;
public:
    DataReceived() 
    { };
public:
    void SetFromPlayerHandle(const InternalMultiplayer::PlayerHandle& val)
    {
        n_assert(!this->handled);
        this->fromplayerhandle = val;
    };
    const InternalMultiplayer::PlayerHandle& GetFromPlayerHandle() const
    {
        return this->fromplayerhandle;
    };
private:
    InternalMultiplayer::PlayerHandle fromplayerhandle;
public:
    void SetToPlayer(const InternalMultiplayer::PlayerHandle& val)
    {
        n_assert(!this->handled);
        this->toplayer = val;
    };
    const InternalMultiplayer::PlayerHandle& GetToPlayer() const
    {
        return this->toplayer;
    };
private:
    InternalMultiplayer::PlayerHandle toplayer;
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
};
} // namespace Multiplayer
//------------------------------------------------------------------------------
