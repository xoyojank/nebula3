//------------------------------------------------------------------------------
//  internalgraphicsentityhandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "internalgraphics/internalstage.h"
#include "models/nodes/statenodeinstance.h"
#include "models/nodes/animatornodeinstance.h"
#include "renderutil/nodelookuputil.h"

using namespace CoreGraphics;
using namespace Models;
using namespace InternalGraphics;
using namespace Graphics;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGraphicsEntity, DiscardGraphicsEntity)
{
    Ptr<InternalStage> stage = obj->GetStage();
    n_assert(stage.isvalid());
    stage->RemoveEntity(obj);    
    msg->GetObjectRef()->Invalidate();
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGraphicsEntity, UpdateTransform)
{
    obj->SetTransform(msg->GetTransform());
}

//------------------------------------------------------------------------------
/**
*/
__Handler(InternalGraphicsEntity, SetVisibility)
{
    obj->SetVisible(msg->GetVisible());
}

//------------------------------------------------------------------------------
/**
    Dispatcher method (must be positioned after the handler methods to
    prevent automatic instantiation).
*/
__Dispatcher(InternalGraphicsEntity)
{
    __Handle(InternalGraphicsEntity, UpdateTransform);
    __Handle(InternalGraphicsEntity, SetVisibility);
    __Handle(InternalGraphicsEntity, DiscardGraphicsEntity);
    __HandleUnknown(Core::RefCounted);
}

} // namespace Messaging