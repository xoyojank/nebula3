#ifndef WIN32_WIN32MOUSE_H
#define WIN32_WIN32MOUSE_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Mouse
    
    Overrides the default Mouse input device class and provides raw
    mouse movement data via DirectInput. This is necessary because
    Windows WM_MOUSEMOVE messages stop at the screen border.
    
    (C) 2007 Radon Labs GmbH
*/
#include "input/base/mousebase.h"
#include "input/win32/win32inputserver.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Mouse : public Base::MouseBase
{
    __DeclareClass(Win32Mouse);
public:
    /// get mouse movement
    const Math::float2& GetMovement() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Math::float2&
Win32Mouse::GetMovement() const
{
    return Win32InputServer::Instance()->GetMouseMovement();
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif
    