//------------------------------------------------------------------------------
//  win360filetime.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/win360/win360filetime.h"

namespace Win360
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Win360FileTime::Win360FileTime(const String& str)
{
    Array<String> tokens = str.Tokenize("#");
    n_assert(tokens.Size() == 2);
    this->time.dwHighDateTime = tokens[0].AsInt();
    this->time.dwLowDateTime = tokens[1].AsInt();
}

//------------------------------------------------------------------------------
/**
*/
String
Win360FileTime::AsString() const
{
    String str;
    str.Format("%d#%d", this->time.dwHighDateTime, this->time.dwLowDateTime);
    return str;
}

} // namespace Win360