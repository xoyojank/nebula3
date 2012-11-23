//------------------------------------------------------------------------------
//  keyboardbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/base/keyboardbase.h"
#include "input/inputserver.h"

namespace Base
{
__ImplementClass(Base::KeyboardBase, 'KBBS', Input::InputHandler);

using namespace Input;

//------------------------------------------------------------------------------
/**
*/
KeyboardBase::KeyboardBase() :
    keyStates(Key::NumKeyCodes)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
KeyboardBase::~KeyboardBase()
{
    n_assert(!this->IsAttached());
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::BeginCapture()
{
    InputHandler::BeginCapture();
    InputServer::Instance()->ObtainKeyboardCapture(this);
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::EndCapture()
{
    InputHandler::EndCapture();
    InputServer::Instance()->ReleaseKeyboardCapture(this);
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::OnAttach()
{
    InputHandler::OnAttach();
    KeyState initialKeyState;
    this->keyStates.Fill(initialKeyState);
    this->charInput.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::OnBeginFrame()
{
    InputHandler::OnBeginFrame();

    // reset the up and down state of all keys
    IndexT i;
    for (i = 0; i < this->keyStates.Size(); i++)
    {
        KeyState& keyState = this->keyStates[i];
        
        // NOTE: if the key was released in the previous frame,
        // clear the pressed state (see the KeyUp-handling
        // code in OnEvent for details why this is a good thing)
        if (keyState.up)
        {
            keyState.pressed = false;
        }
        keyState.down = false;
        keyState.up = false;
    }

    // clear character input
    this->charInput.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
KeyboardBase::OnEvent(const InputEvent& inputEvent)
{
    switch (inputEvent.GetType())
    {
        // reset input handler if another one begins to capture
        case InputEvent::BeginKeyboardCapture:
            if (!this->IsCapturing())
            {
                this->OnReset();
            }
            break;

        case InputEvent::KeyDown:
            // note: if we're already pressed we don't record
            // a key down, this happens when the key repeat kicks in
            {
                KeyState& keyState = this->keyStates[inputEvent.GetKey()];
                if (!keyState.pressed)
                {
                    keyState.down = true;
                    keyState.pressed = true;
                }
            }
            break;

        case InputEvent::KeyUp:
            {
                KeyState& keyState = this->keyStates[inputEvent.GetKey()];
                keyState.up = true;
                // NOTE: we don't clear the pressed flag here because
                // it may happen that a key was only tapped shortly
                // within a frame, and in this case we still want the
                // pressed flag to be set for the current frame (otherwise an application
                // may miss the pressed state) instead, the pressed state will
                // be cleared at the beginning of the next frame
                // when the key up flag was set
            }
            break;

        case InputEvent::Character:
            {
                uchar chr = inputEvent.GetChar();
                this->charInput.AppendRange((const char*)&chr, 1);
            }
            break;

		default:
			break;
    }
    return InputHandler::OnEvent(inputEvent);
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::OnObtainCapture()
{
    InputHandler::OnObtainCapture();

    // put begin keyboard capture event
    InputEvent beginCaptureEvent;
    beginCaptureEvent.SetType(InputEvent::BeginKeyboardCapture);
    InputServer::Instance()->PutEvent(beginCaptureEvent);
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::OnReleaseCapture()
{
    // put end keyboard capture event
    InputEvent endCaptureEvent;
    endCaptureEvent.SetType(InputEvent::EndKeyboardCapture);
    InputServer::Instance()->PutEvent(endCaptureEvent);

    InputHandler::OnReleaseCapture();
}

//------------------------------------------------------------------------------
/**
*/
void
KeyboardBase::OnReset()
{
    IndexT i;
    for (i = 0; i < this->keyStates.Size(); i++)
    {
        KeyState& keyState = this->keyStates[i];
        if (keyState.pressed)
        {
            keyState.up = true;
        }
        else
        {
            keyState.up = false;
        }
        keyState.down = false;
        keyState.pressed = false;
    }
    this->charInput.Clear();
}

} // namespace Base
