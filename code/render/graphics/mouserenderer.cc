//------------------------------------------------------------------------------
//  mouserenderer.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/mouserenderer.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"

namespace Graphics
{
__ImplementClass(Graphics::MouseRenderer, 'MSRR', Core::RefCounted);
__ImplementSingleton(Graphics::MouseRenderer);

using namespace Util;
using namespace Resources;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
MouseRenderer::MouseRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
MouseRenderer::~MouseRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
MouseRenderer::PreloadTextures(const Array<ResourceId>& resIds)
{
    Ptr<PreloadMousePointerTextures> msg = PreloadMousePointerTextures::Create();
    msg->SetResourceIds(resIds);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());   
}

//------------------------------------------------------------------------------
/**
*/
void
MouseRenderer::UpdatePointers(const Array<MousePointer>& pointers)
{
    Ptr<UpdateMousePointers> msg = UpdateMousePointers::Create();
    msg->SetPointers(pointers);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

} // namespace Graphics