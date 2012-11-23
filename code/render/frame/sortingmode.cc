//------------------------------------------------------------------------------
//  sortingmode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/sortingmode.h"

namespace Frame
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
SortingMode::Code
SortingMode::FromString(const String& str)
{
    if (str == "None") return None;
    else if (str == "FrontToBack") return FrontToBack;
    else if (str == "BackToFront") return BackToFront;
    else
    {
        n_error("Frame::SortingMode::FromString(): invalid string '%s'!", str.AsCharPtr());
        return None;
    }
}

//------------------------------------------------------------------------------
/**
*/
String
SortingMode::ToString(Code c)
{
    switch (c)
    {
        case None:          return "None";
        case FrontToBack:   return "FrontToBack";
        case BackToFront:   return "BackToFront";
        default:
            n_error("Frame::SortingMode::ToString(): invalid code!");
            return "";
    }
}

} // namespace Frame
