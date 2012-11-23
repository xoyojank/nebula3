#pragma once
#ifndef STATEINFO_H
#define STATEINFO_H
//------------------------------------------------------------------------------
/**
    @class StateObjectFeature::StateInfo
    
    (C) 2008 Radon Labs GmbH
*/

namespace StateObjectFeature
{
class StateInfo
{
public:
    /// constructor
    //StateInfo() : stateDuration(0.0f) { };

    Util::String soundName;
    float stateDuration;
    Util::Dictionary<Util::String, Util::String> transitions;       // key is target state, value is transition state
};

};//namespace StaticObjectFeature

#endif