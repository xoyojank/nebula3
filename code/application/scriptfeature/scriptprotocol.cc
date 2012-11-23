//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scriptprotocol.h"

namespace ScriptFeature
{
    __ImplementClass(ScriptFeature::StartDialog, 'STDI', Messaging::Message);
    __ImplementMsgId(StartDialog);
    __ImplementClass(ScriptFeature::ChooseResponse, 'CHOR', Messaging::Message);
    __ImplementMsgId(ChooseResponse);
    __ImplementClass(ScriptFeature::CancelDialog, 'CAND', Messaging::Message);
    __ImplementMsgId(CancelDialog);
    __ImplementClass(ScriptFeature::SwitchFSM, 'SFSM', Messaging::Message);
    __ImplementMsgId(SwitchFSM);
    __ImplementClass(ScriptFeature::GetCurrentState, 'GCST', Messaging::Message);
    __ImplementMsgId(GetCurrentState);
} // ScriptFeature

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
