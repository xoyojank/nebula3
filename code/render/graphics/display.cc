//------------------------------------------------------------------------------
//  display.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/display.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"

namespace Graphics
{
__ImplementClass(Graphics::Display, 'DISP', Core::RefCounted);
__ImplementSingleton(Graphics::Display);

using namespace Util;
using namespace CoreGraphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
Display::Display() :
    isOpen(false),
    parentWindow(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Display::~Display()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Display::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->resourceMappers.IsEmpty());

    Ptr<Graphics::SetupGraphics> msg = Graphics::SetupGraphics::Create();
    msg->SetAdapter(this->settings.GetAdapter());
    msg->SetDisplayMode(this->settings.DisplayMode());
    msg->SetAntiAliasQuality(this->settings.GetAntiAliasQuality());
    msg->SetFullscreen(this->settings.IsFullscreen());
	msg->SetAutoAdjustSize(this->settings.IsAutoAdjustSize());
    msg->SetDisplayModeSwitchEnabled(this->settings.IsDisplayModeSwitchEnabled());
    msg->SetTripleBufferingEnabled(this->settings.IsTripleBufferingEnabled());
    msg->SetAlwaysOnTop(this->settings.IsAlwaysOnTop());
    msg->SetVerticalSyncEnabled(this->settings.IsVerticalSyncEnabled());
    msg->SetIconName(this->settings.GetIconName());
    msg->SetWindowTitle(this->settings.GetWindowTitle());
    msg->SetParentWindow(this->parentWindow);
    msg->SetResourceMappers(this->resourceMappers);

    // clear resource mappers array, so there's no danger of accessing them
    // from outside the render thread
    this->resourceMappers.Clear();
    
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());

    // update our display mode with the actual display mode
    this->settings.DisplayMode() = msg->GetActualDisplayMode();
    this->settings.SetFullscreen(msg->GetActualFullscreen());
    this->settings.SetAdapter(msg->GetActualAdapter());
    
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
Display::Close()
{
    n_assert(this->isOpen);

    // FIXME: hmm, closing/reopening the display not currently supported
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Display::AdapterExists(CoreGraphics::Adapter::Code adapter)
{
    Ptr<Graphics::AdapterExists> msg = Graphics::AdapterExists::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
Array<DisplayMode>
Display::GetAvailableDisplayModes(Adapter::Code adapter, PixelFormat::Code pixelFormat)
{
    Ptr<Graphics::GetAvailableDisplayModes> msg = Graphics::GetAvailableDisplayModes::Create();
    msg->SetAdapter(adapter);
    msg->SetPixelFormat(pixelFormat);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
bool
Display::SupportsDisplayMode(Adapter::Code adapter, const DisplayMode& requestedMode)
{
    Ptr<Graphics::SupportsDisplayMode> msg = Graphics::SupportsDisplayMode::Create();
    msg->SetAdapter(adapter);
    msg->SetDisplayMode(requestedMode);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
void
Display::AdjustSize()
{
	Ptr<Graphics::AdjustDisplaySize> msg = Graphics::AdjustDisplaySize::Create();
	GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
/**
*/
DisplayMode
Display::GetCurrentAdapterDisplayMode(Adapter::Code adapter)
{
    Ptr<Graphics::GetCurrentAdapterDisplayMode> msg = Graphics::GetCurrentAdapterDisplayMode::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
AdapterInfo
Display::GetAdapterInfo(Adapter::Code adapter)
{
    Ptr<Graphics::GetAdapterInfo> msg = Graphics::GetAdapterInfo::Create();
    msg->SetAdapter(adapter);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
void
Display::AttachDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::AttachDisplayEventHandler> msg = Graphics::AttachDisplayEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::RemoveDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::RemoveDisplayEventHandler> msg = Graphics::RemoveDisplayEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::AttachRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::AttachRenderEventHandler> msg = Graphics::AttachRenderEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
Display::RemoveRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<Graphics::RemoveRenderEventHandler> msg = Graphics::RemoveRenderEventHandler::Create();
    msg->SetHandler(handler);
    GraphicsInterface::Instance()->Send(msg.cast<Message>());
}

} // namespace Graphics
