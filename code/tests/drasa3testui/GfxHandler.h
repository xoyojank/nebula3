#pragma once
//------------------------------------------------------------------------------
/**
    @class SUI::SUIHandler
    
    Render-thread message handler for the SUI subsystem.
    
    (C) 2009 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "Gfxprotocol.h"

//------------------------------------------------------------------------------
namespace ScaleForms
{
class GfxHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(GfxHandler);
public:
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    ///// do per-frame work
    //virtual void DoWork();

private:
    /// handle ProcessEvents message
    void OnProcessEvents(const Ptr<ProcessEvents>& msg);
};

} // namespace SUI
//------------------------------------------------------------------------------
