//------------------------------------------------------------------------------
//  managers/envquerymanager.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "managers/envquerymanager.h"
#include "managers/entitymanager.h"
#include "input/inputserver.h"
#include "input/mouse.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "renderutil/mouserayutil.h"

#if __USE_PHYSICS__
#include "physics/physicsserver.h"
#endif

namespace BaseGameFeature
{
__ImplementClass(EnvQueryManager, 'EVQM', Game::Manager);
__ImplementSingleton(EnvQueryManager);

using namespace Game;
using namespace Input;
using namespace Math;
using namespace RenderUtil;

//------------------------------------------------------------------------------
/**
*/
EnvQueryManager::EnvQueryManager() :
    entityUnderMouse(0),
    mouseIntersection(false)
#if __USE_PHYSICS__
    ,materialUnderMouse(Physics::InvalidMaterial)
#endif
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
EnvQueryManager::~EnvQueryManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
EnvQueryManager::OnActivate()
{
    Manager::OnActivate();

#if __USE_PHYSICS__
    // setup exclude set for mouse queries
	Physics::MaterialType probeType = Physics::MaterialTable::StringToMaterialType("Probe");
    this->mouseExcludeSet.AddMaterialType(probeType);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
EnvQueryManager::OnDeactivate()
{    
#if __USE_PHYSICS__
    this->mouseExcludeSet.Clear();
#endif

    Manager::OnDeactivate();
}


#if __USE_PHYSICS__
//------------------------------------------------------------------------------
/**
	get a const Ref on the MouseExcludeSet
*/
const Physics::FilterSet&
EnvQueryManager::GetMouseExcludeSet() const
{
	return this->mouseExcludeSet;
}

//------------------------------------------------------------------------------
/**
	set a new MouseExcludeSet
*/
void
EnvQueryManager::SetMouseExcludeSet( const Physics::FilterSet& mouseExcludeSet )
{
    this->mouseExcludeSet = mouseExcludeSet;
}
#endif

//------------------------------------------------------------------------------
/**
    This returns a pointer to the entity under the mouse, or 0 if mouse is 
    not over an entity.
*/
Entity*
EnvQueryManager::GetEntityUnderMouse() const
{
    return EntityManager::Instance()->GetEntityByUniqueId(this->entityUnderMouse).get_unsafe();
}

#if __USE_PHYSICS__
//------------------------------------------------------------------------------
/**
    This returns the MaterialType of the entity under the mouse, or 
    InvalidMaterial if mouse is not over an entity.
*/
Physics::MaterialType
EnvQueryManager::GetMaterialUnderMouse() const
{
    return this->materialUnderMouse;
}
#endif

//------------------------------------------------------------------------------
/**
    This returns the position where a vector through the mouse position 
    intersects the 3d world (or the nearest entity). If the mouse doesn't
    intersect, the result will be undefined, and the method
    HasMouseIntersection() returns false.
*/
const point&
EnvQueryManager::GetMousePos3d() const
{
    return this->mousePos3d;
}

//------------------------------------------------------------------------------
/**
    This returns the upvector of the face under the mousecursor.
    If the mouse doesn't intersect, the result will be undefined, 
    and the method HasMouseIntersection() returns false.
*/
const vector&
EnvQueryManager::GetUpVector() const
{
    return this->upVector;
}

//------------------------------------------------------------------------------
/**
    Returns true if the vector through the current mouse position intersects
    the world, or an entity, false if no intersection exists.
*/
bool
EnvQueryManager::HasMouseIntersection() const
{
    return this->mouseIntersection;
}

//------------------------------------------------------------------------------
/**
    Returns all game entities which intersect the given sphere. Uses the
    physics subsystem to do the query.
*/
Util::Array<Ptr<Game::Entity> >
EnvQueryManager::GetEntitiesInSphere(const point& midPoint, float radius)
{
    Util::Array<Ptr<Game::Entity> > gameEntities;

#if __USE_PHYSICS__
    Physics::FilterSet excludeSet;
    Util::Array<Ptr<Physics::PhysicsEntity> > physicsEntities;
    Physics::PhysicsServer::Instance()->GetEntitiesInSphere(midPoint, radius, excludeSet, physicsEntities);
    
    // convert physics entities back into game entities
    EntityManager* entityManager = EntityManager::Instance();
    int i;
    int num = physicsEntities.Size();
    for (i = 0; i < num; i++)
    {
        if (entityManager->ExistsEntityByUniqueId(physicsEntities[i]->GetUserData()))
        {
            gameEntities.Append(entityManager->GetEntityByUniqueId(physicsEntities[i]->GetUserData()));
        }
    }
#else
    n_printf("EnvQueryManager::GetEntitiesInSphere: Physics not supported yet!!!");
#endif

    return gameEntities;
}
//------------------------------------------------------------------------------
/**
    Returns all game entities which intersect the given box. Uses the
    physics subsystem to do the query.
*/
Util::Array<Ptr<Game::Entity> >
EnvQueryManager::GetEntitiesInBox(const vector& scale, const matrix44& m)
{
    Util::Array<Ptr<Game::Entity> > gameEntities;

#if __USE_PHYSICS__
    Physics::FilterSet excludeSet;
    Util::Array<Ptr<Physics::PhysicsEntity> > physicsEntities;
    Physics::PhysicsServer::Instance()->GetEntitiesInBox(scale, m, excludeSet, physicsEntities);
    
    // convert physics entities back into game entities
    EntityManager* entityManager = EntityManager::Instance();
    int i;
    int num = physicsEntities.Size();
    for (i = 0; i < num; i++)
    {
        if (entityManager->ExistsEntityByUniqueId(physicsEntities[i]->GetUserData()))
        {
            gameEntities.Append(entityManager->GetEntityByUniqueId(physicsEntities[i]->GetUserData()));
        }
    }
#else
    n_printf("EnvQueryManager::GetEntitiesInBox: Physics not supported yet!!!");
#endif

    return gameEntities;
}
//------------------------------------------------------------------------------
/**
    This method is called per-frame by the game server and updates the
    current values, like entity-under-mouse, 3d mouse position, etc...
*/
void
EnvQueryManager::OnFrame()
{   
    // reset values
    this->entityUnderMouse = 0;
    this->mousePos3d.set(0.0f, 0.0f, 0.0f);
    this->mouseIntersection = false;        

    // get 3d contact under mouse
    
    /*Physics::PhysicsServer* physicsServer = Physics::PhysicsServer::Instance();

    const vector2& mousePos = InputServer::Instance()->GetDefaultMouse()->GetScreenPosition();        
    const float rayLength = 5000.0f;            
    line worldMouseRay = nGfxServer2::Instance()->ComputeWorldMouseRay(mousePos, rayLength);
    this->materialUnderMouse = Physics::InvalidMaterial;*/
    
  //      // send a BeginPicking message to all interested entities
  //      Ptr<Messaging::BeginPicking> beginPickingMsg = Messaging::BeginPicking::Create();
  //      beginPickingMsg->SetWorldSpaceRay(worldMouseRay);
  //      IndexT i;
  //      for (i = 0; i < this->pickingEntities.Size(); i++)
  //      {
  //          this->pickingEntities[i]->SendSync(beginPickingMsg);
  //      }

#if __USE_PHYSICS__
    //if (InputServer::HasInstance())
    //{
    //    float2 mousePos = InputServer::Instance()->GetDefaultMouse()->GetScreenPosition();
    //    float length = 1000;
    //    line worldRay = this->ComputeMouseWorldRay(mousePos, length, GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView());

    //    // do the actual picking
    //    const Physics::ContactPoint* contact = 0;
    //    contact = Physics::PhysicsServer::Instance()->GetClosestContactUnderMouse(worldRay, this->mouseExcludeSet);
    //    if (0 != contact)
    //    {
    //        // store intersection position
    //        this->mousePos3d = contact->GetPosition();
    //        this->upVector = contact->GetUpVector();
    //        this->mouseIntersection = true;

    //        // new entity under mouse?
    //        Physics::PhysicsEntity* physicsEntity = Physics::PhysicsServer::Instance()->GetEntityByUniqueId(contact->GetPhysicsEntityId());
    //        Game::Entity::EntityId gameEntityUnderMouse = 0;
    //        if (physicsEntity)
    //        {
    //            // user data of physics entity is unique id of game entity which owns the
    //            // physics entity
    //            gameEntityUnderMouse = physicsEntity->GetUserData();
    //            this->materialUnderMouse = contact->GetMaterial();
    //        }
    //        if (gameEntityUnderMouse != this->entityUnderMouse)
    //        {
    //            this->entityUnderMouse = gameEntityUnderMouse;
    //        }
    //    }    

    //    //// send a EndPicking message to all interested entities
    //    //Ptr<Messaging::EndPicking> endPickingMsg = Messaging::EndPicking::Create();
    //    //for (i = 0; i < this->pickingEntities.Size(); i++)
    //    //{
    //    //    this->pickingEntities[i]->SendSync(endPickingMsg);
    //    //}  
    //}
#endif
}

//------------------------------------------------------------------------------
/**
*/
Math::line 
EnvQueryManager::ComputeMouseWorldRay(const float2& mousePos, float length, const Ptr<Graphics::View>& view) const
{    
    const Ptr<Graphics::CameraEntity>& cam = view->GetCameraEntity();
    n_assert(cam.isvalid());
    const matrix44& invView = matrix44::inverse(cam->GetViewTransform());
    matrix44 invProj = matrix44::inverse(cam->GetProjTransform());    
    return MouseRayUtil::ComputeWorldMouseRay(mousePos, length, invView, invProj, cam->GetCameraSettings().GetZNear());    
}

} // namespace Game
