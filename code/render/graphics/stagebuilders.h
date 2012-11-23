#pragma once
//------------------------------------------------------------------------------
/**
    @file graphics/stagebuilders.h
    
    Frontend typedefs to InternalGraphics::StageBuilder classes.
    
    (C) 2008 Radon Labs GmbH
*/
#include "internalgraphics/stagebuilder.h"
#include "internalgraphics/simplestagebuilder.h"
#include "internalgraphics/quadtreestagebuilder.h"

//------------------------------------------------------------------------------
namespace Graphics
{
typedef InternalGraphics::StageBuilder StageBuilder;
typedef InternalGraphics::SimpleStageBuilder SimpleStageBuilder;
typedef InternalGraphics::QuadtreeStageBuilder QuadtreeStageBuilder;
}
//------------------------------------------------------------------------------

    