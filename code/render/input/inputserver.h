#pragma once
//------------------------------------------------------------------------------
/**
    @class Input::InputServer
    
    The InputServer is the central object of the Input subsystem. It 
    mainly manages a prioritized list of input handlers which process
    incoming input events.

    (C) 2007 Radon Labs GmbH
*/
#include "core/singleton.h"
#if __WIN32__
#include "input/win32/win32inputserver.h"
namespace Input
{
class InputServer : public Win32::Win32InputServer
{
    __DeclareClass(InputServer);
    __DeclareInterfaceSingleton(InputServer);
public:
    /// constructor
    InputServer();
    /// destructor
    virtual ~InputServer();
};
}
#elif __XBOX360__
#include "input/xbox360/xbox360inputserver.h"
namespace Input
{
class InputServer : public Xbox360::Xbox360InputServer
{
    __DeclareClass(InputServer);
    __DeclareInterfaceSingleton(InputServer);
public:
    /// constructor
    InputServer();
    /// destructor
    virtual ~InputServer();
};
}
#elif __WII__
#include "input/wii/wiiinputserver.h"
namespace Input
{
class InputServer : public Wii::WiiInputServer
{
    __DeclareClass(InputServer);
    __DeclareInterfaceSingleton(InputServer);
public:
    /// constructor
    InputServer();
    /// destructor
    virtual ~InputServer();
};
}
#elif __PS3__
#include "input/ps3/ps3inputserver.h"
namespace Input
{
class InputServer : public PS3::PS3InputServer
{
    __DeclareClass(InputServer);
    __DeclareInterfaceSingleton(InputServer);
public:
    /// constructor
	InputServer();
    /// destructor
	virtual ~InputServer();
};
}
#else
#error "InputServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------


   