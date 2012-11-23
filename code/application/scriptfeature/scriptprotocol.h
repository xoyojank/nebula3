#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/string.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace ScriptFeature
{
//------------------------------------------------------------------------------
class StartDialog : public Messaging::Message
{
    __DeclareClass(StartDialog);
    __DeclareMsgId;
public:
    StartDialog() 
    { };
public:
    void SetSpeaker(const Util::String& val)
    {
        n_assert(!this->handled);
        this->speaker = val;
    };
    const Util::String& GetSpeaker() const
    {
        return this->speaker;
    };
private:
    Util::String speaker;
};
//------------------------------------------------------------------------------
class ChooseResponse : public Messaging::Message
{
    __DeclareClass(ChooseResponse);
    __DeclareMsgId;
public:
    ChooseResponse() 
    { };
public:
    void SetResponse(int val)
    {
        n_assert(!this->handled);
        this->response = val;
    };
    int GetResponse() const
    {
        return this->response;
    };
private:
    int response;
};
//------------------------------------------------------------------------------
class CancelDialog : public Messaging::Message
{
    __DeclareClass(CancelDialog);
    __DeclareMsgId;
public:
    CancelDialog() 
    { };
};
//------------------------------------------------------------------------------
class SwitchFSM : public Messaging::Message
{
    __DeclareClass(SwitchFSM);
    __DeclareMsgId;
public:
    SwitchFSM() 
    { };
public:
    void SetStateMachineName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->statemachinename = val;
    };
    const Util::String& GetStateMachineName() const
    {
        return this->statemachinename;
    };
private:
    Util::String statemachinename;
};
//------------------------------------------------------------------------------
class GetCurrentState : public Messaging::Message
{
    __DeclareClass(GetCurrentState);
    __DeclareMsgId;
public:
    GetCurrentState() 
    { };
public:
    void SetStateName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->statename = val;
    };
    const Util::StringAtom& GetStateName() const
    {
        return this->statename;
    };
private:
    Util::StringAtom statename;
};
} // namespace ScriptFeature
//------------------------------------------------------------------------------
