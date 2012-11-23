//------------------------------------------------------------------------------
//  animeventmanager.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "managers/animeventmanager.h"
#include "basegametiming/gametimesource.h"
#include "game/entity.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"

namespace BaseGameFeature
{
__ImplementClass(AnimEventManager, 'AEMA', Game::Manager);
__ImplementSingleton(AnimEventManager);

using namespace Util;
using namespace Animation;

//------------------------------------------------------------------------------
/**
*/
AnimEventManager::AnimEventManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AnimEventManager::~AnimEventManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Array<AnimEventInfo>
AnimEventManager::GetAnimEventsByCategory(const Ptr<Game::Entity>& entity, const Util::StringAtom& category)
{
    Array<AnimEventInfo> result;

    // lookup ModelEntity
    Ptr<GraphicsFeature::GetGraphicsEntities> getGfxMsg = GraphicsFeature::GetGraphicsEntities::Create();
    entity->SendSync(getGfxMsg.cast<Messaging::Message>());
    Ptr<Graphics::ModelEntity> modelEntity = getGfxMsg->GetEntities()[0].downcast<Graphics::ModelEntity>();
    n_assert(modelEntity->IsAnimEventTrackingEnabled());

    // get anim events from entity
    const Array<AnimEventInfo>& allAnimEvents = modelEntity->GetAnimEvents();

    // filter by category
    IndexT i;
    for (i = 0; i < allAnimEvents.Size(); i++)
    {
        if (allAnimEvents[i].GetAnimEvent().GetCategory() == category)
        {
            result.Append(allAnimEvents[i]);
        }
    }
    return result;
}

} // namespace