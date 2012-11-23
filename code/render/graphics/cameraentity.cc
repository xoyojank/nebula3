//------------------------------------------------------------------------------
//  cameraentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/cameraentity.h"
#include "graphics/display.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::CameraEntity, 'GCET', Graphics::GraphicsEntity);

using namespace Util;
using namespace Math;
using namespace Shared;

//------------------------------------------------------------------------------
/**
*/
CameraEntity::CameraEntity()
{
    this->SetType(GraphicsEntityType::Camera);
    float aspectRatio = Display::Instance()->Settings().DisplayMode().GetAspectRatio();
    this->settings.SetupPerspectiveFov(n_deg2rad(60.0f), aspectRatio, 0.1f, 2500.0f);
}

//------------------------------------------------------------------------------
/**
*/
CameraEntity::~CameraEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup the server-side camera entity.
*/
void
CameraEntity::Setup(const Ptr<Stage>& stage_)
{
    GraphicsEntity::Setup(stage_);

    // send off an entity creation message
    Ptr<CreateCameraEntity> msg = CreateCameraEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetCameraSettings(this->settings);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
    Property remove from view if we are attached to a view.
*/
void
CameraEntity::Discard()
{
    if (this->IsAttachedToView())
    {
        this->view->SetCameraEntity(0);
    }
    // call parent
    GraphicsEntity::Discard();
}

//------------------------------------------------------------------------------
/**
    Set new the camera settings. This updates the internal matrices.
*/
void
CameraEntity::SetCameraSettings(const CameraSettings& camSettings)
{
    this->settings = camSettings;
    this->settings.UpdateViewMatrix(matrix44::inverse(this->GetTransform()));
    if (this->IsValid())
    {
        // also send an update message
        Ptr<UpdateCameraEntity> msg = UpdateCameraEntity::Create();
        msg->SetCameraSettings(this->settings);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
    We need to keep track of modifications of the transformation matrix.
*/
void
CameraEntity::OnTransformChanged()
{
    // keep track of view matrix
    this->settings.UpdateViewMatrix(matrix44::inverse(this->GetTransform()));
    GraphicsEntity::OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnAttachToView(const Ptr<View>& view_)
{
    this->view = view_;
    Ptr<AttachCameraToView> msg = AttachCameraToView::Create();
    msg->SetViewName(this->view->GetName());
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
CameraEntity::OnRemoveFromView(const Ptr<View>& view_)
{
    n_assert(this->view == view_);
    Ptr<RemoveCameraFromView> msg = RemoveCameraFromView::Create();
    msg->SetViewName(this->view->GetName());
    this->SendMsg(msg.cast<GraphicsEntityMessage>());
    this->view = 0;
}

} // namespace Graphics
