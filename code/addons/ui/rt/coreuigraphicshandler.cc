//------------------------------------------------------------------------------
//  uigraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/rt/coreuigraphicshandler.h"
#include "ui/rt/coreuirenderer.h"
#include "ui/uirenderprotocol.h"

namespace CoreUI
{
__ImplementClass(CoreUIGraphicsHandler, 'UIGH', Interface::InterfaceHandlerBase);

using namespace Util;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
CoreUIGraphicsHandler::CoreUIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CoreUIGraphicsHandler::~CoreUIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CoreUIGraphicsHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(RenderUIElements::Id))
    {
        this->OnRenderUIElements(msg.cast<RenderUIElements>());
    }
    else if (msg->CheckId(LoadUITexture::Id))
    {
        this->OnLoadUITexture(msg.cast<LoadUITexture>());
    }
    else if (msg->CheckId(UnloadUITexture::Id))
    {
        this->OnUnloadUITexture(msg.cast<UnloadUITexture>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIGraphicsHandler::OnRenderUIElements(const Ptr<RenderUIElements>& msg)
{	
	CoreUI::CoreUIRenderer::Instance()->SetElements(msg->GetUIElements());
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIGraphicsHandler::OnLoadUITexture(const Ptr<LoadUITexture>& msg)
{
    CoreUI::CoreUIRenderer::Instance()->LoadTexture(msg->GetResourceName());
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIGraphicsHandler::OnUnloadUITexture(const Ptr<UnloadUITexture>& msg)
{
    CoreUI::CoreUIRenderer::Instance()->UnloadTexture(msg->GetResourceName());
}

} // namespace Debug
