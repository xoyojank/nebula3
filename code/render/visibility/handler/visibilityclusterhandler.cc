//------------------------------------------------------------------------------
//  internalgraphicshandler.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "visibility/visibilityprotocol.h"
#include "messaging/staticmessagehandler.h"
#include "visibility/visibilitysystems/visibilitycluster.h"
#include "visibility/visibilitychecker.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalgraphicsserver.h"
                          
using namespace Util;
using namespace Visibility;
using namespace InternalGraphics;

namespace Messaging
{

//------------------------------------------------------------------------------
/**
*/
__StaticHandler(CreateVisibilityCluster)
{
    const Ptr<InternalStage>& stage = InternalGraphicsServer::Instance()->GetStageByName(msg->GetStageName());
    Visibility::VisibilityChecker& visChecker = stage->GetVisibilityChecker();
    // get references to its internal entities
    Util::Array<Ptr<InternalGraphicsEntity> > internalEntities;
    IndexT i;
    for (i = 0; i < msg->GetEntities().Size(); ++i)
    {
        internalEntities.Append(msg->GetEntities()[i]->GetObjectRef()->Ref<InternalGraphicsEntity>());    	
    }     

    //    msg->SetResult(DisplayDevice::Instance()->AdapterExists(msg->GetAdapter()));
    Ptr<VisibilityCluster> cluster = VisibilityCluster::Create();
    cluster->SetGraphicsEntities(internalEntities);
    cluster->SetBoxTransforms(msg->GetBoundingBoxes());
    visChecker.BeginAttachVisibilityContainer();
    visChecker.AttachVisibilityContainer(cluster.cast<VisibilityContainer>());
    visChecker.EndAttachVisibilityContainer();
}

} // namespace Messaging

