#pragma once
//------------------------------------------------------------------------------
/**
    @class Visibility::VisibilityContext
       
    (C) 2010 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "math/bbox.h"
              
//------------------------------------------------------------------------------
namespace Visibility
{   
class VisibilityChecker;
class VisibilitySystemBase;

class VisibilityContext : public Core::RefCounted
{
    __DeclareClass(VisibilityContext);
public:
    /// constructor
    VisibilityContext();
    /// destructor
    virtual ~VisibilityContext();     
    /// get GfxEntity	
    const Ptr<InternalGraphics::InternalGraphicsEntity>& GetGfxEntity() const; 
    /// get BoundingBox	
    const Math::bbox& GetBoundingBox() const;  
    /// get frameid visible	
    IndexT GetVisibleFrameId() const;            
    /// set Visible
    void SetVisibleFrameId(IndexT frameId);
              
private:  
    friend class VisibilityChecker;
    /// setup
    void Setup(const Ptr<InternalGraphics::InternalGraphicsEntity>& entity); 
    /// update bounding box
    void UpdateBoundingBox(const Math::bbox& box);

    Ptr<InternalGraphics::InternalGraphicsEntity> gfxEntity;
    IndexT visibleFrameId;
    Math::bbox boundingBox;    
};

//------------------------------------------------------------------------------
/**
*/
inline IndexT 
VisibilityContext::GetVisibleFrameId() const
{
    return this->visibleFrameId;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
VisibilityContext::SetVisibleFrameId(IndexT val)
{
    this->visibleFrameId = val;
}  

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalGraphics::InternalGraphicsEntity>& 
VisibilityContext::GetGfxEntity() const
{
    return this->gfxEntity;        
}        

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox& 
VisibilityContext::GetBoundingBox() const
{
    return this->boundingBox;        
}
} // namespace Visibility
//------------------------------------------------------------------------------

