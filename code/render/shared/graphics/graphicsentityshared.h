#pragma once
//------------------------------------------------------------------------------
/**
    @class Shared::GraphicsEntityShared
    
    Shared data object for InternalGraphicsEntity and GraphicsEntity.
    To be used with a FrameSyncSharedData object.
    
    (C) 2010 Radon Labs GmbH
*/
#include "core/types.h"
#include "math/clipstatus.h"
#include "math/bbox.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Shared
{
class GraphicsEntityShared
{
public:
    /// constructor
    GraphicsEntityShared();    

    Math::ClipStatus::Type clipStatus;  // currently visible through any camera?
    Math::bbox localBox;                // current local bounding box
    Math::bbox globalBox;               // current global bounding box
    Timing::Time entityTime;            // current entity time
    Timing::Time timeFactor;            // current time factor
};

//------------------------------------------------------------------------------
/**
*/
inline
GraphicsEntityShared::GraphicsEntityShared() :
    clipStatus(Math::ClipStatus::Outside),
    entityTime(0.0),
    timeFactor(1.0)
{
    // empty
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
