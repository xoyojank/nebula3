#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::VibrationFX

    A gamepad vibration effect.

    A vibrationMixer calcs the final values and controls the pads.

    (C) 2005 Radon Labs GmbH
*/
#include "fx/rt/effects/effect.h"

//------------------------------------------------------------------------------
namespace FX
{
class VibrationFX : public Effect
{
	__DeclareClass(VibrationFX);
public:
    /// constructor
    VibrationFX();

    /// set high freq duration
    void SetHighFreqDuration(Timing::Time r);
    /// get high freq duration
    Timing::Time GetHighFreqDuration() const;

    /// set low freq duration
    void SetLowFreqDuration(Timing::Time r);
    /// get low freq duration
    Timing::Time GetLowFreqDuration() const;

    /// set low freq intensity
    void SetLowFreqIntensity(float r);
    /// get intensity
    float GetLowFreqIntensity() const;

    /// set high freq intensity
    void SetHighFreqIntensity(float r);
    /// get high freq intensity
    float GetHighFreqIntensity() const;

    /// set player index
    void SetPlayerIndex(IndexT index);
    /// get player index
    IndexT GetPlayerIndex() const;

    /// start the effect
    virtual void OnStart(Timing::Time time);
    /// trigger the effect
    virtual void OnFrame(Timing::Time time);

    /// get current low freq intensity, valid after OnFrame has been called
    float GetCurrentHighFreqIntensity() const;
    /// get current low freq intensity, valid after OnFrame has been called
    float GetCurrentLowFreqIntensity() const;

private:
    IndexT playerIndex;

    float highFreqIntensity;
    float lowFreqIntensity;
    Timing::Time highFreqDuration;
    Timing::Time lowFreqDuration;

    float curHighFreqIntensity;
    float curLowFreqIntensity;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
VibrationFX::SetPlayerIndex(IndexT index)
{
    this->playerIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline
IndexT
VibrationFX::GetPlayerIndex() const
{
    return this->playerIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
VibrationFX::GetCurrentHighFreqIntensity() const
{
    return this->curHighFreqIntensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
VibrationFX::GetCurrentLowFreqIntensity() const
{
    return this->curLowFreqIntensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
VibrationFX::SetHighFreqDuration(Timing::Time r)
{
    this->highFreqDuration = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
VibrationFX::GetHighFreqDuration() const
{
    return this->highFreqDuration;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
VibrationFX::SetLowFreqDuration(Timing::Time r)
{
    this->lowFreqDuration = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
VibrationFX::GetLowFreqDuration() const
{
    return this->lowFreqDuration;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
VibrationFX::SetHighFreqIntensity(float r)
{
    this->highFreqIntensity = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
VibrationFX::GetHighFreqIntensity() const
{
    return this->highFreqIntensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
VibrationFX::SetLowFreqIntensity(float r)
{
    this->lowFreqIntensity = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
VibrationFX::GetLowFreqIntensity() const
{
    return this->lowFreqIntensity;
}
}; // namespace FX
//------------------------------------------------------------------------------