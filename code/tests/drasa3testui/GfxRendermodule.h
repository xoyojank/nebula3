#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIRenderModule

    The central client-side object for the UI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rendermodule.h"
#include "core/singleton.h"
#include "input/inputevent.h"
#include "Gfxprotocol.h"

//------------------------------------------------------------------------------
namespace ScaleForms
{

class GfxRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(GfxRenderModule);
    __DeclareSingleton(GfxRenderModule);
public:
    /// constructor
    GfxRenderModule();
    /// destructor
    virtual ~GfxRenderModule();
    
    /// setup the render module
    virtual void Setup();
    /// discard the render module
    virtual void Discard();   
	/// called per-frame (processes input events)
	virtual void OnFrame();
	/// send input events to visible layouts
	void SendInputEvents(const Util::Array<GFxMouseEvent>& inputEvents);

private:
    //Ptr<UIManager> uiManager;
    //Ptr<UIRenderer> uiRenderer;
	//Util::Array<Input::InputEvent> inputEvents;
	Util::Array<GFxMouseEvent> inputEvents;
	//Ptr<ScaleForms::ProcessEvents> processEventsMsg;
	Math::float2 mousePosition;
};

} // namespace UI
//------------------------------------------------------------------------------
