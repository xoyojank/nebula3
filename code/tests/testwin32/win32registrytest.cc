//------------------------------------------------------------------------------
//  win32registrytest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "test/win32registrytest.h"
#include "system/win32/win32registry.h"

namespace Test
{
ImplementClass(Test::Win32RegistryTest, 'wrgt', Test::TestCase);

using namespace System;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
Win32RegistryTest::Run()
{
    const String key("Software\\RadonLabs\\Nebula3Test");

    // write a key
    this->Verify(Win32Registry::Write(Win32Registry::CurrentUser, key, "name1", "value1"));
    this->Verify(Win32Registry::Write(Win32Registry::CurrentUser, key, "name2", "value2"));

    // check that keys exist
    this->Verify(Win32Registry::Exists(Win32Registry::CurrentUser, key, ""));
    this->Verify(Win32Registry::Exists(Win32Registry::CurrentUser, key, "name1"));
    this->Verify(Win32Registry::Exists(Win32Registry::CurrentUser, key, "name2"));
    this->Verify(!Win32Registry::Exists(Win32Registry::CurrentUser, key, "bla"));

    // read values back
    this->Verify("value1" == Win32Registry::Read(Win32Registry::CurrentUser, key, "name1"));
    this->Verify("value2" == Win32Registry::Read(Win32Registry::CurrentUser, key, "name2"));

    // delete keys
    this->Verify(Win32Registry::Delete(Win32Registry::CurrentUser, key));
    this->Verify(!Win32Registry::Exists(Win32Registry::CurrentUser, key, ""));
}

} // namespace Test