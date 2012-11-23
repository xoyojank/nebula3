#pragma once
//------------------------------------------------------------------------------
/**
    @class Shared::ModelEntityShared

    Shared data object for InternalModelEntity and ModelEntity.
    
    (C) 2010 Radon Labs GmbH
*/
#include "shared/graphics/graphicsentityshared.h"
#include "shared/graphics/charjointinfo.h"

//------------------------------------------------------------------------------
namespace Shared
{
class ModelEntityShared : public GraphicsEntityShared
{
public:
    /// constructor
    ModelEntityShared();
    
    Math::vector animDrivenMotionVector;
    Util::Array<Animation::AnimEventInfo> animEvents;
    bool charJointDataValid;
    Util::Array<CharJointInfo> charJoints;
};

//------------------------------------------------------------------------------
/**
*/
inline
ModelEntityShared::ModelEntityShared() :
    animDrivenMotionVector(Math::vector::nullvec()),
    charJointDataValid(false)
{
    // empty
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
