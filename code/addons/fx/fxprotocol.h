#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "fx/rt/effects/effect.h"
#include "fx/rt/mixer/effectmixer.h"

//------------------------------------------------------------------------------
namespace FX
{
//------------------------------------------------------------------------------
class AttachEffect : public Messaging::Message
{
    __DeclareClass(AttachEffect);
    __DeclareMsgId;
public:
    AttachEffect() 
    { };
public:
    void SetEffect(const Ptr<FX::Effect>& val)
    {
        n_assert(!this->handled);
        this->effect = val;
    };
    const Ptr<FX::Effect>& GetEffect() const
    {
        return this->effect;
    };
private:
    Ptr<FX::Effect> effect;
};
//------------------------------------------------------------------------------
class AttachEffectMixer : public Messaging::Message
{
    __DeclareClass(AttachEffectMixer);
    __DeclareMsgId;
public:
    AttachEffectMixer() 
    { };
public:
    void SetEffectMixer(const Ptr<FX::EffectMixer>& val)
    {
        n_assert(!this->handled);
        this->effectmixer = val;
    };
    const Ptr<FX::EffectMixer>& GetEffectMixer() const
    {
        return this->effectmixer;
    };
private:
    Ptr<FX::EffectMixer> effectmixer;
};
} // namespace FX
//------------------------------------------------------------------------------
