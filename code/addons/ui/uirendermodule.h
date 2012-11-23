#pragma once
//------------------------------------------------------------------------------
/**
    @class UI::UIRenderModule

    The central client-side object for the UI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rendermodule.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer;
class UIManager;
class UIRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(UIRenderModule);
    __DeclareSingleton(UIRenderModule);
public:
    /// constructor
    UIRenderModule();
    /// destructor
    virtual ~UIRenderModule();
    
    /// setup the render module
    virtual void Setup();
    /// discard the render module
    virtual void Discard();   

private:
    Ptr<UIManager> uiManager;
    Ptr<UIRenderer> uiRenderer;
};

} // namespace UI
//------------------------------------------------------------------------------
