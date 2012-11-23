#pragma once
//------------------------------------------------------------------------------
/**
    @class System::Win32Environment
    
    Provides read-access to environment variables. Useful for tools.
    NOTE: using this class restricts your code to the Win32 platform.    
    
    (C) 2009 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace System
{
class Win32Environment
{
public:
    /// return true if env-variable exists
    static bool Exists(const Util::String& envVarName);
    /// get value of existing env-variable
    static Util::String Read(const Util::String& envVarName);
};

} // namespace System
//------------------------------------------------------------------------------
    