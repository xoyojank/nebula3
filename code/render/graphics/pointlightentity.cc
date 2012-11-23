//------------------------------------------------------------------------------
//  pointlightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/pointlightentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::PointLightEntity, 'ASLP', Graphics::AbstractLightEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
PointLightEntity::PointLightEntity()
{
    this->SetLightType(Lighting::LightType::Point);
}

//------------------------------------------------------------------------------
/**
*/
void
PointLightEntity::Setup(const Ptr<Stage>& stage_)
{
    // call parent class
    AbstractLightEntity::Setup(stage_);

    // setup and send entity creation message
    Ptr<CreatePointLightEntity> msg = CreatePointLightEntity::Create();
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
PointLightEntity::SetTransformFromPosDirAndRange(const point& pos, const vector& dir, float range)
{
    matrix44 m = matrix44::lookatrh(pos, pos + dir, vector::upvec());
    float4 scale = float4(range, range, range, 1.0f);
    m.setrow0(float4::multiply(m.getrow0(), scale));
    m.setrow1(float4::multiply(m.getrow1(), scale));
    m.setrow2(float4::multiply(m.getrow2(), scale));
    this->SetTransform(m);
}

} // namespace Graphics
