#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreUI::CoreUIRTPlugin

    Setup render-thread environment for the UI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rt/rtplugin.h"
#include "ui/rt/coreuirenderer.h"
#include "ui/rt/coreuigraphicshandler.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class CoreUIRTPlugin : public RenderModules::RTPlugin
{
    __DeclareClass(CoreUIRTPlugin);
public:
    /// constructor
    CoreUIRTPlugin();
    /// destructor
    virtual ~CoreUIRTPlugin();

    /// called when plugin is registered on the render-thread side
    virtual void OnRegister();
    /// called when plugin is unregistered on the render-thread side
    virtual void OnUnregister();

    /// called if no view exists, and no default camera is set in view
    virtual void OnRenderWithoutView(IndexT frameId, Timing::Time time);
    /// called from within Frame batch rendering (if registered)
    virtual void OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch);

private:
    Ptr<CoreUIRenderer> coreUiRenderer;
    Ptr<CoreUIGraphicsHandler> coreUiHandler;
};

} // namespace UI
//------------------------------------------------------------------------------
