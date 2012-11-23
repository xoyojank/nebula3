//------------------------------------------------------------------------------
//  uigraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "fx/rt/fxhandler.h"
#include "fx/fxprotocol.h"
#include "fx/rt/fxserver.h"

namespace FX
{
__ImplementClass(FxHandler, 'CFXH', Interface::InterfaceHandlerBase);

//------------------------------------------------------------------------------
/**
*/
FxHandler::FxHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FxHandler::~FxHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
FxHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(AttachEffectMixer::Id))
    {
        this->OnAttachEffectMixer(msg.cast<AttachEffectMixer>());
    }
    else if (msg->CheckId(AttachEffect::Id))
    {
        this->OnAttachEffect(msg.cast<AttachEffect>());
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
FxHandler::OnAttachEffect(const Ptr<FX::AttachEffect> &msg)
{
    FxServer::Instance()->AttachEffect(msg->GetEffect());
}

//------------------------------------------------------------------------------
/**
*/
void
FxHandler::OnAttachEffectMixer(const Ptr<FX::AttachEffectMixer>& msg)
{
    FxServer::Instance()->AttachEffectMixer(msg->GetEffectMixer());
}
} // namespace Debug
