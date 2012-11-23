#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/string.h"
#include "util/array.h"
#include "threading/threadid.h"
#include "ui/rt/coreuielement.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
//------------------------------------------------------------------------------
class RenderUIElements : public Messaging::Message
{
    __DeclareClass(RenderUIElements);
    __DeclareMsgId;
public:
    RenderUIElements() 
    { };
public:
    void SetUIElements(const Util::Array<CoreUIElement>& val)
    {
        n_assert(!this->handled);
        this->uielements = val;
    };
    const Util::Array<CoreUIElement>& GetUIElements() const
    {
        return this->uielements;
    };
private:
    Util::Array<CoreUIElement> uielements;
};
//------------------------------------------------------------------------------
class LoadUITexture : public Messaging::Message
{
    __DeclareClass(LoadUITexture);
    __DeclareMsgId;
public:
    LoadUITexture() 
    { };
public:
    void SetResourceName(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Resources::ResourceId& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Resources::ResourceId resourcename;
};
//------------------------------------------------------------------------------
class UnloadUITexture : public Messaging::Message
{
    __DeclareClass(UnloadUITexture);
    __DeclareMsgId;
public:
    UnloadUITexture() 
    { };
public:
    void SetResourceName(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Resources::ResourceId& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Resources::ResourceId resourcename;
};
} // namespace CoreUI
//------------------------------------------------------------------------------
