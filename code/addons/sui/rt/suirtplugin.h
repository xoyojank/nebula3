#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIRTPlugin

    Setup render-thread environment for the SUI system.

    (C) 2009 Radon Labs GmbH
*/
#include "rendermodules/rt/rtplugin.h"
#include "sui/rt/suiserver.h"
#include "sui/rt/suihandler.h"

//------------------------------------------------------------------------------
namespace SUI
{
class SUIRTPlugin : public RenderModules::RTPlugin
{
    __DeclareClass(SUIRTPlugin);
public:
    /// constructor
    SUIRTPlugin();
    /// destructor
    virtual ~SUIRTPlugin();

    /// called when plugin is registered on the render-thread side
    virtual void OnRegister();
    /// called when plugin is unregistered on the render-thread side
    virtual void OnUnregister();
    /// called after updating entities
    virtual void OnUpdateAfter(IndexT frameId, Timing::Time time);
    /// called from within Frame batch rendering (if registered)
    virtual void OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch);

private:
    Ptr<SUIServer> suiServer;
    Ptr<SUIHandler> suiHandler;
};

} // namespace SUI
//------------------------------------------------------------------------------
