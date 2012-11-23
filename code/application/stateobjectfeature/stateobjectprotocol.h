#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "properties/stateinfo.h"

//------------------------------------------------------------------------------
namespace StateObjectFeature
{
//------------------------------------------------------------------------------
class ContainsState : public Messaging::Message
{
    __DeclareClass(ContainsState);
    __DeclareMsgId;
public:
    ContainsState() 
    { };
public:
    void SetStateName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->statename = val;
    };
    const Util::String& GetStateName() const
    {
        return this->statename;
    };
private:
    Util::String statename;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetActiveState : public Messaging::Message
{
    __DeclareClass(GetActiveState);
    __DeclareMsgId;
public:
    GetActiveState() 
    { };
public:
    void SetStateName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->statename = val;
    };
    const Util::String& GetStateName() const
    {
        return this->statename;
    };
private:
    Util::String statename;
};
//------------------------------------------------------------------------------
class InternalSwitchActiveState : public Messaging::Message
{
    __DeclareClass(InternalSwitchActiveState);
    __DeclareMsgId;
public:
    InternalSwitchActiveState() 
    { };
public:
    void SetStateName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->statename = val;
    };
    const Util::String& GetStateName() const
    {
        return this->statename;
    };
private:
    Util::String statename;
};
//------------------------------------------------------------------------------
class SwitchActiveState : public Messaging::Message
{
    __DeclareClass(SwitchActiveState);
    __DeclareMsgId;
public:
    SwitchActiveState() 
    { };
public:
    void SetStateName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->statename = val;
    };
    const Util::String& GetStateName() const
    {
        return this->statename;
    };
private:
    Util::String statename;
};
//------------------------------------------------------------------------------
class Resources : public Messaging::Message
{
    __DeclareClass(Resources);
    __DeclareMsgId;
public:
    Resources() 
    { };
public:
    void SetResources(const Util::Dictionary<Util::String, StateInfo>& val)
    {
        n_assert(!this->handled);
        this->resources = val;
    };
    const Util::Dictionary<Util::String, StateInfo>& GetResources() const
    {
        return this->resources;
    };
private:
    Util::Dictionary<Util::String, StateInfo> resources;
};
} // namespace StateObjectFeature
//------------------------------------------------------------------------------
