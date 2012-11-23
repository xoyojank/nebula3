#ifndef UI_MAINSCREEN_H
#define UI_MAINSCREEN_H
//------------------------------------------------------------------------------
/**
    @class UI::MainScreen
  
    The main menue screen. This window will be shown additionally to the 3D-scene
    in the background.

    (C) 2007 Radon Labs GmbH
*/
#include "ui/elements/window.h"

//------------------------------------------------------------------------------
namespace UI
{

class MainScreen : public Window
{
    __DeclareClass(MainScreen);    
public:
    /// constructor
    MainScreen();
    /// destructor
    virtual ~MainScreen();
    /// open the window
    virtual void Open();
    /// close the window
    virtual void Close();
    /// called to handle a gui event
    virtual void HandleEvent(Event* e);
    /// enable/disable input (needed for safe fade-out)
    void DisableInput();

private:
    bool inputDisabled;     // disable input while fading out
};

//------------------------------------------------------------------------------
/**
*/
inline void
MainScreen::DisableInput()
{
    this->inputDisabled = true;
}

};
//------------------------------------------------------------------------------
#endif
