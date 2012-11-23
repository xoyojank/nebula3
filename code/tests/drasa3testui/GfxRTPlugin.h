#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIRTPlugin

    Setup render-thread environment for the SUI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rt/rtplugin.h"
#include "GFXRender.h"
#include "GfxHandler.h"

//------------------------------------------------------------------------------
namespace ScaleForms
{
class GfxRTPlugin : public RenderModules::RTPlugin
{
    __DeclareClass(GfxRTPlugin);
public:
    /// constructor
    GfxRTPlugin();
    /// destructor
    virtual ~GfxRTPlugin();

    /// called when plugin is registered on the render-thread side
    virtual void OnRegister();
    /// called when plugin is unregistered on the render-thread side
    virtual void OnUnregister();
    /// called after updating entities
    virtual void OnUpdateAfter(IndexT frameId, Timing::Time time);
    /// called from within Frame batch rendering (if registered)
    virtual void OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch);

private:
    Ptr<GfxRender> gfxRender;
    Ptr<GfxHandler> gfxHandler;
};

} // namespace SUI
//------------------------------------------------------------------------------
