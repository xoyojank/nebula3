//------------------------------------------------------------------------------
//  network/multiplayer/DistributionNotificationHandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/distributionnotificationhandler.h"
#include "multiplayerfeature/distributionmanager.h"
#include "multiplayer/multiplayermanager.h"

namespace MultiplayerFeature
{
__ImplementClass(DistributionNotificationHandler, 'DINH', Multiplayer::DefaultMultiplayerNotificationHandler);

using namespace Multiplayer;
//------------------------------------------------------------------------------
/**
*/
DistributionNotificationHandler::DistributionNotificationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DistributionNotificationHandler::~DistributionNotificationHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionNotificationHandler::HandleDataReceived(const Ptr<DataReceived>& msg)
{
    // pass to manager
    DistributionManager::Instance()->HandleDataStream(msg->GetStream());
} 

//------------------------------------------------------------------------------
/**
*/
void 
DistributionNotificationHandler::HandleNotification(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(HostSessionSucceeded::Id))
    {
        DistributionManager::Instance()->RequestMode(DistributionManager::Host);
    }
    else if (msg->CheckId(JoinSessionSucceeded::Id))
    {
        DistributionManager::Instance()->RequestMode(DistributionManager::Client);
    }
    DefaultMultiplayerNotificationHandler::HandleNotification(msg);
}
} // namespace Multiplayer
