#ifndef STORY_ATTRIBUTES_H
#define STORY_ATTRIBUTES_H
//------------------------------------------------------------------------------
/**
    @class Script::Attributes
  
    Declare attributes used by the story subsystem system.
    
    (C) 2005 Radon Labs GmbH
*/
#include "attr/attrid.h"
#include "attr/attributedefinition.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // *** dialog attributes ***

	// table '_Story_Dialogs'
    DeclareGuid(DialogGUID, 'DUID', ReadWrite);
    DeclareString(DialogId, 'DIID', ReadWrite);
    DeclareString(DialogSpeaker, 'DISP', ReadWrite);
    DeclareString(DialogGroup, 'DIGR', ReadWrite);
    DeclareBool(DialogLocked, 'DILO', ReadWrite);
    DeclareBool(DialogAsConversation, 'DIAC', ReadWrite);

	// table '_Story_DialogTakes'
    DeclareGuid(TakeGUID, 'TUID', ReadWrite);
    DeclareString(TakeId, 'TAID', ReadWrite);
    DeclareString(TakeSpeaker, 'TASP', ReadWrite);
    DeclareString(TakeType, 'TATY', ReadWrite);
    DeclareString(TakeEmote, 'TAEM', ReadWrite);
    DeclareString(TakeSound, 'TASN', ReadWrite);
    DeclareBool(TakeStatus, 'TAST', ReadWrite);
    DeclareBool(TakeSoundActiv, 'TASA', ReadWrite);
    DeclareFloat(TakeTimeStamp, 'TATS', ReadWrite);
    DeclareGuid(TakeLocaId, 'TALI', ReadWrite);
    DeclareGuid(TakeShortTextLocaId, 'TSTL', ReadWrite);

	// table '_Story_DialogTakeChildMapping'
    DeclareString(TakeChildId, 'TACI', ReadWrite);
    DeclareInt(TakeChildPosition, 'TACP', ReadWrite);

	// table _Scripts_Conditions
    DeclareGuid(ConditionGUID, 'COID', ReadWrite);
    DeclareString(ConditionType, 'COTY', ReadWrite);
    DeclareString(ConditionContent, 'COCO', ReadWrite);
    DeclareGuid(ConditionBlockGUID, 'COBI', ReadWrite);
    DeclareGuid(ConditionStatementGUID, 'COSI', ReadWrite);
    DeclareGuid(ConditionRef, 'CORE', ReadWrite);
    DeclareGuid(ConditionBlock, 'COBL', ReadWrite);

    // table _Scripts_Statements
    DeclareGuid(StatementGUID, 'STID', ReadWrite);
    DeclareString(StatementType, 'STTY', ReadWrite);
    DeclareString(StatementContent, 'STCO', ReadWrite);
    DeclareGuid(StatementBlock, 'STBL', ReadWrite);
    DeclareGuid(StatementRef, 'STRF', ReadWrite);
    DeclareGuid(StatementBlockGUID, 'STBI', ReadWrite);
    DeclareString(ScriptName, 'SCNA', ReadWrite);

	// table '_Story_DialogActions'
    DeclareString(ActionContent, 'ACCO', ReadWrite);

	// *** quest attributes ***

    // task attributes
    DeclareGuid(QuestGuid, 'QUID', ReadWrite);
    DeclareGuid(TaskGUID, 'TGID', ReadWrite);
    DeclareString(TaskId, 'TASI', ReadWrite);
    DeclareString(TaskState, 'TASS', ReadWrite);
    DeclareBool(TaskViewed, 'TASV', ReadWrite);
    DeclareGuid(TaskParentGuid, 'TAPG', ReadWrite);

    DeclareGuid(CloseConditionBlock, 'CCBL', ReadWrite);
    DeclareGuid(CloseConditionRef, 'CCRF', ReadWrite);
    DeclareGuid(OpenConditionBlock, 'OCBL', ReadWrite);
    DeclareGuid(OpenConditionRef, 'OCRF', ReadWrite);
    DeclareGuid(FailConditionBlock, 'FCBL', ReadWrite);
    DeclareGuid(FailConditionRef, 'FCRF', ReadWrite);

    DeclareGuid(OpenStatementBlock, 'OSBL', ReadWrite);
    DeclareGuid(CloseStatementBlock, 'CSBL', ReadWrite);
    DeclareGuid(FailStatementBlock, 'FSBL', ReadWrite);
    DeclareGuid(OpenStatementRef, 'OSRF', ReadWrite);
    DeclareGuid(CloseStatementRef, 'CSRF', ReadWrite);
    DeclareGuid(FailStatementRef, 'FSRF', ReadWrite);

	DeclareString(TaskTargetEntity, 'TTEN', ReadWrite);
    DeclareGuid(TaskTitleLocaId, 'TTLI', ReadWrite);
    DeclareGuid(TaskOpenTextLocaId, 'TOTL', ReadWrite);
    DeclareGuid(TaskCloseTextLocaId, 'TCTL', ReadWrite);
    DeclareGuid(TaskFailedTextLocaId, 'TFTL', ReadWrite);

    DeclareString(StateName, 'STNA', ReadWrite)
    DeclareGuid(OnEntryStatementRef, 'STER', ReadWrite);
    DeclareGuid(OnEntryStatementBlock, 'ESTB', ReadWrite);
    DeclareGuid(OnFrameStatementRef, 'STFR', ReadWrite);
    DeclareGuid(OnFrameStatementBlock, 'STFB', ReadWrite);
    DeclareGuid(OnExitStatementRef, 'SEXR', ReadWrite);
    DeclareGuid(OnExitStatementBlock, 'SEXB', ReadWrite);
    DeclareString(MachineName, 'MANA', ReadWrite);
    DeclareString(StartState, 'STST', ReadWrite);
    DeclareString(ToState, 'TOST', ReadWrite);         
};
//------------------------------------------------------------------------------
#endif
