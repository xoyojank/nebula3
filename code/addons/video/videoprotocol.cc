//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "videoprotocol.h"

namespace Video
{
    __ImplementClass(Video::PlayVideo, 'pvid', Messaging::Message);
    __ImplementMsgId(PlayVideo);
    __ImplementClass(Video::StopVideo, 'stvi', Messaging::Message);
    __ImplementMsgId(StopVideo);
    __ImplementClass(Video::PauseVideo, 'pavi', Messaging::Message);
    __ImplementMsgId(PauseVideo);
    __ImplementClass(Video::ResumeVideo, 'resv', Messaging::Message);
    __ImplementMsgId(ResumeVideo);
    __ImplementClass(Video::DeleteVideo, 'delv', Messaging::Message);
    __ImplementMsgId(DeleteVideo);
    __ImplementClass(Video::IsVideoPlaying, 'isvp', Messaging::Message);
    __ImplementMsgId(IsVideoPlaying);
} // Video

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
