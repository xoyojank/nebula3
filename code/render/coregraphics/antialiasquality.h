#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::AntiAliasQuality
    
    Anti-alias quality levels.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class AntiAliasQuality
{
public:
    /// enum
    enum Code
    {
        None,
        Low,
        Medium,
        High,
    };
    
    /// convert from string
    static Code FromString(const Util::String& str);
    /// convert to string
    static Util::String ToString(Code code);
};

} // namespace AntiAliasQuality
//------------------------------------------------------------------------------

    