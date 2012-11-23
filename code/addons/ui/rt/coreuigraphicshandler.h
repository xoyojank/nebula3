#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreUI::CoreUIGraphicsHandler
    
    Handles visualization of the UI elements.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "ui/uirenderprotocol.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class CoreUIGraphicsHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(CoreUIGraphicsHandler);

public:
    /// constructor
    CoreUIGraphicsHandler();
    /// destructor
    virtual ~CoreUIGraphicsHandler();    
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// handle RenderUIElements message
    void OnRenderUIElements(const Ptr<RenderUIElements>& msg);
    /// handle LoadUITexture message
    void OnLoadUITexture(const Ptr<LoadUITexture>& msg);
    /// handle UnloadUITexture message
    void OnUnloadUITexture(const Ptr<UnloadUITexture>& msg);
};

} // namespace Debug
//------------------------------------------------------------------------------
