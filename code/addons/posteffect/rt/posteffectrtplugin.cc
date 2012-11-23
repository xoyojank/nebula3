//------------------------------------------------------------------------------
// posteffectrtplugin.cc
// (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "posteffect/rt/posteffectrtplugin.h"
#include "graphics/graphicsinterface.h"

namespace PostEffect
{
__ImplementClass(PostEffectRTPlugin,'PERP',RenderModules::RTPlugin);

using namespace Graphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
    Constructor	
*/
PostEffectRTPlugin::PostEffectRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
	Destructor
*/
PostEffectRTPlugin::~PostEffectRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
    Called when registered to the render-thread side
*/
void
PostEffectRTPlugin::OnRegister()
{
    // setup the posteffect server
    this->postEffectServer = PostEffectServer::Create();
    this->postEffectServer->Open();

    // setup the posteffect handler
    this->postEffectHandler = PostEffectHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->postEffectHandler.cast<Handler>());

}

//------------------------------------------------------------------------------
/**
    Called when unregistered from the render-thread side
*/
void
PostEffectRTPlugin::OnUnregister()
{
    // discard the posteffect handler
    this->postEffectHandler = 0;

    // discard the posteffect server
    this->postEffectServer->Close();
    this->postEffectServer = 0;
}

//------------------------------------------------------------------------------
/**
    Called before rendering
*/
void
PostEffectRTPlugin::OnRenderBefore(IndexT frameId, Timing::Time time)
{
    this->postEffectServer->OnFrame(time);
}

} // namespace PostEffect