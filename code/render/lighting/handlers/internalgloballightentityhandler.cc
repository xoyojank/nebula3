//------------------------------------------------------------------------------
//  internalgloballightentityhandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "lighting/internalgloballightentity.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalstage.h"

using namespace Util;
using namespace Graphics;
using namespace InternalGraphics;
using namespace Lighting;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(CreateGlobalLightEntity)
{
    // create a new global light entity
    Ptr<Lighting::InternalGlobalLightEntity> lightEntity = Lighting::InternalGlobalLightEntity::Create();
    lightEntity->SetSharedData(msg->GetSharedData());
    lightEntity->SetTransform(msg->GetTransform());
    lightEntity->SetVisible(msg->GetVisible());
    lightEntity->SetColor(msg->GetLightColor());
    lightEntity->SetBackLightColor(msg->GetBackLightColor());
    lightEntity->SetCastShadows(msg->GetCastShadows());
    lightEntity->SetProjMapUvOffsetAndScale(msg->GetProjMapUvOffsetAndScale());
    lightEntity->SetAmbientLightColor(msg->GetAmbientLightColor());
    lightEntity->SetBackLightOffset(msg->GetBackLightOffset());

    // lookup stage and attach entity
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    stage->AttachEntity(lightEntity.cast<InternalGraphicsEntity>());

    // set return values
    msg->GetObjectRef()->Validate<Lighting::InternalGlobalLightEntity>(lightEntity.get());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGlobalLightEntity, SetGlobalBackLightColor)
{
    obj->SetBackLightColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGlobalLightEntity, SetGlobalAmbientLightColor)
{
    obj->SetAmbientLightColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGlobalLightEntity, SetGlobalBackLightOffset)
{
    obj->SetBackLightOffset(msg->GetOffset());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGlobalLightEntity, SetGlobalLightParams)
{
    obj->SetColor(msg->GetLightColor());
    obj->SetBackLightColor(msg->GetBackLightColor());
    obj->SetAmbientLightColor(msg->GetAmbientLightColor());
    obj->SetBackLightOffset(msg->GetBackLightOffset());
    obj->SetCastShadows(msg->GetCastShadows());
}

//------------------------------------------------------------------------------
/**
*/
__Dispatcher(InternalGlobalLightEntity)
{
    __Handle(InternalGlobalLightEntity, SetGlobalBackLightColor);
    __Handle(InternalGlobalLightEntity, SetGlobalAmbientLightColor);
    __Handle(InternalGlobalLightEntity, SetGlobalBackLightOffset);
    __Handle(InternalGlobalLightEntity, SetGlobalLightParams);
    __HandleUnknown(InternalAbstractLightEntity);
}

} // namespace Messaging