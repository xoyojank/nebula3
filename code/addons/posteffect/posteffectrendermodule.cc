//------------------------------------------------------------------------------
// posteffectrendermodule.cc
// (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "posteffectrendermodule.h"
#include "rt/posteffectrtplugin.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"

namespace PostEffect
{
__ImplementClass(PostEffectRenderModule,'PERM',RenderModules::RenderModule);
__ImplementSingleton(PostEffectRenderModule);

using namespace Graphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
    Constructor
*/
PostEffectRenderModule::PostEffectRenderModule()
{
}

//------------------------------------------------------------------------------
/**
    Destructor	
*/
PostEffectRenderModule::~PostEffectRenderModule()
{
}

//------------------------------------------------------------------------------
/**
    Setup the render module	
*/
void
PostEffectRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // Register the posteffect render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&PostEffectRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());    
}

//------------------------------------------------------------------------------
/**
    Discard the render module	
*/
void
PostEffectRenderModule::Discard()
{
    n_assert(this->IsValid());
    RenderModule::Discard();

    // Unregister the posteffect render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&PostEffectRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

}