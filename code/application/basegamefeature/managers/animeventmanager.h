#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::AnimEventManager
  
    This application side manager, polls every frame all anim events.
    
    (C) 2009 Radon Labs GmbH
*/    
#include "game/manager.h"
#include "core/singleton.h"
#include "graphics/graphicsprotocol.h"

// forward decls
namespace Game
{
    class Entity;
}

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class AnimEventManager : public Game::Manager
{
    __DeclareClass(AnimEventManager);
    __DeclareSingleton(AnimEventManager);
public:
    /// constructor
    AnimEventManager();
    /// destructor
    virtual ~AnimEventManager();
    /// get all anim events for entity
    Util::Array<Animation::AnimEventInfo> GetAnimEventsByCategory(const Ptr<Game::Entity>& entity, const Util::StringAtom& category);
};

} // namespace BaseGameFeature
//------------------------------------------------------------------------------