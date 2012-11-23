//------------------------------------------------------------------------------
//  internalview.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/internalmodelentity.h"
#include "models/visresolver.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaperenderer.h"
#include "models/modelserver.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "particles/particlerenderer.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalView, 'IGVW', Core::RefCounted);

using namespace Util;
using namespace Models;
using namespace CoreGraphics;
using namespace Lighting;
using namespace Characters;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
InternalView::InternalView() :
    isAttachedToServer(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalView::~InternalView()
{
    // check that we've been properly cleaned up
    n_assert(!this->isAttachedToServer);
    n_assert(!this->stage.isvalid());
    n_assert(!this->camera.isvalid());
    n_assert(!this->renderTarget.isvalid() && !this->multipleRenderTarget.isvalid());
    n_assert(!this->frameShader.isvalid());
    n_assert(this->dependencies.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
InternalView::OnAttachToServer()
{
    n_assert(!this->IsAttachedToServer());
    this->isAttachedToServer = true;

#if NEBULA3_ENABLE_PROFILING
    this->resolveVisibleModelNodeInstances = Debug::DebugTimer::Create();
    String name("View_" + this->GetName().AsString() + "_ResolveModelInstanceNodes");
    this->resolveVisibleModelNodeInstances->Setup(name);

    this->updateShadowBuffers = Debug::DebugTimer::Create();
    name.Format("View_%s_UpdateShadowBuffers", this->GetName().Value());
    this->updateShadowBuffers->Setup(name);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
InternalView::OnRemoveFromServer()
{
    n_assert(this->IsAttachedToServer());
    if (this->camera.isvalid())
    {
        this->camera->OnRemoveFromView(this);
        this->camera = 0;
    }
    this->stage = 0;
    this->renderTarget = 0;
    this->multipleRenderTarget = 0;
    this->frameShader = 0;
    this->dependencies.Clear();
    this->isAttachedToServer = false;

    _discard_timer(resolveVisibleModelNodeInstances);
    _discard_timer(updateShadowBuffers);
}

//------------------------------------------------------------------------------
/**
    This method updates the visibility links for the view's camera.
    This method should usually be called before Render().
*/
void
InternalView::UpdateVisibilityLinks()
{
    n_assert(this->camera.isvalid());
    this->stage->UpdateCameraLinks(this->camera);
}

//------------------------------------------------------------------------------
/**
    This attaches visible lights to the light and shadow server.
    @todo: currently this methods needs to go over all visible
    graphics entities to find the lights... 
*/
void
InternalView::ResolveVisibleLights()
{
    n_assert(this->camera.isvalid());
    LightServer* lightServer = LightServer::Instance();
    ShadowServer* shadowServer = ShadowServer::Instance();

    lightServer->BeginAttachVisibleLights();
    shadowServer->BeginAttachVisibleLights();
    const Array<Ptr<InternalGraphicsEntity> >& visLinks = this->camera->GetLinks(InternalGraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = visLinks[i];
        if(InternalGraphicsEntityType::Light == curEntity->GetType())
        {
            curEntity->OnResolveVisibility();
        }
    }    
    shadowServer->EndAttachVisibleLights();
    lightServer->EndAttachVisibleLights();
}

//------------------------------------------------------------------------------
/**
    Resolve all visible ModelNodeInstances by following the visibility
    links of our camera. This is necessary as preparation for rendering.
    This is also where the OnRenderBefore() callback will be invoked on
    entities.
*/
void
InternalView::ResolveVisibleModelNodeInstances(IndexT frameIndex)
{
    n_assert(this->camera.isvalid());
    const matrix44& cameraTransform = this->camera->GetTransform();
    VisResolver* visResolver = VisResolver::Instance();
    visResolver->BeginResolve(cameraTransform);
    const Array<Ptr<InternalGraphicsEntity> >& visLinks = this->camera->GetLinks(InternalGraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = visLinks[i];
        if (InternalGraphicsEntityType::Model == curEntity->GetType())
        {
            curEntity->OnRenderBefore(frameIndex);
            curEntity->OnResolveVisibility();
        }
    }
    visResolver->EndResolve();
}

//------------------------------------------------------------------------------
/**
    This method sets the new camera transforms valid for this frame in the transformdevice
*/
void
InternalView::ApplyCameraSettings()
{
    TransformDevice* transformDevice = TransformDevice::Instance();
    transformDevice->SetProjTransform(this->camera->GetProjTransform());
    transformDevice->SetViewTransform(this->camera->GetViewTransform());  
    transformDevice->SetFocalLength(this->camera->GetCameraSettings().GetFocalLength());
    transformDevice->ApplyViewSettings();
}

//------------------------------------------------------------------------------
/**
    This method renders the current view into the render target. This method
    must be called sometimes after UpdateVisibilityLinks()
*/
void
InternalView::Render(IndexT frameIndex)
{
    n_assert(this->frameShader.isvalid());      
    n_assert(this->camera.isvalid());

    // reset model node instance index for a new render frame (view)
    ModelServer::Instance()->ResetModelNodeInstanceIndex();

    LightServer* lightServer = LightServer::Instance();
    ShadowServer* shadowServer = ShadowServer::Instance();
    lightServer->BeginFrame(this->camera);
    shadowServer->BeginFrame(this->camera);

    // resolve visible light source
    this->ResolveVisibleLights();

    _start_timer(updateShadowBuffers);
    // draw the shadow pass
    shadowServer->UpdateShadowBuffers();
    _stop_timer(updateShadowBuffers);
            
    Particles::ParticleRenderer::Instance()->BeginAttach();
    _start_timer(resolveVisibleModelNodeInstances);
    // resolve visible ModelNodeInstances
    this->ResolveVisibleModelNodeInstances(frameIndex);
    _stop_timer(resolveVisibleModelNodeInstances);  
    Particles::ParticleRenderer::Instance()->EndAttach();

    // render the world...
    this->frameShader->Render();

    // tell light and shadow servers that rendering is finished
    shadowServer->EndFrame();
    lightServer->EndFrame();
}

//------------------------------------------------------------------------------
/**
    Renders a debug visualization. Can be called alone or after Render().
*/
void
InternalView::RenderDebug()
{
    n_assert(this->frameShader.isvalid());

    // setup global transforms...
    TransformDevice* transformDevice = TransformDevice::Instance();
    transformDevice->SetProjTransform(this->camera->GetProjTransform());
    transformDevice->SetViewTransform(this->camera->GetViewTransform());

    // just call OnRenderDebug on all graphics entities visible through our camera
    const Array<Ptr<InternalGraphicsEntity> >& visLinks = this->camera->GetLinks(InternalGraphicsEntity::CameraLink);
    IndexT i;
    SizeT num = visLinks.Size();
    for (i = 0; i < num; i++)
    {
        const Ptr<InternalGraphicsEntity>& curEntity = visLinks[i];
        curEntity->OnRenderDebug();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InternalView::SetCameraEntity(const Ptr<InternalCameraEntity>& newCameraEntity)
{
    // assert if cam is not attached to a stage
    n_assert2(!newCameraEntity.isvalid() || newCameraEntity->IsAttachedToStage(), "Camera has to be attached to a stage first!");
    if (this->camera.isvalid())
    {
        this->camera->OnRemoveFromView(this);
        this->camera = 0;
    }
    if (newCameraEntity.isvalid())
    {
        this->camera = newCameraEntity;
        this->camera->OnAttachToView(this);
    }
}

} // namespace Graphics
