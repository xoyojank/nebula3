//------------------------------------------------------------------------------
//  network/multiplayer/distributionmanager.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/distributionmanager.h"   
#include "multiplayerfeature/distributionproperty.h"

namespace MultiplayerFeature
{
__ImplementClass(DistributionManager, 'DSHM', Core::RefCounted);
__ImplementSingleton(DistributionManager);

//------------------------------------------------------------------------------
/**
*/
DistributionManager::DistributionManager() : 
    currentMode(Idle),
    requestedMode(Idle),
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DistributionManager::~DistributionManager()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
DistributionManager::Open()
{
    n_assert(!this->isOpen);
	this->isOpen = true;
    
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DistributionManager::Close()
{
    n_assert(this->isOpen);
  
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionManager::HandleDataStream( const Ptr<InternalMultiplayer::NetStream>& stream )
{
    n_assert(this->distributionSystem.isvalid());
    this->distributionSystem->HandleDataStream(stream);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionManager::OnBeginFrame()
{
    if (Idle != this->currentMode)
    {
        n_assert(this->distributionSystem.isvalid());
        this->distributionSystem->OnBeginFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionManager::OnFrame()
{    
    if (Idle != this->currentMode)
    {
        n_assert(this->distributionSystem.isvalid());
        this->distributionSystem->OnFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionManager::OnEndFrame()
{   
    if (Idle != this->currentMode)
    {
        // trigger 
        if (this->distributionSystem.isvalid())
        {
            n_assert(this->currentMode != Idle);
            this->distributionSystem->OnEndFrame();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionManager::SwitchMode()
{
    // close
    if (this->distributionSystem.isvalid())
    {
        this->distributionSystem->Close();
        this->distributionSystem = 0;
    }

    // create new
    if (Host == this->requestedMode)
    {   
        Ptr<MultiplayerFeature::DistributionHost> hostMode = MultiplayerFeature::DistributionHost::Create();
        this->distributionSystem = hostMode.cast<Base::DistributionSystem>();
    }
    else if (Client == this->requestedMode)
    {
        Ptr<MultiplayerFeature::DistributionClient> clientMode = MultiplayerFeature::DistributionClient::Create();
        this->distributionSystem = clientMode.cast<Base::DistributionSystem>();            
    }
    else if (Idle == this->requestedMode)
    {
        this->currentMode = this->requestedMode;
        return;
    }

    this->distributionSystem->Open();
    this->currentMode = this->requestedMode;
}
} // namespace MultiplayerFeature