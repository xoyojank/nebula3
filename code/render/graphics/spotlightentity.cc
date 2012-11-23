//------------------------------------------------------------------------------
//  spotlightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/spotlightentity.h"
#include "graphics/stage.h"
#include "math/polar.h"

namespace Graphics
{
__ImplementClass(Graphics::SpotLightEntity, 'SLEN', Graphics::AbstractLightEntity);

using namespace Math;
//------------------------------------------------------------------------------
/**
*/
SpotLightEntity::SpotLightEntity()
{
    this->SetLightType(Lighting::LightType::Spot);
}

//------------------------------------------------------------------------------
/**
*/
void
SpotLightEntity::Setup(const Ptr<Stage>& stage_)
{
    // call parent class
    AbstractLightEntity::Setup(stage_);

    // setup and send entity creation message
    Ptr<CreateSpotLightEntity> msg = CreateSpotLightEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetLightColor(this->color);
    msg->SetProjMapUvOffsetAndScale(this->projMapUvOffsetAndScale);
    msg->SetCastShadows(this->castShadows);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void 
SpotLightEntity::SetTransformFromPosDirRangeAndCone(const Math::point& pos, const Math::vector& dir, float range, float coneAngle)
{
    const float lightVolumeAngle = 90.0f;
    float coneScale = Math::n_sqrt(coneAngle / lightVolumeAngle);
    matrix44 lightFrustum = matrix44::scaling(vector(range, range, range / coneScale));
    polar dirAngle(dir);
    matrix44 lightTransform = matrix44::rotationyawpitchroll(dirAngle.rho, dirAngle.theta - N_PI * 0.5f, 0);
    lightTransform.set_position(pos);
    lightFrustum = matrix44::multiply(lightFrustum, lightTransform);
    this->SetTransform(lightFrustum);
}
} // namespace Graphics
