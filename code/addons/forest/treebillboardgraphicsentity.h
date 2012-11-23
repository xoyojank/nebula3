#pragma once
//------------------------------------------------------------------------------
/**
    @class Forest::TreeBillboardGraphicsEntity

    A subclass of graphics entity to notify the tree billboard renderer
    about the Nebula 3 render loop.

    (C) 2009 Radon Labs GmbH
*/
#include "internalgraphics/internalgraphicsentity.h"

//------------------------------------------------------------------------------
namespace Forest
{

class TreeBillboardGraphicsEntity : public InternalGraphics::InternalGraphicsEntity
{
    __DeclareClass(TreeBillboardGraphicsEntity);
public:
    /// constructor
    TreeBillboardGraphicsEntity();
    /// destructor
    virtual ~TreeBillboardGraphicsEntity();

    /// get entity type
    virtual InternalGraphics::InternalGraphicsEntityType::Code GetType() const;
    /// called when attached to level
    virtual void OnActivate();
    /// called when removed from level
    virtual void OnDeactivate();
    /// render the graphics entity
    virtual void Render();
};

} // namespace Forest2
//------------------------------------------------------------------------------
