//------------------------------------------------------------------------------
//  inputserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/inputserver.h"

namespace Input
{
#if __WIN32__
__ImplementClass(Input::InputServer, 'INPS', Win32::Win32InputServer);
__ImplementInterfaceSingleton(Input::InputServer);
#elif __XBOX360__
// FIXME!
__ImplementClass(Input::InputServer, 'INPS', Xbox360::Xbox360InputServer);
__ImplementInterfaceSingleton(Input::InputServer);
#elif __WII__
__ImplementClass(Input::InputServer, 'INPS', Wii::WiiInputServer);
__ImplementInterfaceSingleton(Input::InputServer);
#elif __PS3__
__ImplementClass(Input::InputServer, 'INPS', PS3::PS3InputServer);
__ImplementInterfaceSingleton(Input::InputServer);
#else
#error "InputServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
InputServer::InputServer()
{
    __ConstructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InputServer::~InputServer()
{
    __DestructInterfaceSingleton;
}

} // namespace Input
