#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::CameraAnimationFX

    This effect type applies an animation to the active camera.

    (C) 2009 Radon Labs GmbH
*/
#include "fx/rt/effects/effect.h"
#include "timing/timer.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"

//------------------------------------------------------------------------------
namespace FX
{
class CameraAnimationFX: public Effect
{
	__DeclareClass(CameraAnimationFX);
public:
    /// constructor
    CameraAnimationFX();
    /// deconstructor
    virtual ~CameraAnimationFX();

    /// activate the effect
    virtual void OnActivate(Timing::Time time);
    /// start the effect, called by OnFrame() after delay is over
    virtual void OnStart(Timing::Time time);
    /// trigger the effect
    virtual void OnFrame(Timing::Time time);
    /// deactivate the effect
    virtual void OnDeactivate();

    /// set animation filepath
    void SetAnimationFilePath(const Util::StringAtom& path);
    /// set the animation positions relative
    void SetAnimationRelative(bool b);

private:
    /// setup animation resource
    bool SetupAnimation();
    /// discard animation resource
    void DiscardAnimation();
    /// start playback
    void Play(Timing::Time time);
    /// stop playback
    void Stop(Timing::Time time);

    Util::StringAtom animPath;
    bool isRelative;
    bool isPlaying;

    Math::matrix44 initialTransform;
    Timing::Time animTime;
    Timing::Time animStartTime;
    Timing::Time animEndTime;
    Ptr<CoreAnimation::AnimResource> animResource;
    Ptr<CoreAnimation::AnimSampleBuffer> result;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
CameraAnimationFX::SetAnimationFilePath(const Util::StringAtom &path)
{
    this->animPath = path;
}
}; // namespace FX
//------------------------------------------------------------------------------