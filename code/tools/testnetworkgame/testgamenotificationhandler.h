#pragma once
//------------------------------------------------------------------------------
/**
    @class Multiplayer::TestGameNotificationHandler

    (C) 2009 Radon Labs GmbH
*/
#include "multiplayerfeature/distributionnotificationhandler.h"
#include "notificationprotocol.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestGameNotificationHandler : public MultiplayerFeature::DistributionNotificationHandler
{
    __DeclareClass(TestGameNotificationHandler);
public:
    /// constructor
    TestGameNotificationHandler();
    /// destructor
    virtual ~TestGameNotificationHandler();        

protected:   
    /// handle game starting message, host invoked a game start, load level etc
    virtual void HandleGameStarting(const Ptr<Multiplayer::GameStarting>& msg); 
    /// handle player left session msg
    virtual void HandlePlayerLeftSession(const Ptr<Multiplayer::PlayerLeftSession>& msg);     
    /// handle player joined session msg
    virtual void HandlePlayerJoinedSession(const Ptr<Multiplayer::PlayerJoinedSession>& msg);
    /// handle game started message: Game started on each player in session, we are ready to run gameplay
    virtual void HandleGameStarted(const Ptr<Multiplayer::GameStarted>& msg);
};
}; // namespace Multiplayer
//------------------------------------------------------------------------------