//------------------------------------------------------------------------------
//  gamepad.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/gamepad.h"
#if (__WIN32__ || __XBOX360__)
namespace Input
{
__ImplementClass(Input::GamePad, 'GMPD', XInput::XInputGamePad);
}
#elif (__WII__)
namespace Input
{
__ImplementClass(Input::GamePad, 'GMPD', Wii::Wiimote);
}
#elif __PS3__
namespace Input
{
__ImplementClass(Input::GamePad, 'GMPD', PS3::PS3GamePad);
}
#else
#error "GamePad class not implemented on this platform!"
#endif
