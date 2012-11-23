//------------------------------------------------------------------------------
//  internalcameraentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalcameraentity.h"
#include "coregraphics/displaydevice.h"
#include "internalgraphics/internalview.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalCameraEntity, 'ICME', InternalGraphics::InternalGraphicsEntity);

using namespace Math;
using namespace Messaging;
using namespace CoreGraphics;
using namespace Shared;

//------------------------------------------------------------------------------
/**
*/
InternalCameraEntity::InternalCameraEntity()
{
    this->SetType(InternalGraphicsEntityType::Camera);
    float aspectRatio = DisplayDevice::Instance()->GetDisplayMode().GetAspectRatio();
    this->camSettings.SetupPerspectiveFov(n_deg2rad(60.0f), aspectRatio, 0.1f, 2500.0f);
}

//------------------------------------------------------------------------------
/**
*/
InternalCameraEntity::~InternalCameraEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalCameraEntity::IsAttachedToView() const
{
    return this->view.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalCameraEntity::OnAttachToView(const Ptr<InternalView>& v)
{
    n_assert(!this->view.isvalid());
    this->view = v;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalCameraEntity::OnRemoveFromView(const Ptr<InternalView>& v)
{
    n_assert(this->view.isvalid() && this->view == v);
    this->view = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalCameraEntity::OnDeactivate()
{
    // if we're currently attached to a view, we need to detach first
    if (this->IsAttachedToView())
    {
        this->view->SetCameraEntity(0);
    }
    InternalGraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    We need to keep track of modifications of the transformation matrix.
*/
void
InternalCameraEntity::OnTransformChanged()
{
    this->camSettings.UpdateViewMatrix(matrix44::inverse(this->GetTransform()));
    InternalGraphicsEntity::OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
    Computes the clip status of a bounding box in global space against
    the view volume of this camera entity.
*/    
ClipStatus::Type
InternalCameraEntity::ComputeClipStatus(const bbox& box)
{
    ClipStatus::Type clipStatus = box.clipstatus(this->camSettings.GetViewProjTransform());
    return clipStatus;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalCameraEntity::SetCameraSettings(const CameraSettings& settings)
{
    this->camSettings = settings;
    this->camSettings.UpdateViewMatrix(matrix44::inverse(this->GetTransform()));
}

//------------------------------------------------------------------------------
/**
    Handle a message, override this method accordingly in subclasses!
*/
void
InternalCameraEntity::HandleMessage(const Ptr<Message>& msg)
{
    __Dispatch(InternalCameraEntity, this, msg);
}

} // namespace InternalGraphics
