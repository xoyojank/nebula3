#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::PointLightEntity
    
    Client-side proxy of a InternalGraphics::InternalPointLightEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/abstractlightentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class PointLightEntity : public AbstractLightEntity
{
    __DeclareClass(PointLightEntity);
public:
    /// constructor
    PointLightEntity();

    /// set transform from position and range
    void SetTransformFromPosDirAndRange(const Math::point& pos, const Math::vector& dir, float range);

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);
};

} // namespace Graphics
//------------------------------------------------------------------------------
