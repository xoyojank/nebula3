//------------------------------------------------------------------------------
//  globallightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/globallightentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::GlobalLightEntity, 'GLEP', Graphics::AbstractLightEntity);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
GlobalLightEntity::GlobalLightEntity() :
    backLightColor(0.0f, 0.0f, 0.0f, 0.0f),
    ambientLightColor(0.0f,0.0f,0.0f,1.0f),
    backLightOffset(0.0f)
{
    this->SetLightType(Lighting::LightType::Global);
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalLightEntity::Setup(const Ptr<Stage>& stage_)
{
    // call parent class
    AbstractLightEntity::Setup(stage_);

    // setup and send entity creation message
    Ptr<CreateGlobalLightEntity> msg = CreateGlobalLightEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetLightColor(this->color);
    msg->SetProjMapUvOffsetAndScale(this->projMapUvOffsetAndScale);
    msg->SetCastShadows(this->castShadows);
    msg->SetBackLightColor(this->backLightColor);
    msg->SetBackLightOffset(this->backLightOffset);
    msg->SetAmbientLightColor(this->ambientLightColor);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalLightEntity::SetBackLightColor(const float4& c)
{
    this->backLightColor = c;
    if (this->IsValid())
    {
        Ptr<SetGlobalBackLightColor> msg = SetGlobalBackLightColor::Create();
        msg->SetColor(c);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GlobalLightEntity::SetAmbientLightColor(const float4& c)
{
    this->ambientLightColor = c;
    if (this->IsValid())
    {
        Ptr<SetGlobalAmbientLightColor> msg = SetGlobalAmbientLightColor::Create();
        msg->SetColor(c);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GlobalLightEntity::SetBackLightOffset(float val)
{
    this->backLightOffset = val;
    if (this->IsValid())
    {
        Ptr<SetGlobalBackLightOffset> msg = SetGlobalBackLightOffset::Create();
        msg->SetOffset(val);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalLightEntity::SetAllParams(const Math::float4& color, const Math::float4& backColor, const Math::float4& ambColor, float backOffset, bool castShdws)
{
    this->color = color;
    this->backLightColor = backColor;
    this->ambientLightColor = ambColor;
    this->backLightOffset = backOffset;
    this->castShadows = castShdws;

    if (this->IsValid())
    {
        Ptr<SetGlobalLightParams> msg = SetGlobalLightParams::Create();
        msg->SetLightColor(color);
        msg->SetBackLightColor(backColor);
        msg->SetAmbientLightColor(ambColor);
        msg->SetBackLightOffset(backOffset);
        msg->SetCastShadows(castShdws);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

} // namespace Graphics
