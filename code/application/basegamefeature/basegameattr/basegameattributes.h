#pragma once
//------------------------------------------------------------------------------
/**
    @file basegameattr/basegameattributes.h    

    This is the central attribute registry for this feature. For more information
    on attributes, see Attr::Attribute.

    (C) 2007 Radon Labs GmbH
*/
#include "attr/attrid.h"
#include "attr/attributedefinition.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareFloat(EntityTriggerRadius, 'ETRR', ReadWrite);

    DeclareFloat(TestFloatAttr, 'TFLT', ReadWrite);
    DeclareInt(TestIntAttr, 'TINT', ReadWrite);
    DeclareString(TestStringAttr, 'TSTR', ReadWrite);    
    DeclareFloat4(TestFloat4Attr, 'TFL4', ReadWrite);
    DeclareBool(TestBoolAttr, 'TBOL', ReadWrite);
    
    DeclareString(_Type, '_TYP', ReadOnly);
    DeclareString(_Level, '_LEV', ReadOnly);
    DeclareString(_Layers, '_LAY', ReadOnly);
    DeclareString(_ID, '_ID_', ReadOnly);
    DeclareGuid(Guid, 'GUID', ReadWrite);
    DeclareString(Id, 'IDEN', ReadWrite);
    DeclareString(Name, 'NAME', ReadWrite);
    DeclareString(Placeholder, 'PLHO', ReadWrite);
    DeclareMatrix44(Transform, 'TRAN', ReadWrite);

    DeclareString(Dummy, 'DUMY', ReadWrite);
    DeclareBool(StartLevel, 'STLV', ReadWrite);    
    DeclareString(CurrentLevel, 'CLEV', ReadWrite);
    DeclareFloat(Time, 'TIME', ReadWrite);
}; // namespace Attr
//------------------------------------------------------------------------------