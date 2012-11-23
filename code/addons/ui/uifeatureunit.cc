//------------------------------------------------------------------------------
//  ui/uifeatureunit.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uifeatureunit.h"
#include "graphics/graphicsserver.h"
#include "framesync/framesynctimer.h"
#include "ui/uimanager.h"
#include "ui/uirenderer.h"

namespace UI
{
__ImplementClass(UIFeatureUnit, 'UIFU' , Game::FeatureUnit);
__ImplementSingleton(UIFeatureUnit);

using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::UIFeatureUnit()
{	
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::~UIFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnActivate()
{
    // create module
    uiRenderModule = UI::UIRenderModule::Create();
    uiRenderModule->Setup();

    FeatureUnit::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnDeactivate()
{
    // discard render module
    uiRenderModule->Discard();
    uiRenderModule = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnRenderDebug()
{
    UIManager::Instance()->RenderDebug();
	FeatureUnit::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::SetUIEventHandler(const Ptr<UI::UIEventHandler>& handler)
{
	n_assert(handler.isvalid());
    UIManager::Instance()->SetEventHandler(handler);
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnFrame()
{    
    // update time in ui system
    Timing::Time time = FrameSync::FrameSyncTimer::Instance()->GetTime();
    UIManager* uiManager = UIManager::Instance();

    // trigger ui
    uiManager->SetTime(time);
    uiManager->Trigger();
	uiManager->Render();

	// now render the stuff	
    UIRenderer::Instance()->OnFrame();

    FeatureUnit::OnFrame();
}
}; // namespace UI
