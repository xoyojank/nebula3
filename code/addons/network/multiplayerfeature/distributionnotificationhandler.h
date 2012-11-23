#pragma once
//------------------------------------------------------------------------------
/**
    @class Multiplayer::DistributionNotificationHandler

    (C) 2009 Radon Labs GmbH
*/
#include "multiplayer/defaultmultiplayernotificationhandler.h"
#include "notificationprotocol.h"

//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class DistributionNotificationHandler : public Multiplayer::DefaultMultiplayerNotificationHandler
{
    __DeclareClass(DistributionNotificationHandler);
public:
    /// constructor
    DistributionNotificationHandler();
    /// destructor
    virtual ~DistributionNotificationHandler();        

protected:   
    /// handle a single multiplayer Notification message
    virtual void HandleNotification(const Ptr<Messaging::Message>& msg);
    /// handle data received message
    virtual void HandleDataReceived(const Ptr<Multiplayer::DataReceived>& msg);
};
}; // namespace Multiplayer
//------------------------------------------------------------------------------