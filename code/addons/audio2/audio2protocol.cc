//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio2protocol.h"

namespace Audio2
{
    __ImplementClass(Audio2::SetupAudio, 'saud', Messaging::Message);
    __ImplementMsgId(SetupAudio);
    __ImplementClass(Audio2::UpdateListener, 'updl', Messaging::Message);
    __ImplementMsgId(UpdateListener);
    __ImplementClass(Audio2::LoadEventProject, 'levr', Messaging::Message);
    __ImplementMsgId(LoadEventProject);
    __ImplementClass(Audio2::UnloadEventProject, 'uevp', Messaging::Message);
    __ImplementMsgId(UnloadEventProject);
    __ImplementClass(Audio2::EventPlayFireAndForget, 'epff', Messaging::Message);
    __ImplementMsgId(EventPlayFireAndForget);
    __ImplementClass(Audio2::EventPlayFireAndForget3D, 'epfd', Messaging::Message);
    __ImplementMsgId(EventPlayFireAndForget3D);
    __ImplementClass(Audio2::EventCategoryControlBase, 'eccb', Messaging::Message);
    __ImplementMsgId(EventCategoryControlBase);
    __ImplementClass(Audio2::EventCategorySetVolume, 'ecsv', EventCategoryControlBase);
    __ImplementMsgId(EventCategorySetVolume);
    __ImplementClass(Audio2::EventCategorySetPaused, 'ecsp', EventCategoryControlBase);
    __ImplementMsgId(EventCategorySetPaused);
    __ImplementClass(Audio2::EventCategorySetPitch, 'ecpi', EventCategoryControlBase);
    __ImplementMsgId(EventCategorySetPitch);
    __ImplementClass(Audio2::EventCreate, 'crev', Messaging::Message);
    __ImplementMsgId(EventCreate);
    __ImplementClass(Audio2::EventControlBase, 'ceba', Messaging::Message);
    __ImplementMsgId(EventControlBase);
    __ImplementClass(Audio2::EventUpdateState, 'evus', EventControlBase);
    __ImplementMsgId(EventUpdateState);
    __ImplementClass(Audio2::EventDiscard, 'evdi', EventControlBase);
    __ImplementMsgId(EventDiscard);
    __ImplementClass(Audio2::EventPlay, 'evpl', EventControlBase);
    __ImplementMsgId(EventPlay);
    __ImplementClass(Audio2::EventPlayProgrammerSound, 'epps', EventControlBase);
    __ImplementMsgId(EventPlayProgrammerSound);
    __ImplementClass(Audio2::EventStop, 'evst', EventControlBase);
    __ImplementMsgId(EventStop);
    __ImplementClass(Audio2::EventPause, 'evpa', EventControlBase);
    __ImplementMsgId(EventPause);
    __ImplementClass(Audio2::EventResume, 'evre', EventControlBase);
    __ImplementMsgId(EventResume);
    __ImplementClass(Audio2::EventUpdate3D, 'evud', EventControlBase);
    __ImplementMsgId(EventUpdate3D);
    __ImplementClass(Audio2::EventSetParameter, 'evsp', EventControlBase);
    __ImplementMsgId(EventSetParameter);
    __ImplementClass(Audio2::EventSetVolume, 'evsv', EventControlBase);
    __ImplementMsgId(EventSetVolume);
    __ImplementClass(Audio2::SetAuditioningEnabled, 'esae', Messaging::Message);
    __ImplementMsgId(SetAuditioningEnabled);
    __ImplementClass(Audio2::CueBase, 'cuba', Messaging::Message);
    __ImplementMsgId(CueBase);
    __ImplementClass(Audio2::PlayCue, 'plcu', CueBase);
    __ImplementMsgId(PlayCue);
    __ImplementClass(Audio2::StopCue, 'stcu', CueBase);
    __ImplementMsgId(StopCue);
} // Audio2

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
