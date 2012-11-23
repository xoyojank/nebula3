//------------------------------------------------------------------------------
//  fx/effects/camerashakefx.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fx/rt/effects/camerashakefx.h"
#include "math/vector.h"

namespace FX
{
__ImplementClass(FX::CameraShakeFX, 'FXSH', FX::Effect);

//------------------------------------------------------------------------------
/**
*/
CameraShakeFX::CameraShakeFX() :
    range(1.0f),
    intensity(Math::vector::nullvec()),
    curIntensity(Math::vector::nullvec())
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
CameraShakeFX::OnStart(Timing::Time time)
{
    Effect::OnStart(time);
    this->curIntensity = this->intensity;
    this->curRotation = this->rotation;
}

//------------------------------------------------------------------------------
/**
*/
void
CameraShakeFX::OnFrame(Timing::Time time)
{
    Effect::OnFrame(time);

    if (this->IsPlaying())
    {
        // drop off shake intensity by time
        Timing::Time age = time - this->startTime;
        if (age <= this->GetDuration())
        {
            this->curIntensity = this->intensity * (1.0f - Math::n_saturate(float(age / this->duration)));
            this->curRotation = this->rotation * (1.0f - Math::n_saturate(float(age / this->duration)));
        }
    }
}
}; // namespace FX