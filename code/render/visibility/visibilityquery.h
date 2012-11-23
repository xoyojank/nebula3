#pragma once
//------------------------------------------------------------------------------
/**
    @class Visibility::VisibilityQuery

    Each VisibilityQuery starts dependend jobs for every attached 
    visibility system.
           
    (C) 2010 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "visibility/visibilitysystems/visibilitysystembase.h"
#include "jobs/jobport.h"
              
//------------------------------------------------------------------------------
namespace Visibility
{   
class VisibilityQuery : public Core::RefCounted
{
    __DeclareClass(VisibilityQuery);
public:
    /// constructor
    VisibilityQuery();
    /// destructor
    virtual ~VisibilityQuery();
    /// attach visible system, used by this job
    void AttachVisibilitySystem(const Ptr<VisibilitySystemBase>& visSystem); 
    /// set Observer
    void SetObserver(const Ptr<InternalGraphics::InternalGraphicsEntity>& val);       
    /// get Observer
    const Ptr<InternalGraphics::InternalGraphicsEntity>& GetObserver() const;   
    /// run job
    void Run(IndexT frameId);
    /// is finished
    bool IsFinished() const;
    /// wait for finished
    bool WaitForFinished() const;
    /// get observer link type
    InternalGraphics::InternalGraphicsEntity::LinkType GetObserverType();
    /// get visible entities   
    const Util::FixedArray<Ptr<VisibilityContext> >& GetVisibleEntities() const;
    /// set entity mask
    void SetEntityMask(uint mask);
              
private:  
    Ptr<InternalGraphics::InternalGraphicsEntity> observer; 
    Ptr<ObserverContext> observerContext;
    Util::FixedArray<Ptr<VisibilityContext> > visibleEntities;
    Util::Array<Ptr<VisibilitySystemBase> > visibilitySystems;       
    uint entityMask;
    Ptr<Jobs::JobPort> jobPort;
};

//------------------------------------------------------------------------------
/**
*/
inline const Util::FixedArray<Ptr<VisibilityContext> >& 
VisibilityQuery::GetVisibleEntities() const
{
    n_assert(this->IsFinished());    
    return this->visibleEntities;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
VisibilityQuery::SetObserver(const Ptr<InternalGraphics::InternalGraphicsEntity>& val)
{
    this->observer = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<InternalGraphics::InternalGraphicsEntity>& 
VisibilityQuery::GetObserver() const
{
    return this->observer;
}       

//------------------------------------------------------------------------------
/**
*/
inline void 
VisibilityQuery::SetEntityMask(uint mask)
{
    this->entityMask = mask;
}
} // namespace Visibility
//------------------------------------------------------------------------------

