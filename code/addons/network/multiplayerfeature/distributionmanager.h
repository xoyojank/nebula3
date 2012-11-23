#pragma once
//------------------------------------------------------------------------------
/**
    @class MultiplayerFeature::DistributionManager

    This manager is the central object of the "distributed object system" (DOS).
    
    It handles host migration or triggering of the needed distribution system.
    And it triggeres the distrib server or the distrib host.

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "multiplayerfeature/objectview.h"
#include "multiplayerfeature/distributionclient.h"
#include "multiplayerfeature/distributionhost.h"
#include "framesync/framesynctimer.h"

#if !PUBLIC_BUILD
#define NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT (0)
#endif
//------------------------------------------------------------------------------
namespace MultiplayerFeature
{
class DistributionManager : public Core::RefCounted
{
    __DeclareClass(DistributionManager);
    __DeclareSingleton(DistributionManager);

public:
    enum Mode
    {
        Host,
        Client,
        
        Idle
    };

    /// constructor
    DistributionManager();
    /// destructor
    virtual ~DistributionManager();        

    /// open the ui subsystem
    bool Open();
    /// close the ui subsystem
    void Close();
    /// return true if currently open
    bool IsOpen() const;

    /// request mode
    void RequestMode(Mode newMode);
    /// get mode
    Mode GetCurrentMode() const;

    /// on begin frame
    void OnBeginFrame();
    /// on frame
    void OnFrame();
    /// on end frame
    void OnEndFrame();

    /// register objectview
    virtual void RegisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov, MultiplayerFeature::ObjectView::ObjectViewId id);
    /// unregister objectview
    virtual void UnregisterObjectView(const Ptr<MultiplayerFeature::ObjectView>& ov);

    /// pass the data stream to the current distribution system
    void HandleDataStream(const Ptr<InternalMultiplayer::NetStream>& stream);

    /// get distribution system
    const Ptr<Base::DistributionSystem>& GetDistributionSystem() const;

private:                       
    /// switch distribution mode  
    void SwitchMode();

    Ptr<Base::DistributionSystem> distributionSystem;
    Mode currentMode;
    Mode requestedMode;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline void
DistributionManager::RequestMode(Mode newMode)
{
    this->requestedMode = newMode;     

    // check if other mode requested
    if (this->requestedMode != this->currentMode)
    {
        this->SwitchMode();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline DistributionManager::Mode
DistributionManager::GetCurrentMode() const
{
    return this->requestedMode;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DistributionManager::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Base::DistributionSystem>&
DistributionManager::GetDistributionSystem() const
{
    return this->distributionSystem;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
DistributionManager::RegisterObjectView( const Ptr<MultiplayerFeature::ObjectView>& ov, MultiplayerFeature::ObjectView::ObjectViewId id )
{
    n_assert(this->distributionSystem.isvalid());
    this->distributionSystem->RegisterObjectView(ov, id);

#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    n_printf("%i: DistributionManager::RegisterObjectView with id: %i \n", FrameSync::FrameSyncTimer::Instance()->GetTicks(), ov->GetObjectViewId());
#endif
}

//------------------------------------------------------------------------------
/**
*/
inline void 
DistributionManager::UnregisterObjectView( const Ptr<MultiplayerFeature::ObjectView>& ov )
{
    n_assert(this->distributionSystem.isvalid());
#if NEBULA3_DISTRIBUTION_PRINT_DEBUG_OUT
    n_printf("DistributionManager::UnregisterObjectView with id: %i \n", ov->GetObjectViewId());
#endif
    this->distributionSystem->UnregisterObjectView(ov);
}
}; // namespace MultiplayerFeature
//------------------------------------------------------------------------------