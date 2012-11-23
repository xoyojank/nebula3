//------------------------------------------------------------------------------
//  uirendermodule.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "GfxRendermodule.h"
#include "GfxRTPlugin.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"
#include "input/mouse.h"
#include "input/inputevent.h"
#include "input/inputserver.h"

namespace ScaleForms
{
__ImplementClass(ScaleForms::GfxRenderModule, 'GFRM', RenderModules::RenderModule);
__ImplementSingleton(ScaleForms::GfxRenderModule);

using namespace RenderModules;
using namespace Graphics;
using namespace Messaging;
using namespace Util;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
GfxRenderModule::GfxRenderModule()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GfxRenderModule::~GfxRenderModule()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // register the render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&ScaleForms::GfxRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::Discard()
{
    n_assert(this->IsValid());
    RenderModule::Discard();

    // unregister the render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&ScaleForms::GfxRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRenderModule::SendInputEvents(const Array<GFxMouseEvent>& inputEvents)
{
    // accumulate input events for one frame, and send them as batch 
    // during OnFrame()
    this->inputEvents.AppendArray(inputEvents);
}

//------------------------------------------------------------------------------
/**
    The OnFrame() method sends input events and receives SUI events.
*/
void
GfxRenderModule::OnFrame()
{
	Ptr<Input::Mouse> mouse = Input::InputServer::Instance()->GetDefaultMouse();

	if (mouse.isvalid())
	{
		// record current mouse position
		this->mousePosition = mouse->GetPixelPosition();

		if(mouse->GetMovement().lengthsq() > N_TINY)
		{
			GFxMouseEvent mevent(GFxEvent::MouseMove,0,this->mousePosition.x(),this->mousePosition.y());
			this->inputEvents.Append(mevent);
		}

		if( mouse->ButtonDown(MouseButton::LeftButton) )
		{
			GFxMouseEvent mevent(GFxEvent::MouseDown,0,this->mousePosition.x(),this->mousePosition.y());
			this->inputEvents.Append(mevent);
		}

		if( mouse->ButtonUp(MouseButton::LeftButton) )
		{
			GFxMouseEvent mevent(GFxEvent::MouseUp,0,this->mousePosition.x(),this->mousePosition.y());
			this->inputEvents.Append(mevent);
		}
	}

	// clear SUI events from previous frame
	//this->gfxEvents.Clear();

	//if (this->processEventsMsg.isvalid() && this->processEventsMsg->Handled())
	//{        
	//	//this->gfxEvents = this->processEventsMsg->GetInputEvents();
	//	this->processEventsMsg = 0;
	//}

	// may not send new message before previous has been handled
	if ( this->inputEvents.Size() > 0)
	{
		Ptr<ScaleForms::ProcessEvents> processEventsMsg = ProcessEvents::Create();
		processEventsMsg->SetInputEvents(this->inputEvents);
		GraphicsInterface::Instance()->Send(processEventsMsg.cast<Message>());
		this->inputEvents.Clear();
	}
}

} // namespace UI