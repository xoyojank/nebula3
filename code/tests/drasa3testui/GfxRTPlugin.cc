//------------------------------------------------------------------------------
//  suirtplugin.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "GfxRTPlugin.h"
#include "frame/framebatch.h"
#include "graphics/graphicsinterface.h"

namespace ScaleForms
{
__ImplementClass(ScaleForms::GfxRTPlugin, 'SRTP', RenderModules::RTPlugin);

using namespace RenderModules;
using namespace Frame;
using namespace CoreGraphics;
using namespace Graphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
GfxRTPlugin::GfxRTPlugin()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GfxRTPlugin::~GfxRTPlugin()
{
    n_assert(!this->gfxRender.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRTPlugin::OnRegister()
{
    n_assert(!this->gfxRender.isvalid());

    // setup the render-thread SUIServer
	this->gfxRender = GfxRender::Create();
    this->gfxRender->Setup();
    
    // setup the SUI message handler
    this->gfxHandler = GfxHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->gfxHandler.cast<Handler>());
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRTPlugin::OnUnregister()
{
    n_assert(this->gfxRender.isvalid());

    // release SUI handler ref
    GraphicsInterface::Instance()->RemoveHandler(this->gfxHandler.cast<Handler>());
    this->gfxHandler->Close();
    this->gfxHandler = 0;

    // discard the SUIServer
    this->gfxRender->Discard();
    this->gfxRender = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRTPlugin::OnUpdateAfter(IndexT frameTime, Timing::Time time)
{
    //this->suiServer->Update();
}

//------------------------------------------------------------------------------
/**
*/
void
GfxRTPlugin::OnRenderFrameBatch(const Ptr<FrameBatch>& frameBatch)
{
    if (frameBatch->GetType() == BatchType::UI)
    {
        this->gfxRender->Render(frameBatch);
    }
}

} // namespace SUI