//------------------------------------------------------------------------------
//  multiplayer/multiplayerfeatureunit.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/multiplayerfeatureunit.h"
#include "multiplayerfeature/distributionnotificationhandler.h"
#include "basegamefeature/basegametiming/gametimesource.h"

using namespace InternalMultiplayer;
using namespace Multiplayer;

namespace MultiplayerFeature
{
__ImplementClass(MultiplayerFeatureUnit, 'MUFU' , Game::FeatureUnit);
__ImplementSingleton(MultiplayerFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
MultiplayerFeatureUnit::MultiplayerFeatureUnit():
notificationHandlerRtti(&DistributionNotificationHandler::RTTI)
{	
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
MultiplayerFeatureUnit::~MultiplayerFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();

    // setup multiplayer thread
    this->internalMpInterface = InternalMultiplayerInterface::Create();
    this->internalMpInterface->Open();

    // setup manager
    this->multiplayerManager = Multiplayer::MultiplayerManager::Create();
    this->multiplayerManager->Open();	

    // create and attach default handler
    Ptr<DistributionNotificationHandler> handler = (DistributionNotificationHandler*)this->notificationHandlerRtti->Create();
    this->notificationHandler.Append(handler.cast<Base::MultiplayerNotificationHandlerBase>());
    this->multiplayerManager->RegisterNotificationHandler(handler.cast<Base::MultiplayerNotificationHandlerBase>());

    // distribution
    this->distributionManager = DistributionManager::Create();
    this->distributionManager->Open();

    this->SetRenderDebug(true);
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnDeactivate()
{
    this->multiplayerManager->Close();
    this->multiplayerManager = 0;

    this->internalMpInterface->Close();
    this->internalMpInterface = 0;

    this->distributionManager->Close();
    this->distributionManager = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnRenderDebug()
{
	this->multiplayerManager->RenderDebug();

	FeatureUnit::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnBeginFrame()
{
    IndexT index;
    for (index = 0; index < this->notificationHandler.Size(); index++)
    {
        this->notificationHandler[index]->HandlePendingNotifications();
    }

    // trigger distribution
    this->distributionManager->OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnFrame()
{    
    // trigger multiplayer stuff
    this->multiplayerManager->Trigger();

    this->distributionManager->OnFrame();

    FeatureUnit::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
MultiplayerFeatureUnit::OnEndFrame()
{
    this->distributionManager->OnEndFrame();

    // flush all messages
    this->internalMpInterface->FlushBatchedMessages();
    
    Game::FeatureUnit::OnEndFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
MultiplayerFeatureUnit::SetNotificationHandlerRtti(Core::Rtti* rtti)
{
    this->notificationHandlerRtti = rtti;
}
}; // namespace Multiplayer
