#pragma once
//------------------------------------------------------------------------------
/**
    @class System::Win32Registry
    
    A simple wrapper class to access the Win32 registry.
    NOTE: using this class restricts your code to the Win32 platform.

    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace System
{
class Win32Registry
{
public:
    /// key enumeration
    enum RootKey
    {
        ClassesRoot,
        CurrentUser,
        LocalMachine,
        Users,
    };

    /// return true if a registry entry exists
    static bool Exists(RootKey rootKey, const Util::String& key, const Util::String& name);
    /// write a registry entry
    static bool WriteString(RootKey rootKey, const Util::String& key, const Util::String& name, const Util::String& value);
    /// read a string registry entry, the string will be UTF-8 encoded!
    static Util::String ReadString(RootKey rootKey, const Util::String& key, const Util::String& name);
    /// read an int registry entry
    static int ReadInt(RootKey rootKey, const Util::String& key, const Util::String& name);
    /// delete a registry key (and all its contained values)
    static bool Delete(RootKey rootKey, const Util::String& key);
    /// convert rootkey from string
    static RootKey AsRootKey(const Util::String& str);
private:
    /// convert RootKey enum into Win32 key handle
    static HKEY RootKeyToWin32KeyHandle(RootKey rootKey);
};

} // namespace System
//------------------------------------------------------------------------------
