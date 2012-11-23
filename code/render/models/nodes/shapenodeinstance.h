#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::ShapeNodeInstance
  
    The ShapeNodeInstance actually renders a static shape, and holds
    all the necessary per-instance state to do so.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/statenodeinstance.h"

//------------------------------------------------------------------------------
namespace Models
{
class ShapeNodeInstance : public StateNodeInstance
{
    __DeclareClass(ShapeNodeInstance);
public:
    /// constructor
    ShapeNodeInstance();
    /// destructor
    virtual ~ShapeNodeInstance();
    
    /// called during visibility resolve
    virtual void OnVisibilityResolve(IndexT resolveIndex, float distToViewer);
    /// perform rendering
    virtual void Render();
};

} // namespace Models
//------------------------------------------------------------------------------

