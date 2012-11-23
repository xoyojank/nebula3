#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/array.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "input/inputevent.h"
#include "sui/suievent.h"
#include "sui/suiactions.h"

//------------------------------------------------------------------------------
namespace SUI
{
//------------------------------------------------------------------------------
class CreateLayout : public Messaging::Message
{
    __DeclareClass(CreateLayout);
    __DeclareMsgId;
public:
    CreateLayout() 
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
};
//------------------------------------------------------------------------------
class DestroyLayout : public Messaging::Message
{
    __DeclareClass(DestroyLayout);
    __DeclareMsgId;
public:
    DestroyLayout() 
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
};
//------------------------------------------------------------------------------
class ShowLayout : public Messaging::Message
{
    __DeclareClass(ShowLayout);
    __DeclareMsgId;
public:
    ShowLayout() :
        zprio(1)
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
public:
    void SetZPrio(int val)
    {
        n_assert(!this->handled);
        this->zprio = val;
    };
    int GetZPrio() const
    {
        return this->zprio;
    };
private:
    int zprio;
};
//------------------------------------------------------------------------------
class HideLayout : public Messaging::Message
{
    __DeclareClass(HideLayout);
    __DeclareMsgId;
public:
    HideLayout() 
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
};
//------------------------------------------------------------------------------
class ProcessEvents : public Messaging::Message
{
    __DeclareClass(ProcessEvents);
    __DeclareMsgId;
public:
    ProcessEvents() 
    { };
public:
    void SetInputEvents(const Util::Array<Input::InputEvent>& val)
    {
        n_assert(!this->handled);
        this->inputevents = val;
    };
    const Util::Array<Input::InputEvent>& GetInputEvents() const
    {
        return this->inputevents;
    };
private:
    Util::Array<Input::InputEvent> inputevents;
public:
    void SetSUIEvents(const Util::Array<SUI::SUIEvent>& val)
    {
        n_assert(!this->handled);
        this->suievents = val;
    };
    const Util::Array<SUI::SUIEvent>& GetSUIEvents() const
    {
        n_assert(this->handled);
        return this->suievents;
    };
private:
    Util::Array<SUI::SUIEvent> suievents;
};
//------------------------------------------------------------------------------
class InvokeAction : public Messaging::Message
{
    __DeclareClass(InvokeAction);
    __DeclareMsgId;
public:
    InvokeAction() 
    { };
public:
    void SetAction(const Ptr<SUI::SUIAction>& val)
    {
        n_assert(!this->handled);
        this->action = val;
    };
    const Ptr<SUI::SUIAction>& GetAction() const
    {
        return this->action;
    };
private:
    Ptr<SUI::SUIAction> action;
};
} // namespace SUI
//------------------------------------------------------------------------------
