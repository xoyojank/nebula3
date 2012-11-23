#pragma once
//------------------------------------------------------------------------------
/**
    @class RakNet::RakNetInternalPlayer
        
    (C) 2009 Radon Labs GmbH
*/
#include "internalmultiplayer/base/internalplayerbase.h"          
#include "internalmultiplayer/packetpriority.h"
#include "util/ringbuffer.h"
#include "framesync/framesynctimer.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RakNetInternalMultiplayerServer;

class RakNetInternalPlayer : public Base::InternalPlayerBase
{
    __DeclareClass(RakNetInternalPlayer);
public:
    /// constructor
    RakNetInternalPlayer();
    /// destructor
    virtual ~RakNetInternalPlayer();

    /// set sign in state, updates out unique id on change
    void SetSignInState(Base::InternalPlayerBase::SignInState val);
    /// get Started	
    bool GetStarted() const;
    /// set Started
    void SetStarted(const bool val);                                               
    /// on join session 
    virtual void OnJoinSession(InternalMultiplayer::InternalSession* sessionPtr);

 #if NEBULA3_ENABLE_PROFILING     
    class StatisticValue
    {
    public:
        // constructor
        StatisticValue();
        // constrcutor with description
        StatisticValue(const Util::String& desc);
        // add value with timestamp
        void Add(int value);
        // values as array
        Util::Array<int> ValuesAsArray() const;
        // timestamps as array
        Util::Array<Timing::Tick> TimeStampsAsArray() const;
        // get description
        const Util::String& GetDescription() const;
    private:
        friend class RakNetInternalPlayer;
        Util::String description;
        Util::RingBuffer<Util::KeyValuePair<Timing::Tick, int> > values;
    };        
    Util::Dictionary<Util::StringAtom, StatisticValue> debugCounterSet;

    /// get counter set to fill by RakNetInternalMultiplayerServer
    Util::Dictionary<Util::StringAtom, StatisticValue>& GetDebugCounterSet();
#endif   

private:  

    friend class RakNetInternalMultiplayerServer;
    bool started;
};

#if NEBULA3_ENABLE_PROFILING  
//------------------------------------------------------------------------------
/**
*/
inline 
RakNetInternalPlayer::StatisticValue::StatisticValue()
{
    values.SetCapacity(50);
}     
//------------------------------------------------------------------------------
/**
*/
inline
RakNetInternalPlayer::StatisticValue::StatisticValue(const Util::String& desc)
{
    this->description = desc;
    values.SetCapacity(50);
}
//------------------------------------------------------------------------------
/**
*/
inline const Util::String& 
RakNetInternalPlayer::StatisticValue::GetDescription() const
{
    return this->description;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
RakNetInternalPlayer::StatisticValue::Add(int value)
{
    Timing::Tick curTime = FrameSync::FrameSyncTimer::Instance()->GetTicks();
    Util::KeyValuePair<Timing::Tick, int> newEntry(curTime, value);
    this->values.Add(newEntry);
}    

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<int> 
RakNetInternalPlayer::StatisticValue::ValuesAsArray() const
{
    Util::Array<int> intValues;
    IndexT i;
    for (i = 0; i < this->values.Size(); ++i)
    {
        intValues.Append(this->values[i].Value());    	
    }
    return intValues;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<Timing::Tick> 
RakNetInternalPlayer::StatisticValue::TimeStampsAsArray() const
{       
    Util::Array<Timing::Tick> timeStamps;
    IndexT i;
    for (i = 0; i < this->values.Size(); ++i)
    {
        timeStamps.Append(this->values[i].Key());    	
    }
    return timeStamps;

}      

//------------------------------------------------------------------------------
/**
*/
inline Util::Dictionary<Util::StringAtom, RakNetInternalPlayer::StatisticValue>& 
RakNetInternalPlayer::GetDebugCounterSet()
{
    return this->debugCounterSet;
} 
#endif

//------------------------------------------------------------------------------
/**
*/
inline void 
RakNetInternalPlayer::SetSignInState(Base::InternalPlayerBase::SignInState val)
{
    this->signInState = val;
}
//------------------------------------------------------------------------------
/**
*/
inline bool 
RakNetInternalPlayer::GetStarted() const
{
    return this->started;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void RakNetInternalPlayer::SetStarted(const bool val)
{
    this->started = val;
}  
} // namespace RakNet
//------------------------------------------------------------------------------
