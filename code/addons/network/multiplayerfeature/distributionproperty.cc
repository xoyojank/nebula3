//------------------------------------------------------------------------------
//  properties/Multiplayerproperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerfeature/distributionproperty.h"
#include "game/entity.h"
#include "multiplayerfeature/distributionmanager.h"
#include "multiplayerfeature/objectview.h"
#include "distributionprotocol.h"

namespace Attr  
{
    DefineBoolWithDefault(IsMaster, 'ISPE', ReadWrite, true);
};

namespace MultiplayerFeature
{
__ImplementClass(MultiplayerFeature::DistributionProperty, 'MPPR', Game::Property);

using namespace Game;

//------------------------------------------------------------------------------
/**
*/
DistributionProperty::DistributionProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DistributionProperty::~DistributionProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::OnActivate()
{
    Property::OnActivate();

    // all attributes to be tracked will be added in this function on derived class
    this->AddMonitoredAttributes();

    DistributionManager::Mode mode = DistributionManager::Instance()->GetCurrentMode();
    if (mode == DistributionManager::Host)
    {                    
        // create any object views
        this->SetupHostObjectView(); 
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::OnDeactivate()
{
    Property::OnDeactivate();

    // cleanup any object views
    this->DestroyObjectView();
}       

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::SetupDefaultAttributes()
{
    SetupAttr(Attr::IsMaster);
    Game::Property::SetupDefaultAttributes();
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render); 
    this->entity->RegisterPropertyCallback(this, RenderDebug);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::OnRender()
{
    // Choose operations depended on mode (master = playerControlled)
    if (this->GetEntity()->GetBool(Attr::IsMaster))
    {
        if (this->HasMasterOperations())
        {
            this->DoMasterOperations();
        }
    }
    else
    {
        if (this->HasSlaveOperations())
        {
            this->DoSlaveOperations();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::DoMasterOperations()
{
    // if needed: overwrite in subclass and let HasMasterOperations return true
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::DoSlaveOperations()
{
    // if needed: overwrite in subclass and let HasSlaveOperations return true
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::SetupHostObjectView()
{
    n_assert(!this->objectView.isvalid());
    this->objectView = this->CreateObjectView();
    this->objectView->Initialize((MultiplayerFeature::ObjectView::ObjectViewId)InvalidIndex, this->GetEntity());   
    n_assert(this->objectView->GetObjectViewId() != InvalidIndex);
    
    IndexT i;
    for (i = 0; i < this->monitoredAttributes.Size(); ++i)
    {
        MonitorAttrEntry& entry = this->monitoredAttributes[i];
        this->objectView->RegisterAttribute(this->GetEntity(), entry.id, entry.initOnly);    	
    }
}     

//------------------------------------------------------------------------------
/**
*/
bool 
DistributionProperty::SetupClientObjectView(ObjectView::ObjectViewId id)
{
    if (!this->objectView.isvalid())
    {
        this->objectView = this->CreateObjectView();
        this->objectView->Initialize(id, this->GetEntity());       
        n_assert(this->objectView->GetObjectViewId() != InvalidIndex);

        IndexT i;
        for (i = 0; i < this->monitoredAttributes.Size(); ++i)
        {
            MonitorAttrEntry& entry = this->monitoredAttributes[i];
            this->objectView->RegisterAttribute(this->GetEntity(), entry.id, entry.initOnly);    	
        }

        return true;
    }
    else
    {
        n_assert(this->objectView->GetObjectViewId() != id);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::AddMonitoredAttributes()
{
    // overwrite in subclass and add here any attributes to monitor
}

//------------------------------------------------------------------------------
/**
*/
Ptr<MultiplayerFeature::ObjectView> 
DistributionProperty::CreateObjectView()
{
    // overwrite in subclass for yout own object view class
    return ObjectView::Create();
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::DestroyObjectView()
{
    if (this->objectView.isvalid())
    {   
        this->objectView->Clear();
    }   
    this->objectView = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(MultiplayerFeature::CreateObjectView::Id);
}

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(MultiplayerFeature::CreateObjectView::Id))        
    {
        if (!msg->Handled())
        {
            const Ptr<MultiplayerFeature::CreateObjectView>& createObjVMsg = msg.cast<MultiplayerFeature::CreateObjectView>();
            bool obvCreated = this->SetupClientObjectView(createObjVMsg->GetObjectViewId());
            msg->SetHandled(obvCreated);
        }
    }
}     

//------------------------------------------------------------------------------
/**
*/
void 
DistributionProperty::OnRenderDebug()
{
    if (this->HasObjectView())
    {
        this->GetObjectView()->RenderDebug();
    }
}
}; // namespace Properties
