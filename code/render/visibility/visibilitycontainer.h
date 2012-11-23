#pragma once
//------------------------------------------------------------------------------
/**
    @class Visibility::VisibilityContainer
       
    (C) 2010 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "visibility/visibilitycontext.h"
#include "visibility/visibilitychecker.h"
              
//------------------------------------------------------------------------------
namespace Visibility
{   
class VisibilityContainer : public Core::RefCounted
{
    __DeclareClass(VisibilityContainer);
public:
    /// constructor
    VisibilityContainer();
    /// destructor
    virtual ~VisibilityContainer();    
    /// set gfx entities 
    void SetGraphicsEntities(const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& entities);
    /// get visibility contexts
    const Util::Array<Ptr<VisibilityContext> >& GetVisibilityContexts() const;
              
protected:    
    friend class VisibilityChecker;
    /// get entities
    const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& GetGraphicsEntities() const;
    /// convert gfx entities to contexts
    void SetVisibilityContexts(const Util::Array<Ptr<VisibilityContext> >& contexts);

    Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> > gfxEntities; 
    Util::Array<Ptr<VisibilityContext> > visibilityContexts; 
};      
//------------------------------------------------------------------------------
/**
*/
inline void 
VisibilityContainer::SetGraphicsEntities(const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& entities)
{
    this->gfxEntities = entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<VisibilityContext> >& 
VisibilityContainer::GetVisibilityContexts() const
{
    return this->visibilityContexts;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& 
VisibilityContainer::GetGraphicsEntities() const
{
    return this->gfxEntities;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
VisibilityContainer::SetVisibilityContexts(const Util::Array<Ptr<VisibilityContext> >& contexts)
{
    this->visibilityContexts = contexts;
}
} // namespace Visibility
//------------------------------------------------------------------------------

