#pragma once
//------------------------------------------------------------------------------
/**
    @class Shared::TreeClusterGraphicsEntityShared

    Shared data object for TreeClusterGraphicsEntity.
    
    (C) 2010 Radon Labs GmbH
*/
#include "shared/graphics/graphicsentityshared.h"

//------------------------------------------------------------------------------
namespace Forest
{
    class TreeClusterGraphicsEntityShared : public Shared::GraphicsEntityShared
{
public:
    /// constructor
    TreeClusterGraphicsEntityShared();
};

//------------------------------------------------------------------------------
/**
*/
inline
TreeClusterGraphicsEntityShared::TreeClusterGraphicsEntityShared()
{
    // empty
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
