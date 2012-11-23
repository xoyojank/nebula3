#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::ForestHandler
    
    Handles debug-visualization messages in the graphics thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacehandlerbase.h"
#include "messaging/message.h"
#include "forest/forestprotocol.h"


//------------------------------------------------------------------------------
namespace Terrain
{
class TerrainHandler : public Interface::InterfaceHandlerBase
{
    __DeclareClass(TerrainHandler);
public:
    /// constructor
    TerrainHandler();
    /// destructor
    virtual ~TerrainHandler();  

    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

};

} // namespace Forest
//------------------------------------------------------------------------------