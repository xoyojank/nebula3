//------------------------------------------------------------------------------
// fxrendermodule.cc
// (C) 2009 Radon Labs GmbH

#include "stdneb.h"
#include "fx/fxrendermodule.h"
#include "fx/rt/fxrtplugin.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"

namespace FX
{
__ImplementClass(FXRenderModule,'FXRM',RenderModules::RenderModule);
__ImplementSingleton(FXRenderModule);

using namespace Graphics;
using namespace Messaging;
using namespace RenderModules;

//------------------------------------------------------------------------------
/**
    Constructor	
*/
FXRenderModule::FXRenderModule()
{
}
//------------------------------------------------------------------------------
/**
    Destructor	
*/
FXRenderModule::~FXRenderModule()
{
}

//------------------------------------------------------------------------------
/**
    Setup render module    	
*/
void
FXRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // Register the fx render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&FXRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());

}

//------------------------------------------------------------------------------
/**
    Discard render module	
*/
void
FXRenderModule::Discard()
{
    n_assert(this->IsValid());
    RenderModule::Discard();

    // Unregister the fx render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&FXRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());

}

} // namespace FX