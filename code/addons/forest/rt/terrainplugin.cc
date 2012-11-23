//------------------------------------------------------------------------------
// posteffectrtplugin.cc
// (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "forest/rt/terrainplugin.h"
#include "graphics/graphicsinterface.h"

namespace Terrain
{
__ImplementClass(TerrainRTPlugin,'FORP',RenderModules::RTPlugin);

using namespace Graphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
    Constructor	
*/
TerrainRTPlugin::TerrainRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
	Destructor
*/
TerrainRTPlugin::~TerrainRTPlugin()
{
}

//------------------------------------------------------------------------------
/**
    Called when registered to the render-thread side
*/
void
TerrainRTPlugin::OnRegister()
{
    // setup the posteffect server
    this->terrainServer = TerrainServer::Create();
	this->terrainServer->SetChunkFile("bin:test.chu");
    this->terrainServer->Open();

    // setup the posteffect handler
    this->terrainHandler = TerrainHandler::Create();
    GraphicsInterface::Instance()->AttachHandler(this->terrainHandler.cast<Handler>());

}

//------------------------------------------------------------------------------
/**
    Called when unregistered from the render-thread side
*/
void
TerrainRTPlugin::OnUnregister()
{
    // discard the forest handler
    this->terrainHandler = 0;

    // discard the forest server
    this->terrainServer = 0;
}

//------------------------------------------------------------------------------
/**
    Called before rendering
*/
void
TerrainRTPlugin::OnRenderBefore(IndexT frameId, Timing::Time time)
{
    this->terrainServer->OnRenderBefore(frameId, time);
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainRTPlugin::OnRenderAfter(IndexT frameId, Timing::Time time)
{
    this->terrainServer->OnRenderAfter(frameId, time);
}

} // namespace Forest