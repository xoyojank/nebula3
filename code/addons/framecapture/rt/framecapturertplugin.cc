//------------------------------------------------------------------------------
// framecapturertplugin.cc
// (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framecapturertplugin.h"
#include "framecaptureserver.h"
#include "framecapturehandler.h"
#include "framecapturepagehandler.h"
#include "graphics/graphicsinterface.h"
#include "http/httpserverproxy.h"

using namespace Graphics;
using namespace Http;
//------------------------------------------------------------------------------
namespace FrameCapture
{
__ImplementClass(FrameCapture::FrameCaptureRTPlugin,'FCRP',RenderModules::RTPlugin);
//------------------------------------------------------------------------------
/**
    Constructor	
*/
FrameCaptureRTPlugin::FrameCaptureRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
    Destructor	
*/
FrameCaptureRTPlugin::~FrameCaptureRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
    On register plugin	
*/
void
FrameCaptureRTPlugin::OnRegister()
{
    // create frame capture server
    this->frameCaptureServer = FrameCaptureServer::Create();
    // setup base standard directory
    Util::String baseDir;
    baseDir.Format("user:%s/capture",GraphicsInterface::Instance()->GetAppName().Value());
    this->frameCaptureServer->SetBaseDirectory(baseDir);

    // create and attach frame capture handler
    this->frameCaptureHandler = FrameCaptureHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->frameCaptureHandler.cast<Messaging::Handler>());
#if __NEBULA3_HTTP__
    // create and attach http request handler
    this->httpHandler = FrameCapturePageHandler::Create();    
    HttpServerProxy::Instance()->AttachRequestHandler(this->httpHandler.cast<Http::HttpRequestHandler>());
#endif
}

//------------------------------------------------------------------------------
/**
    On unregister plugin	
*/
void
FrameCaptureRTPlugin::OnUnregister()
{
    // remove http request handler
#if __NEBULA3_HTTP__
    HttpServerProxy::Instance()->RemoveRequestHandler(this->httpHandler.cast<Http::HttpRequestHandler>());
#endif
    // dispose and detach frame capture handler
    this->frameCaptureHandler->Close();
    GraphicsInterface::Instance()->RemoveHandler(this->frameCaptureHandler.cast<Messaging::Handler>());
    this->frameCaptureHandler = 0;
    // dispose frame capture server
    this->frameCaptureServer = 0;
}
} // namespace FrameCapture