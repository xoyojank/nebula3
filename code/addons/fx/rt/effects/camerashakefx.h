#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::CameraShakeFX

    A shake effect applies a shake shake to the world as long as it's alive.

    The FX::CameraShakeMixer should calculate the final shake values
    over all CameraShakeFX's and apply the modified transform to the current 
    camera

    (C) 2005 Radon Labs GmbH
*/
#include "fx/rt/effects/effect.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace FX
{
class CameraShakeFX : public Effect
{
	__DeclareClass(CameraShakeFX);
public:
    /// constructor
    CameraShakeFX();

    /// set range
    void SetRange(float r);
    /// get range
    float GetRange() const;

    /// set intensity
    void SetIntensity(const Math::vector& i);
    /// get intensity
    const Math::vector& GetIntensity() const;

    /// set additional rotation intensity
    void SetRotation(const Math::vector& r);
    /// get additional rotation intensity
    const Math::vector& GetRotation() const;

    /// set additional rotation intensity
    void SetSpeed(float s);
    /// get additional rotation intensity
    float GetSpeed() const;

    /// start the effect
    virtual void OnStart(Timing::Time time);
    /// trigger the effect
    virtual void OnFrame(Timing::Time time);

    /// get current intensity, valid after Update has been called
    const Math::vector& GetCurrentIntensity() const;
    /// get current rotation intensity, valid after Update has been called
    const Math::vector& GetCurrentRotation() const;

private:
    float range;
    Math::vector intensity;
    Math::vector rotation;
    Math::vector curIntensity;     // current intensity, updated by Trigger
    Math::vector curRotation;     // current rotation, updated by Trigger
    float speed;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetSpeed(float s)
{
    this->speed = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
CameraShakeFX::GetSpeed() const
{
    return this->speed;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetRange(float r)
{
    this->range = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
CameraShakeFX::GetRange() const
{
    return this->range;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetIntensity(const Math::vector& i)
{
    this->intensity = i;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
CameraShakeFX::GetIntensity() const
{
    return this->intensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
CameraShakeFX::GetCurrentIntensity() const
{
    return this->curIntensity;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraShakeFX::SetRotation(const Math::vector& r)
{
    this->rotation = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
CameraShakeFX::GetRotation() const
{
    return this->rotation;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
CameraShakeFX::GetCurrentRotation() const
{
    return this->curRotation;
}
}; // namespace FX
//------------------------------------------------------------------------------