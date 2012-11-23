//------------------------------------------------------------------------------
//  abstractlightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/abstractlightentity.h"

namespace Graphics
{
__ImplementClass(Graphics::AbstractLightEntity, 'GALE', Graphics::GraphicsEntity);

using namespace Lighting;

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::AbstractLightEntity() :
    lightType(LightType::InvalidLightType),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    projMapUvOffsetAndScale(0.5f, 0.5f, 0.5f, -0.5f),
    castShadows(false)
{
    this->SetType(GraphicsEntityType::Light);
}

//------------------------------------------------------------------------------
/**
*/
AbstractLightEntity::~AbstractLightEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AbstractLightEntity::SetColor(const Math::float4& c)
{
    this->color = c;
    if (this->IsValid())
    {
        Ptr<SetLightColor> msg = SetLightColor::Create();
        msg->SetColor(c);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AbstractLightEntity::SetCastShadows(bool b)
{
    this->castShadows = b;
    if (this->IsValid())
    {
        Ptr<SetLightCastShadows> msg = SetLightCastShadows::Create();
        msg->SetCastShadows(b);
        this->SendMsg(msg.cast<GraphicsEntityMessage>());
    }
}

} // namespace AsyncGraphics