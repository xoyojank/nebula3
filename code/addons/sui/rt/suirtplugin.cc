//------------------------------------------------------------------------------
//  suirtplugin.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "sui/rt/suirtplugin.h"
#include "frame/framebatch.h"
#include "graphics/graphicsinterface.h"

namespace SUI
{
__ImplementClass(SUI::SUIRTPlugin, 'SRTP', RenderModules::RTPlugin);

using namespace RenderModules;
using namespace Frame;
using namespace CoreGraphics;
using namespace Graphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
SUIRTPlugin::SUIRTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SUIRTPlugin::~SUIRTPlugin()
{
    n_assert(!this->suiServer.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRTPlugin::OnRegister()
{
    n_assert(!this->suiServer.isvalid());

    // setup the render-thread SUIServer
    this->suiServer = SUIServer::Create();
    this->suiServer->Setup();
    
    // setup the SUI message handler
    this->suiHandler = SUIHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->suiHandler.cast<Handler>());
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRTPlugin::OnUnregister()
{
    n_assert(this->suiServer.isvalid());

    // release SUI handler ref
    GraphicsInterface::Instance()->RemoveHandler(this->suiHandler.cast<Handler>());
    this->suiHandler->Close();
    this->suiHandler = 0;

    // discard the SUIServer
    this->suiServer->Discard();
    this->suiServer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRTPlugin::OnUpdateAfter(IndexT frameTime, Timing::Time time)
{
    this->suiServer->Update();
}

//------------------------------------------------------------------------------
/**
*/
void
SUIRTPlugin::OnRenderFrameBatch(const Ptr<FrameBatch>& frameBatch)
{
    if (frameBatch->GetType() == BatchType::UI)
    {
        this->suiServer->Render(frameBatch);
    }
}

} // namespace SUI