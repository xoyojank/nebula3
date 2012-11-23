//------------------------------------------------------------------------------
//  internalabstractlightentityhandler.cc
//  (C) 2010 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "lighting/internalabstractlightentity.h"

using namespace Graphics;
using namespace Lighting;
using namespace InternalGraphics;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalAbstractLightEntity, SetLightColor)
{
    obj->SetColor(msg->GetColor());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalAbstractLightEntity, SetLightCastShadows)
{
    obj->SetCastShadows(msg->GetCastShadows());
}

//------------------------------------------------------------------------------
/**
*/
__Dispatcher(InternalAbstractLightEntity)
{
    __Handle(InternalAbstractLightEntity, SetLightColor);
    __Handle(InternalAbstractLightEntity, SetLightCastShadows);
    __HandleUnknown(InternalGraphicsEntity);
}

} // namespace Messaging
