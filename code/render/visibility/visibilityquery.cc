//------------------------------------------------------------------------------
//  entityvisibility.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "visibility/visibilityquery.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalgraphicsserver.h"

namespace Visibility
{
__ImplementClass(Visibility::VisibilityQuery, 'VIQU', Core::RefCounted);

using namespace Util;
using namespace Math;
using namespace Jobs;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
VisibilityQuery::VisibilityQuery()
{
    this->jobPort = JobPort::Create();
    this->jobPort->Setup();
}

//------------------------------------------------------------------------------
/**
*/
VisibilityQuery::~VisibilityQuery()
{
    this->jobPort = 0;
    this->observer = 0;
    this->observerContext = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
VisibilityQuery::AttachVisibilitySystem(const Ptr<VisibilitySystemBase>& visSystem)
{
    this->visibilitySystems.Append(visSystem);
}

//------------------------------------------------------------------------------
/**
*/
void 
VisibilityQuery::Run(IndexT frameId)
{
    // not necessary, job will be destroyed after results are applied          
    this->visibleEntities.SetSize(InternalGraphicsServer::Instance()->GetEntities().Size());     
    this->visibleEntities.Fill(0);
    
    // TODO: create worker thread which goes thru all vis systems
    this->observerContext = ObserverContext::Create();
    this->observerContext->Setup(this->observer);    

    // for now, no multi threading: call directly check visibility function
    Util::Array<Ptr<Job> > jobs;
    IndexT i;
    for (i = 0; i < this->visibilitySystems.Size(); ++i)
    {   
        const Ptr<VisibilitySystemBase>& visSystem = this->visibilitySystems[i];
        Ptr<Job> newJob = visSystem->CreateVisibilityJob(frameId, 
                                                         this->observerContext, 
                                                         this->visibleEntities, 
                                                         this->entityMask);
        // for non multi-threaded 
        if (newJob.isvalid())
        {
            jobs.Append(newJob);        
        }  
        // if multi threading, all visibility systems must work doublebuffered over 2 frames
        //
    }
    // visibility systems are dependent on results of previous visibility system
    this->jobPort->PushJobChain(jobs);
    this->jobPort->WaitDone();
}  

//------------------------------------------------------------------------------
/**
*/
bool 
VisibilityQuery::IsFinished() const
{
    return this->jobPort->CheckDone();
}
    
//------------------------------------------------------------------------------
/**
*/
bool
VisibilityQuery::WaitForFinished() const
{
    this->jobPort->WaitDone();
    return true;
}    

//------------------------------------------------------------------------------
/**
*/
InternalGraphics::InternalGraphicsEntity::LinkType 
VisibilityQuery::GetObserverType()
{       
    if (this->observer->GetType() == InternalGraphicsEntityType::Light)
    {
        return InternalGraphicsEntity::LightLink;
    }
    return InternalGraphicsEntity::CameraLink;
}
} // namespace Visibility
