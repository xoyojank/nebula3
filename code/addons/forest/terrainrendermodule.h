#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::ForestRenderModule

    The client-side object for the forest subsystem

    (C) 2009 Radon Labs GmbH
*/
#include "core/singleton.h"
#include "rendermodules/rendermodule.h"

namespace Terrain
{
class TerrainRenderModule : public RenderModules::RenderModule
{
    __DeclareClass(TerrainRenderModule);
    __DeclareSingleton(TerrainRenderModule);
    
public:
    /// Constructor
    TerrainRenderModule();
    /// Destructor
    virtual ~TerrainRenderModule();

    /// Setup the render module (call once at startup to register rendermodule)
    virtual void Setup();
    /// Discard the render module
    virtual void Discard();
};

} // namespace Forest