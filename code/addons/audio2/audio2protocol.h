#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "io/uri.h"
#include "util/array.h"
#include "math/matrix44.h"
#include "math/vector.h"
#include "audio2/fmodeventprojectid.h"
#include "audio2/fmodeventid.h"
#include "audio2/fmodeventhandle.h"
#include "audio2/fmodeventstate.h"
#include "audio2/fmodeventcategoryid.h"
#include "audio2/fmodeventparameterid.h"
#include "audio2/fmodpitchunit.h"

//------------------------------------------------------------------------------
namespace Audio2
{
//------------------------------------------------------------------------------
class SetupAudio : public Messaging::Message
{
    __DeclareClass(SetupAudio);
    __DeclareMsgId;
public:
    SetupAudio() 
    { };
};
//------------------------------------------------------------------------------
class UpdateListener : public Messaging::Message
{
    __DeclareClass(UpdateListener);
    __DeclareMsgId;
public:
    UpdateListener() 
    { };
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
};
//------------------------------------------------------------------------------
class LoadEventProject : public Messaging::Message
{
    __DeclareClass(LoadEventProject);
    __DeclareMsgId;
public:
    LoadEventProject() 
    { };
public:
    void SetProjectId(const Audio2::FmodEventProjectId& val)
    {
        n_assert(!this->handled);
        this->projectid = val;
    };
    const Audio2::FmodEventProjectId& GetProjectId() const
    {
        return this->projectid;
    };
private:
    Audio2::FmodEventProjectId projectid;
};
//------------------------------------------------------------------------------
class UnloadEventProject : public Messaging::Message
{
    __DeclareClass(UnloadEventProject);
    __DeclareMsgId;
public:
    UnloadEventProject() 
    { };
public:
    void SetProjectId(const Audio2::FmodEventProjectId& val)
    {
        n_assert(!this->handled);
        this->projectid = val;
    };
    const Audio2::FmodEventProjectId& GetProjectId() const
    {
        return this->projectid;
    };
private:
    Audio2::FmodEventProjectId projectid;
};
//------------------------------------------------------------------------------
class EventPlayFireAndForget : public Messaging::Message
{
    __DeclareClass(EventPlayFireAndForget);
    __DeclareMsgId;
public:
    EventPlayFireAndForget() 
    { };
public:
    void SetEventId(const Audio2::FmodEventId& val)
    {
        n_assert(!this->handled);
        this->eventid = val;
    };
    const Audio2::FmodEventId& GetEventId() const
    {
        return this->eventid;
    };
private:
    Audio2::FmodEventId eventid;
public:
    void SetVolume(float val)
    {
        n_assert(!this->handled);
        this->volume = val;
    };
    float GetVolume() const
    {
        return this->volume;
    };
private:
    float volume;
};
//------------------------------------------------------------------------------
class EventPlayFireAndForget3D : public Messaging::Message
{
    __DeclareClass(EventPlayFireAndForget3D);
    __DeclareMsgId;
public:
    EventPlayFireAndForget3D() 
    { };
public:
    void SetEventId(const Audio2::FmodEventId& val)
    {
        n_assert(!this->handled);
        this->eventid = val;
    };
    const Audio2::FmodEventId& GetEventId() const
    {
        return this->eventid;
    };
private:
    Audio2::FmodEventId eventid;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
public:
    void SetVolume(float val)
    {
        n_assert(!this->handled);
        this->volume = val;
    };
    float GetVolume() const
    {
        return this->volume;
    };
private:
    float volume;
};
//------------------------------------------------------------------------------
class EventCategoryControlBase : public Messaging::Message
{
    __DeclareClass(EventCategoryControlBase);
    __DeclareMsgId;
public:
    EventCategoryControlBase() 
    { };
public:
    void SetCategoryId(const Audio2::FmodEventCategoryId& val)
    {
        n_assert(!this->handled);
        this->categoryid = val;
    };
    const Audio2::FmodEventCategoryId& GetCategoryId() const
    {
        return this->categoryid;
    };
private:
    Audio2::FmodEventCategoryId categoryid;
};
//------------------------------------------------------------------------------
class EventCategorySetVolume : public EventCategoryControlBase
{
    __DeclareClass(EventCategorySetVolume);
    __DeclareMsgId;
public:
    EventCategorySetVolume() 
    { };
public:
    void SetVolume(float val)
    {
        n_assert(!this->handled);
        this->volume = val;
    };
    float GetVolume() const
    {
        return this->volume;
    };
private:
    float volume;
};
//------------------------------------------------------------------------------
class EventCategorySetPaused : public EventCategoryControlBase
{
    __DeclareClass(EventCategorySetPaused);
    __DeclareMsgId;
public:
    EventCategorySetPaused() 
    { };
public:
    void SetPaused(bool val)
    {
        n_assert(!this->handled);
        this->paused = val;
    };
    bool GetPaused() const
    {
        return this->paused;
    };
private:
    bool paused;
};
//------------------------------------------------------------------------------
class EventCategorySetPitch : public EventCategoryControlBase
{
    __DeclareClass(EventCategorySetPitch);
    __DeclareMsgId;
public:
    EventCategorySetPitch() :
        pitchunit(Audio2::PitchUnitUnknown)
    { };
public:
    void SetPitch(float val)
    {
        n_assert(!this->handled);
        this->pitch = val;
    };
    float GetPitch() const
    {
        return this->pitch;
    };
private:
    float pitch;
public:
    void SetPitchUnit(const Audio2::FmodPitchUnit& val)
    {
        n_assert(!this->handled);
        this->pitchunit = val;
    };
    const Audio2::FmodPitchUnit& GetPitchUnit() const
    {
        return this->pitchunit;
    };
private:
    Audio2::FmodPitchUnit pitchunit;
};
//------------------------------------------------------------------------------
class EventCreate : public Messaging::Message
{
    __DeclareClass(EventCreate);
    __DeclareMsgId;
public:
    EventCreate() 
    { };
public:
    void SetEventId(const Audio2::FmodEventId& val)
    {
        n_assert(!this->handled);
        this->eventid = val;
    };
    const Audio2::FmodEventId& GetEventId() const
    {
        return this->eventid;
    };
private:
    Audio2::FmodEventId eventid;
public:
    void SetEventHandle(const Audio2::FmodEventHandle& val)
    {
        n_assert(!this->handled);
        this->eventhandle = val;
    };
    const Audio2::FmodEventHandle& GetEventHandle() const
    {
        n_assert(this->handled);
        return this->eventhandle;
    };
private:
    Audio2::FmodEventHandle eventhandle;
};
//------------------------------------------------------------------------------
class EventControlBase : public Messaging::Message
{
    __DeclareClass(EventControlBase);
    __DeclareMsgId;
public:
    EventControlBase() 
    { };
public:
    void SetEventHandle(const Audio2::FmodEventHandle& val)
    {
        n_assert(!this->handled);
        this->eventhandle = val;
    };
    const Audio2::FmodEventHandle& GetEventHandle() const
    {
        return this->eventhandle;
    };
private:
    Audio2::FmodEventHandle eventhandle;
};
//------------------------------------------------------------------------------
class EventUpdateState : public EventControlBase
{
    __DeclareClass(EventUpdateState);
    __DeclareMsgId;
public:
    EventUpdateState() :
        state(Audio2::EventStateUnknown)
    { };
public:
    void SetState(const Audio2::FmodEventState& val)
    {
        n_assert(!this->handled);
        this->state = val;
    };
    const Audio2::FmodEventState& GetState() const
    {
        n_assert(this->handled);
        return this->state;
    };
private:
    Audio2::FmodEventState state;
};
//------------------------------------------------------------------------------
class EventDiscard : public EventControlBase
{
    __DeclareClass(EventDiscard);
    __DeclareMsgId;
public:
    EventDiscard() 
    { };
};
//------------------------------------------------------------------------------
class EventPlay : public EventControlBase
{
    __DeclareClass(EventPlay);
    __DeclareMsgId;
public:
    EventPlay() 
    { };
};
//------------------------------------------------------------------------------
class EventPlayProgrammerSound : public EventControlBase
{
    __DeclareClass(EventPlayProgrammerSound);
    __DeclareMsgId;
public:
    EventPlayProgrammerSound() 
    { };
public:
    void SetName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::String& GetName() const
    {
        return this->name;
    };
private:
    Util::String name;
};
//------------------------------------------------------------------------------
class EventStop : public EventControlBase
{
    __DeclareClass(EventStop);
    __DeclareMsgId;
public:
    EventStop() 
    { };
};
//------------------------------------------------------------------------------
class EventPause : public EventControlBase
{
    __DeclareClass(EventPause);
    __DeclareMsgId;
public:
    EventPause() 
    { };
};
//------------------------------------------------------------------------------
class EventResume : public EventControlBase
{
    __DeclareClass(EventResume);
    __DeclareMsgId;
public:
    EventResume() 
    { };
};
//------------------------------------------------------------------------------
class EventUpdate3D : public EventControlBase
{
    __DeclareClass(EventUpdate3D);
    __DeclareMsgId;
public:
    EventUpdate3D() 
    { };
public:
    void SetPosition(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->position = val;
    };
    const Math::vector& GetPosition() const
    {
        return this->position;
    };
private:
    Math::vector position;
public:
    void SetVelocity(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->velocity = val;
    };
    const Math::vector& GetVelocity() const
    {
        return this->velocity;
    };
private:
    Math::vector velocity;
public:
    void SetHasOrientation(bool val)
    {
        n_assert(!this->handled);
        this->hasorientation = val;
    };
    bool GetHasOrientation() const
    {
        return this->hasorientation;
    };
private:
    bool hasorientation;
public:
    void SetOrientation(const Math::vector& val)
    {
        n_assert(!this->handled);
        this->orientation = val;
    };
    const Math::vector& GetOrientation() const
    {
        return this->orientation;
    };
private:
    Math::vector orientation;
};
//------------------------------------------------------------------------------
class EventSetParameter : public EventControlBase
{
    __DeclareClass(EventSetParameter);
    __DeclareMsgId;
public:
    EventSetParameter() 
    { };
public:
    void SetParameterId(const Audio2::FmodEventParameterId& val)
    {
        n_assert(!this->handled);
        this->parameterid = val;
    };
    const Audio2::FmodEventParameterId& GetParameterId() const
    {
        return this->parameterid;
    };
private:
    Audio2::FmodEventParameterId parameterid;
public:
    void SetValue(float val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    float GetValue() const
    {
        return this->value;
    };
private:
    float value;
};
//------------------------------------------------------------------------------
class EventSetVolume : public EventControlBase
{
    __DeclareClass(EventSetVolume);
    __DeclareMsgId;
public:
    EventSetVolume() 
    { };
public:
    void SetParameterId(const Audio2::FmodEventParameterId& val)
    {
        n_assert(!this->handled);
        this->parameterid = val;
    };
    const Audio2::FmodEventParameterId& GetParameterId() const
    {
        return this->parameterid;
    };
private:
    Audio2::FmodEventParameterId parameterid;
public:
    void SetVolume(float val)
    {
        n_assert(!this->handled);
        this->volume = val;
    };
    float GetVolume() const
    {
        return this->volume;
    };
private:
    float volume;
};
//------------------------------------------------------------------------------
class SetAuditioningEnabled : public Messaging::Message
{
    __DeclareClass(SetAuditioningEnabled);
    __DeclareMsgId;
public:
    SetAuditioningEnabled() 
    { };
public:
    void SetEnabled(bool val)
    {
        n_assert(!this->handled);
        this->enabled = val;
    };
    bool GetEnabled() const
    {
        return this->enabled;
    };
private:
    bool enabled;
};
//------------------------------------------------------------------------------
class CueBase : public Messaging::Message
{
    __DeclareClass(CueBase);
    __DeclareMsgId;
public:
    CueBase() 
    { };
public:
    void SetCueId(int val)
    {
        n_assert(!this->handled);
        this->cueid = val;
    };
    int GetCueId() const
    {
        return this->cueid;
    };
private:
    int cueid;
};
//------------------------------------------------------------------------------
class PlayCue : public CueBase
{
    __DeclareClass(PlayCue);
    __DeclareMsgId;
public:
    PlayCue() 
    { };
};
//------------------------------------------------------------------------------
class StopCue : public CueBase
{
    __DeclareClass(StopCue);
    __DeclareMsgId;
public:
    StopCue() 
    { };
};
} // namespace Audio2
//------------------------------------------------------------------------------
