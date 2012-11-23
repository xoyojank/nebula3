#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Vibration
{
//------------------------------------------------------------------------------
class ClearVibration : public Messaging::Message
{
    __DeclareClass(ClearVibration);
    __DeclareMsgId;
public:
    ClearVibration() :
        highfreq(true),
        lowfreq(true),
        releasetime(0)
    { };
public:
    void SetHighFreq(bool val)
    {
        n_assert(!this->handled);
        this->highfreq = val;
    };
    bool GetHighFreq() const
    {
        return this->highfreq;
    };
private:
    bool highfreq;
public:
    void SetLowFreq(bool val)
    {
        n_assert(!this->handled);
        this->lowfreq = val;
    };
    bool GetLowFreq() const
    {
        return this->lowfreq;
    };
private:
    bool lowfreq;
public:
    void SetPlayerIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->playerindex = val;
    };
    IndexT GetPlayerIndex() const
    {
        return this->playerindex;
    };
private:
    IndexT playerindex;
public:
    void SetReleaseTime(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->releasetime = val;
    };
    const Timing::Time& GetReleaseTime() const
    {
        return this->releasetime;
    };
private:
    Timing::Time releasetime;
};
//------------------------------------------------------------------------------
class AddEnvelopeVibration : public Messaging::Message
{
    __DeclareClass(AddEnvelopeVibration);
    __DeclareMsgId;
public:
    AddEnvelopeVibration() 
    { };
public:
    void SetPlayerIndex(IndexT val)
    {
        n_assert(!this->handled);
        this->playerindex = val;
    };
    IndexT GetPlayerIndex() const
    {
        return this->playerindex;
    };
private:
    IndexT playerindex;
public:
    void SetLowFreqAmount(float val)
    {
        n_assert(!this->handled);
        this->lowfreqamount = val;
    };
    float GetLowFreqAmount() const
    {
        return this->lowfreqamount;
    };
private:
    float lowfreqamount;
public:
    void SetLowFreqAttack(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->lowfreqattack = val;
    };
    const Timing::Time& GetLowFreqAttack() const
    {
        return this->lowfreqattack;
    };
private:
    Timing::Time lowfreqattack;
public:
    void SetLowFreqSustain(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->lowfreqsustain = val;
    };
    const Timing::Time& GetLowFreqSustain() const
    {
        return this->lowfreqsustain;
    };
private:
    Timing::Time lowfreqsustain;
public:
    void SetLowFreqRelease(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->lowfreqrelease = val;
    };
    const Timing::Time& GetLowFreqRelease() const
    {
        return this->lowfreqrelease;
    };
private:
    Timing::Time lowfreqrelease;
public:
    void SetHighFreqAmount(float val)
    {
        n_assert(!this->handled);
        this->highfreqamount = val;
    };
    float GetHighFreqAmount() const
    {
        return this->highfreqamount;
    };
private:
    float highfreqamount;
public:
    void SetHighFreqAttack(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->highfreqattack = val;
    };
    const Timing::Time& GetHighFreqAttack() const
    {
        return this->highfreqattack;
    };
private:
    Timing::Time highfreqattack;
public:
    void SetHighFreqSustain(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->highfreqsustain = val;
    };
    const Timing::Time& GetHighFreqSustain() const
    {
        return this->highfreqsustain;
    };
private:
    Timing::Time highfreqsustain;
public:
    void SetHighFreqRelease(const Timing::Time& val)
    {
        n_assert(!this->handled);
        this->highfreqrelease = val;
    };
    const Timing::Time& GetHighFreqRelease() const
    {
        return this->highfreqrelease;
    };
private:
    Timing::Time highfreqrelease;
};
} // namespace Vibration
//------------------------------------------------------------------------------
