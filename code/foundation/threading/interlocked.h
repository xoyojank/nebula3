#pragma once
//------------------------------------------------------------------------------
/**
    @class Threading::Interlocked
    
    Provide simple atomic operations on memory variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360interlocked.h"
namespace Threading
{
class Interlocked : public Win360::Win360Interlocked
{ };
}
#elif __WII__
#include "threading/wii/wiiinterlocked.h"
namespace Threading
{
class Interlocked : public Wii::WiiInterlocked
{ };
}
#elif __PS3__
#include "threading/ps3/ps3interlocked.h"
namespace Threading
{
class Interlocked : public PS3::PS3Interlocked
{ };
}
#elif __OSX__
#include "threading/osx/osxinterlocked.h"
namespace Threading
{
class Interlocked : public OSX::OSXInterlocked
{ };
}
#else
#error "Threading::Interlocked not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
