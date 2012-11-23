//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uirenderer.h"
#include "ui/uirenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "threading/thread.h"

namespace UI
{
__ImplementClass(UIRenderer, 'UIRR', Core::RefCounted);
__ImplementSingleton(UIRenderer);

using namespace Util;
using namespace Math;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
UIRenderer::UIRenderer() :
    isValid(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIRenderer::~UIRenderer()
{
    n_assert(!this->IsValid());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::Setup()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
    this->texUseCounts.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::DrawElement(const StringAtom& name, IndexT id, const Math::rectangle<float>& rect, const Math::float4& uv, int depth, const Math::float4& color, const Resources::ResourceId& texResId)
{
    n_assert(this->IsValid());
	CoreUI::CoreUIElement element;
    element.SetName(name);
    element.SetColor(color);
    element.SetUV(uv);
    element.SetDimension(rect);
    element.SetDepth(depth);
    element.SetElementId(id);
    element.SetTextureResId(texResId);
    this->elements.Append(element);
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::OnFrame()
{
    n_assert(this->IsValid());

    // send display list to render thread
    Ptr<CoreUI::RenderUIElements> msg = CoreUI::RenderUIElements::Create();
    msg->SetUIElements(this->elements);
    Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());

    // clear elements
    this->elements.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::RegisterTexture(const Resources::ResourceId& resId)
{
    n_assert(this->IsValid());
    if (this->texUseCounts.Contains(resId))
    {
        this->texUseCounts[resId]++;
    }
    else
    {
        // new texture, add entry and send load message
        this->texUseCounts.Add(resId, 1);
        Ptr<CoreUI::LoadUITexture> loadTexMsg = CoreUI::LoadUITexture::Create();
        loadTexMsg->SetResourceName(resId);
        Graphics::GraphicsInterface::Instance()->Send(loadTexMsg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::UnregisterTexture(const Resources::ResourceId& resId)
{
    n_assert(this->IsValid());
    n_assert(this->texUseCounts.Contains(resId));
    n_assert(this->texUseCounts[resId] > 0);
    this->texUseCounts[resId]--;
    if (0 == this->texUseCounts[resId])
    {
        // discard entry and send unload message
        this->texUseCounts.Erase(resId);
        Ptr<CoreUI::UnloadUITexture> unloadTexMsg = CoreUI::UnloadUITexture::Create();
        unloadTexMsg->SetResourceName(resId);
        Graphics::GraphicsInterface::Instance()->Send(unloadTexMsg);
    }
}

} // namespace Debug
