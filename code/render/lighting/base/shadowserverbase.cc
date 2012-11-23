//------------------------------------------------------------------------------
//  shadowserverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/base/shadowserverbase.h"
#include "lighting/lightserver.h"

namespace Lighting
{
__ImplementClass(Lighting::ShadowServerBase, 'SDSB', Core::RefCounted);
__ImplementSingleton(Lighting::ShadowServerBase);

using namespace InternalGraphics;
using namespace CoreGraphics;
using namespace Math;
using namespace Threading;
//------------------------------------------------------------------------------
/**
*/
ShadowServerBase::ShadowServerBase() :
    isOpen(false),
    inBeginFrame(false),
    inBeginAttach(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShadowServerBase::~ShadowServerBase()
{
    n_assert(!this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::BeginFrame(const Ptr<InternalCameraEntity>& camEntity)
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(this->localLightEntities.IsEmpty());
    n_assert(!this->globalLightEntity.isvalid());
    n_assert(!this->cameraEntity.isvalid());
    n_assert(camEntity.isvalid());
    this->cameraEntity = camEntity;
    this->inBeginFrame = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::EndFrame()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    this->inBeginFrame = false;
    this->localLightEntities.Clear();
    this->globalLightEntity = 0;
    this->cameraEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::BeginAttachVisibleLights()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(this->localLightEntities.IsEmpty());
    n_assert(!this->globalLightEntity.isvalid());
    this->inBeginAttach = true;
    this->globalLightEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::AttachVisibleLight(const Ptr<InternalAbstractLightEntity>& lightEntity)
{
    n_assert(lightEntity->GetCastShadows());
    if (lightEntity->GetLightType() == LightType::Global)
    {
        this->globalLightEntity = lightEntity.downcast<InternalGlobalLightEntity>();
    }
    else
    {
        this->localLightEntities.Append(lightEntity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::EndAttachVisibleLights()
{
    n_assert(this->inBeginAttach);
    this->inBeginAttach = false;

    // @todo: sort shadow casting light sources by priority
    this->SortLights();

    // set only max num lights to shadow casting
    IndexT i;
    IndexT countLights = 0;
    for (i = 0; i < this->localLightEntities.Size(); ++i)
    {           
        if (countLights < MaxNumShadowLights)
        {               
            this->localLightEntities[i]->SetCastShadowsThisFrame(true);
            // attach light to light server with correct shadow casting flag
            LightServer::Instance()->AttachVisibleLight(this->localLightEntities[i]);
        }
        else 
        {   
            this->localLightEntities[i]->SetCastShadowsThisFrame(false);
            // attach light to light server with correct shadow casting flag
            LightServer::Instance()->AttachVisibleLight(this->localLightEntities[i]);
            this->localLightEntities.EraseIndex(i);
            --i;
        }
        countLights++;
    }
}

//------------------------------------------------------------------------------
/**
    This method updates the shadow buffers (if supported on the
    platform).
*/
void
ShadowServerBase::UpdateShadowBuffers()
{
    // override in subclass
}

//------------------------------------------------------------------------------
/**
    Sort lights by their attenuation at point of interest
*/
void 
ShadowServerBase::SortLights()      
{
    // implement in platform dependend class    
}

} // namespace Lighting
