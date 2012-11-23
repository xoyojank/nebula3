//------------------------------------------------------------------------------
//  script/attributes.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "scriptfeature/scriptattr/scriptattributes.h"

namespace Attr
{
    // *** dialog attributes ***

	// table '_Story_Dialogs'
    DefineGuid(DialogGUID, 'DUID', ReadWrite);
    DefineString(DialogId, 'DIID', ReadWrite);
    DefineString(DialogSpeaker, 'DISP', ReadWrite);
    DefineString(DialogGroup, 'DIGR', ReadWrite);
    DefineBoolWithDefault(DialogLocked, 'DILO', ReadWrite, true);
    DefineBoolWithDefault(DialogAsConversation, 'DIAC', ReadWrite, false);

	// table '_Story_DialogTakes'
    DefineGuid(TakeGUID, 'TUID', ReadWrite);
    DefineString(TakeId, 'TAID', ReadWrite);
    DefineString(TakeSpeaker, 'TASP', ReadWrite);
    DefineString(TakeType, 'TATY', ReadWrite);
    DefineString(TakeEmote, 'TAEM', ReadWrite);
    DefineString(TakeSound, 'TASN', ReadWrite);
    DefineBool(TakeStatus, 'TAST', ReadWrite);
    DefineBool(TakeSoundActiv, 'TASA', ReadWrite);
    DefineFloat(TakeTimeStamp, 'TATS', ReadWrite);
    DefineGuid(TakeLocaId, 'TALI', ReadWrite);
    DefineGuid(TakeShortTextLocaId, 'TSTL', ReadWrite);

	// table '_Story_DialogTakeChildMapping'
    DefineString(TakeChildId, 'TACI', ReadWrite);
    DefineInt(TakeChildPosition, 'TACP', ReadWrite);

	// table _Scripts_Conditions
    DefineGuid(ConditionGUID, 'COID', ReadWrite);
    DefineString(ConditionType, 'COTY', ReadWrite);
    DefineString(ConditionContent, 'COCO', ReadWrite);
    DefineGuid(ConditionBlockGUID, 'COBI', ReadWrite);
    DefineGuid(ConditionStatementGUID, 'COSI', ReadWrite);
    DefineGuid(ConditionRef, 'CORE', ReadWrite);
    DefineGuid(ConditionBlock, 'COBL', ReadWrite);

    // table _Scripts_Statements
    DefineGuid(StatementGUID, 'STID', ReadWrite);
    DefineString(StatementType, 'STTY', ReadWrite);
    DefineString(StatementContent, 'STCO', ReadWrite);
    DefineGuid(StatementBlock, 'STBL', ReadWrite);
    DefineGuid(StatementRef, 'STRF', ReadWrite);
    DefineGuid(StatementBlockGUID, 'STBI', ReadWrite);
    DefineString(ScriptName, 'SCNA', ReadWrite);

	// table '_Story_DialogActions'
    DefineString(ActionContent, 'ACCO', ReadWrite);

	// *** quest attributes ***

    // task attributes
    DefineGuid(QuestGuid, 'QUID', ReadWrite);
    DefineGuid(TaskGUID, 'TGID', ReadWrite);
    DefineString(TaskId, 'TASI', ReadWrite);
    DefineString(TaskState, 'TASS', ReadWrite);
    DefineBool(TaskViewed, 'TASV', ReadWrite);
    DefineGuid(TaskParentGuid, 'TAPG', ReadWrite);

    DefineGuid(CloseConditionBlock, 'CCBL', ReadWrite);
    DefineGuid(CloseConditionRef, 'CCRF', ReadWrite);
    DefineGuid(OpenConditionBlock, 'OCBL', ReadWrite);
    DefineGuid(OpenConditionRef, 'OCRF', ReadWrite);
    DefineGuid(FailConditionBlock, 'FCBL', ReadWrite);
    DefineGuid(FailConditionRef, 'FCRF', ReadWrite);

    DefineGuid(OpenStatementBlock, 'OSBL', ReadWrite);
    DefineGuid(CloseStatementBlock, 'CSBL', ReadWrite);
    DefineGuid(FailStatementBlock, 'FSBL', ReadWrite);
    DefineGuid(OpenStatementRef, 'OSRF', ReadWrite);
    DefineGuid(CloseStatementRef, 'CSRF', ReadWrite);
    DefineGuid(FailStatementRef, 'FSRF', ReadWrite);

	DefineString(TaskTargetEntity, 'TTEN', ReadWrite);
    DefineGuid(TaskTitleLocaId, 'TTLI', ReadWrite);
    DefineGuid(TaskOpenTextLocaId, 'TOTL', ReadWrite);
    DefineGuid(TaskCloseTextLocaId, 'TCTL', ReadWrite);
    DefineGuid(TaskFailedTextLocaId, 'TFTL', ReadWrite);

    DefineString(StateName, 'STNA', ReadWrite)
    DefineGuid(OnEntryStatementRef, 'STER', ReadWrite);
    DefineGuid(OnEntryStatementBlock, 'ESTB', ReadWrite);
    DefineGuid(OnFrameStatementRef, 'STFR', ReadWrite);
    DefineGuid(OnFrameStatementBlock, 'STFB', ReadWrite);
    DefineGuid(OnExitStatementRef, 'SEXR', ReadWrite);
    DefineGuid(OnExitStatementBlock, 'SEXB', ReadWrite);
    DefineString(MachineName, 'MANA', ReadWrite);
    DefineString(StartState, 'STST', ReadWrite);
    DefineString(ToState, 'TOST', ReadWrite);         
};

