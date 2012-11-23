//------------------------------------------------------------------------------
//  suihandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "GfxHandler.h"
#include "GfXRender.h"

namespace ScaleForms
{
__ImplementClass(ScaleForms::GfxHandler, 'SUHD', Interface::InterfaceHandlerBase);

using namespace Messaging;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
bool
GfxHandler::HandleMessage(const Ptr<Message>& msg)
{
    if (msg->CheckId(ProcessEvents::Id))
    {        
        this->OnProcessEvents(msg.cast<ProcessEvents>());
    }
    else
    {
        return false;
    }
    return true;    
}

//------------------------------------------------------------------------------
/**
*/
void
GfxHandler::OnProcessEvents(const Ptr<ProcessEvents>& msg)
{
    GfxRender::Instance()->ProcessInputEvents(msg->GetInputEvents());
}
      
} // namespace SUI
