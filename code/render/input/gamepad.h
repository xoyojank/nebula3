#pragma once
//------------------------------------------------------------------------------
/**
    @class Input::GamePad
  
    An input handler which represents a game pad.
    
    (C) 2007 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__)
#include "input/xinput/xinputgamepad.h"
namespace Input
{
class GamePad : public XInput::XInputGamePad
{
    __DeclareClass(GamePad);
};
}
#elif (__WII__)
#include "input/wii/wiimote.h"
namespace Input
{
class GamePad : public Wii::Wiimote
{
    __DeclareClass(GamePad);
};
}
#elif __PS3__
#include "input/ps3/ps3gamepad.h"
namespace Input
{
class GamePad : public PS3::PS3GamePad
{
    __DeclareClass(GamePad);
public:
	virtual ~GamePad() {}
};
}
#else
#error "GamePad class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

