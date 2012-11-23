//------------------------------------------------------------------------------
//  mouse.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/mouse.h"
#if __WIN32__
namespace Input
{
__ImplementClass(Input::Mouse, 'MOUS', Win32::Win32Mouse);
}
#elif __XBOX360__
namespace Input
{
__ImplementClass(Input::Mouse, 'MOUS', Xbox360::Xbox360Mouse);
}
#elif __WII__
namespace Input
{
__ImplementClass(Input::Mouse, 'MOUS', Wii::WiiMouse);
}
#elif __PS3__
namespace Input
{
__ImplementClass(Input::Mouse, 'MOUS', Base::MouseBase);
}
#else
#error "Mouse class not implemented on this platform!"
#endif
