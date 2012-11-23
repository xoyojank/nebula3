//------------------------------------------------------------------------------
// posteffectrendermodule.cc
// (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "terrainrendermodule.h"
#include "rt/terrainplugin.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsprotocol.h"

namespace Terrain
{
__ImplementClass(TerrainRenderModule,'TRMD',RenderModules::RenderModule);
__ImplementSingleton(TerrainRenderModule);

using namespace Graphics;
using namespace Messaging;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
    Constructor
*/
TerrainRenderModule::TerrainRenderModule()
{
}

//------------------------------------------------------------------------------
/**
    Destructor	
*/
TerrainRenderModule::~TerrainRenderModule()
{
}

//------------------------------------------------------------------------------
/**
    Setup the render module	
*/
void
TerrainRenderModule::Setup()
{
    n_assert(!this->IsValid());
    RenderModule::Setup();

    // Register the posteffect render-thread plugin
    Ptr<RegisterRTPlugin> msg = RegisterRTPlugin::Create();
    msg->SetType(&TerrainRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());    
}

//------------------------------------------------------------------------------
/**
    Discard the render module	
*/
void
TerrainRenderModule::Discard()
{
    n_assert(this->IsValid());
    RenderModule::Discard();

    // Unregister the forest render-thread plugin
    Ptr<UnregisterRTPlugin> msg = UnregisterRTPlugin::Create();
    msg->SetType(&TerrainRTPlugin::RTTI);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

}