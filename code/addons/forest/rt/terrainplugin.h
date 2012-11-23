#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestRTPlugin
    
    Setup render-thread environment for the forest subsystem

    (C) 2009 Radon Labs GmbH
*/

#include "forest/rt/terrainhandler.h"
#include "forest/rt/terrainserver.h"
#include "rendermodules/rt/rtplugin.h"

namespace Terrain
{
class TerrainRTPlugin : public RenderModules::RTPlugin
{
__DeclareClass(TerrainRTPlugin);

public:
    /// Constructor
    TerrainRTPlugin();
    /// Destructor
    virtual ~TerrainRTPlugin();
    
    /// Called when registered on the render-thread side
    virtual void OnRegister();
    /// Called when unregistered from the render-thread side
    virtual void OnUnregister();
    /// Called before rendering
    virtual void OnRenderBefore(IndexT frameId, Timing::Time time);
    /// called after rendering entities
    virtual void OnRenderAfter(IndexT frameId, Timing::Time time);

private:
    
    Ptr<TerrainServer> terrainServer;
    Ptr<TerrainHandler> terrainHandler;

};
} // namespace Forest