//------------------------------------------------------------------------------
//  properties/mayacamerappoperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/mayacameraproperty.h"
#include "managers/focusmanager.h"
#include "graphics/cameraentity.h"
#include "game/entity.h"
#include "basegameprotocol.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "input/inputserver.h"
#include "basegametiming/inputtimesource.h"
#include "graphicsfeature/properties/inputproperty.h"

// video camera specific attributes
namespace Attr
{
    DefineFloat4(MayaCameraCenterOfInterest, 'MCCI', ReadWrite);
	DefineFloat4WithDefault(MayaCameraDefaultUpVec, 'MCDU', ReadWrite, Math::float4(0.0f, 1.0f, 0.0f, 1.0f));
};

namespace GraphicsFeature
{

using namespace Input;
using namespace Math;
using namespace BaseGameFeature;

__ImplementClass(GraphicsFeature::MayaCameraProperty, 'MCAM', GraphicsFeature::CameraProperty);

//------------------------------------------------------------------------------
/**
*/
MayaCameraProperty::MayaCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MayaCameraProperty::~MayaCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property.
*/
void
MayaCameraProperty::SetupDefaultAttributes()
{
    CameraProperty::SetupDefaultAttributes();
    SetupAttr(Attr::MayaCameraCenterOfInterest);
    SetupAttr(Attr::MayaCameraDefaultUpVec);   
    SetupAttr(Attr::InputFocus);
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::SetupAcceptedMessages()
{
	this->RegisterMessage(UpdateTransform::Id);
    this->RegisterMessage(InputFocus::Id);
	CameraProperty::SetupAcceptedMessages();
}
    
//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnObtainInputFocus()
{
    this->entity->SetBool(Attr::InputFocus, true);
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnLoseInputFocus()
{     
    this->entity->SetBool(Attr::InputFocus, false);
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(UpdateTransform::Id))
    {        
		this->cameraEntity->SetTransform(((UpdateTransform*)msg.get())->GetMatrix());
    }
    else if (msg->CheckId(InputFocus::Id))
    {
        InputFocus* focusMsg = msg.downcast<InputFocus>();
        if (focusMsg->GetObtainFocus())
        {
            this->OnObtainInputFocus();
        }
        else
        {
            this->OnLoseInputFocus();
        }
    }
    else
    {
        CameraProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnActivate()
{
    CameraProperty::OnActivate();

	// initialize the Maya camera object
	const matrix44& m = this->GetEntity()->GetMatrix44(Attr::Transform);
    
	// setup the camera util object
	this->mayaCameraUtil.Setup(	this->entity->GetFloat4(Attr::MayaCameraCenterOfInterest), 
								m.get_position(), 
								this->entity->GetFloat4(Attr::MayaCameraDefaultUpVec));
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnRender()
{
    // handle input even if we have only camera focus
    if (FocusManager::Instance()->GetInputFocusEntity() == GetEntity())
    {
        // feed the Maya camera object with input
		InputServer* inputServer = InputServer::Instance();
        const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
        const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);

        // standard input handling: manipulate camera
        this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());

        // process gamepad input
        float zoomIn = 0.0f;
        float zoomOut = 0.0f;
        float2 panning(0.0f, 0.0f);
        float2 orbiting(0.0f, 0.0f);
        if (gamePad->IsConnected())
        {
            const float gamePadZoomSpeed = 50.0f;
            const float gamePadOrbitSpeed = 10.0f;
            const float gamePadPanSpeed = 10.0f;
            if (gamePad->ButtonDown(GamePad::AButton))
            {
                this->mayaCameraUtil.Reset();
            }
            float frameTime = (float) InputTimeSource::Instance()->GetFrameTime();
            zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime * gamePadZoomSpeed;
            zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime * gamePadZoomSpeed;
            panning.x()  += gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime * gamePadPanSpeed;
            panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime * gamePadPanSpeed;
            orbiting.x() += gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime * gamePadOrbitSpeed;
            orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime * gamePadOrbitSpeed;
        }

        // handle keyboard input
        if (keyboard->KeyDown(Key::Space))
        {
            this->mayaCameraUtil.Reset();
        }
        if (keyboard->KeyPressed(Key::Left))
        {
            panning.x() -= 0.1f;
        }
        if (keyboard->KeyPressed(Key::Right))
        {
            panning.x() += 0.1f;
        }
        if (keyboard->KeyPressed(Key::Up))
        {
            panning.y() -= 0.1f;
        }
        if (keyboard->KeyPressed(Key::Down))
        {
            panning.y() += 0.1f;
        }

        // update panning, orbiting, zooming speeds
        this->mayaCameraUtil.SetPanning(panning);
        this->mayaCameraUtil.SetOrbiting(orbiting);
        this->mayaCameraUtil.SetZoomIn(zoomIn);
        this->mayaCameraUtil.SetZoomOut(zoomOut);
        this->mayaCameraUtil.Update();
    }

    if (FocusManager::Instance()->GetCameraFocusEntity() == GetEntity())
    {
		// only use the internal matrix if we are not animated
		this->GetEntity()->SetMatrix44(Attr::Transform, this->mayaCameraUtil.GetCameraTransform());	
	}

    // important: call parent class at the end to apply any further effects
    CameraProperty::OnRender();
}

//------------------------------------------------------------------------------
/**
*/
void 
MayaCameraProperty::OnDeactivate()
{
    // clear input focus, if we are the input focus object
    if (this->HasFocus())
    {
        FocusManager::Instance()->SetInputFocusEntity(0);
    }
    CameraProperty::OnDeactivate();
}
}; // namespace Properties






