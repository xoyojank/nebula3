//------------------------------------------------------------------------------
//  attr/attributes.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegameattr/basegameattributes.h"

namespace Attr
{
    DefineFloat(EntityTriggerRadius, 'ETRR', ReadWrite);
    
    DefineFloat(TestFloatAttr, 'TFLT', ReadWrite);
    DefineInt(TestIntAttr, 'TINT', ReadWrite);
    DefineString(TestStringAttr, 'TSTR', ReadWrite);    
    DefineFloat4(TestFloat4Attr, 'TFL4', ReadWrite);
    DefineBool(TestBoolAttr, 'TBOL', ReadWrite);

    DefineString(_Type, '_TYP', ReadOnly);
    DefineString(_Level, '_LEV', ReadOnly);
    DefineString(_Layers, '_LAY', ReadOnly);
    DefineString(_ID, '_ID_', ReadOnly);
    DefineGuid(Guid, 'GUID', ReadWrite);
    DefineString(Id, 'IDEN', ReadWrite);
    DefineString(Name, 'NAME', ReadWrite);
    DefineString(Placeholder, 'PLHO', ReadWrite);
    DefineMatrix44WithDefault(Transform, 'TRAN', ReadWrite, Math::matrix44::identity());
    DefineString(Dummy, 'DUMY', ReadWrite);
    DefineBool(StartLevel, 'STLV', ReadWrite);  
    DefineString(CurrentLevel, 'CLEV', ReadWrite);
    DefineFloat(Time, 'TIME', ReadWrite);   
};
