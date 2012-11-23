//------------------------------------------------------------------------------
//  enventitymanager.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegameattr/basegameattributes.h"
#include "graphicsattr/graphicsattributes.h"
#include "managers/enventitymanager.h"
#include "managers/factorymanager.h"
#include "managers/entitymanager.h"
#include "io/ioserver.h"
#include "graphicsutil/segmentedgfxutil.h"
#include "graphics/stage.h"
#include "basegameprotocol.h"
#include "game/gameserver.h"
#include "graphicsfeature/graphicsfeatureunit.h"

#if __USE_PHYSICS__
#include "physics/collideshapeloader.h"
#include "physicsfeature/physicsfeatureunit.h"   
#include "physicsfeature/properties/environmentcollideproperty.h"
#endif    

namespace BaseGameFeature
{
__ImplementClass(EnvEntityManager, 'MENV', Game::Manager);
__ImplementSingleton(EnvEntityManager);

using namespace GraphicsFeature;
using namespace Util;
using namespace Math;
using namespace Game;

//------------------------------------------------------------------------------
/**
*/
EnvEntityManager::EnvEntityManager()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
EnvEntityManager::~EnvEntityManager()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
EnvEntityManager::OnDeactivate()
{
    // release environment entity, we don't need to remove it from the
    // world, the entity manager will take care of this itself
    this->envEntity = 0;
    this->envGraphicsProperty = 0;
#if __USE_PHYSICS__    
    this->envCollideProperty = 0;
#endif    

    Manager::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    This method checks if a physics file exists for the provided resource
    name.
*/
bool
EnvEntityManager::HasEntityPhysics(const Util::String& resName) const
{
    Util::String filename;
    filename.Format("physics:%s.xml", resName.AsCharPtr());
    return IO::IoServer::Instance()->FileExists(filename);
}

//------------------------------------------------------------------------------
/**
    Check if the pool env entity exists, if not create it.
*/
void
EnvEntityManager::ValidateEnvEntity()
{
    if (!this->envEntity.isvalid())
    {
        // create a pool entity for simple environment objects and attach to world            
        this->envEntity = FactoryManager::Instance()->CreateEntityByClassName("Entity");
        CategoryManager::Entry dummyEntry = CategoryManager::Instance()->CreateDummyInstance();
        this->envEntity->SetCategory("_Environment");
        this->envEntity->SetAttrTable(dummyEntry.Values());
        this->envEntity->SetAttrTableRowIndex(dummyEntry.RowIndex());
        
        // create properties
        if (GraphicsFeatureUnit::HasInstance())
        {
            this->envGraphicsProperty = EnvironmentGraphicsProperty::Create();
            this->envEntity->AttachProperty(this->envGraphicsProperty.upcast<Game::Property>());
        }        
    #if __USE_PHYSICS__
        this->envCollideProperty = PhysicsFeature::EnvironmentCollideProperty::Create();
        this->envEntity->AttachProperty(this->envCollideProperty.upcast<Game::Property>());
    #endif        
        
        EntityManager::Instance()->AttachEntity(this->envEntity);
    }
}

//------------------------------------------------------------------------------
/**
    Create an simple environment entity from scratch. The following
    attributes should be provided:
    - Attr::Graphics
    - Attr::Physics
    - Attr::_ID
    - Attr::Transform
    - Attr::AnimPath
*/
void
EnvEntityManager::CreateEnvEntity(const Util::Array<Attr::Attribute>& attrs)
{
    // create a new category instance
    CategoryManager::Entry newEntry = CategoryManager::Instance()->CreateInstanceFromAttrs("_Environment", attrs);

    // create entity
    this->CreateEnvEntity(newEntry.Values(), newEntry.RowIndex());
}

//------------------------------------------------------------------------------
/**
    Create an environment entity from its database attributes. This could
    end up in a bunch of graphics entities and collide shapes pooled in the
    central env entity, or as a traditional game object (if the environment
    object is animated or has physics). 
*/
void
EnvEntityManager::CreateEnvEntity(const Ptr<Db::ValueTable>& instTable, IndexT instTableRowIndex)
{
    n_assert(0 != instTable);
    n_assert(InvalidIndex != instTableRowIndex);

    // check if pool environment entity must be created
    this->ValidateEnvEntity();

    SegmentedGfxUtil segGfxUtil;
    Util::String resName = instTable->GetString(Attr::Graphics, instTableRowIndex);    
    
    const Util::String& id = instTable->GetString(Attr::_ID, instTableRowIndex);
    const matrix44& worldMatrix = instTable->GetMatrix44(Attr::Transform, instTableRowIndex);
    bool createdAsGameEntity = false;
    //if (instTable->GetString(Attr::AnimPath, instTableRowIndex).IsValid())
    //{
    //    // if the AnimPath attribute exists, create an animated entity
    //    this->CreateAnimatedEntity(instTable, instTableRowIndex);
    //    createdAsGameEntity = true;
    //}
#if __USE_PHYSICS__
    if (this->HasEntityPhysics(resName))
    {
        // check if this is a pure static collide object
        Physics::CollideShapeLoader collShapeLoader;
        Util::Array<Ptr<Physics::Shape> > collideShapes = collShapeLoader.Load(resName, worldMatrix);
        if (collideShapes.Size() > 0)
        {
            this->envCollideProperty->AddShapes(id, worldMatrix, collideShapes);
        }
        else
        {
            // not a pure collide object, load as physics entity
            this->CreatePhysicsEntity(instTable, instTableRowIndex);
            createdAsGameEntity = true;
        }
    }
#endif

    // create graphics entities (if not already created as a game entity)
    if (!createdAsGameEntity && this->envGraphicsProperty.isvalid())
    {
        // create graphics entity(s) and attach to graphics property 
        Util::Array<Ptr<Graphics::ModelEntity> > gfxEntities = segGfxUtil.CreateAndSetupGraphicsEntities(resName, worldMatrix, false);
        this->envGraphicsProperty->AddGraphicsEntities(id, worldMatrix, gfxEntities);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called if an environment with a path animation is
    encountered. In this case we cannot put the object into the
    big environment pool, but instead must create a unique game entity.
*/
void
EnvEntityManager::CreateAnimatedEntity(const Ptr<Db::ValueTable>& instTable, IndexT instTableRowIndex)
{
 //   n_assert((0 != instTable) && (InvalidIndex != instTableRowIndex));
 //   FactoryManager* factory = FactoryManager::Instance();

 //   // create a raw game entity
 //   Entity* gameEntity = factory->CreateEntityByClassName("Entity");

 //   // link game entity to instance attribute table
 //   gameEntity->SetCategory("_Environment");
	//gameEntity->SetAttrTable(instTable);
 //   gameEntity->SetAttrTableRowIndex(instTableRowIndex);

 //   // attach required properties
 //   Ptr<Property> graphicsProperty = factory->CreateProperty("GraphicsProperty");
 //   Ptr<Property> pathAnimProperty = factory->CreateProperty("PathAnimProperty");
 //   gameEntity->AttachProperty(graphicsProperty);
 //   gameEntity->AttachProperty(pathAnimProperty);

 //   // attach game entity to world
 //   EntityManager::Instance()->AttachEntity(gameEntity);
}

//------------------------------------------------------------------------------
/**
    This method creates a game entity with graphics and physics properties
    attached. This is a usual passive, bouncing, rolling game entity.
*/
void
EnvEntityManager::CreatePhysicsEntity(const Ptr<Db::ValueTable>& instTable, IndexT instTableRowIndex)
{
#if __USE_PHYSICS__
    FactoryManager* factory = FactoryManager::Instance();

    // create a game entity
    Ptr<Game::Entity> gameEntity = factory->CreateEntityByClassName("Entity");

    // link game entity to instance attribute table
    gameEntity->SetCategory("_Environment");
	gameEntity->SetAttrTable(instTable);
    gameEntity->SetAttrTableRowIndex(instTableRowIndex);

    // attach required properties (NOTE: the order of attachment is
    // important in this case)    
    #if __USE_PHYSICS__
    if (PhysicsFeature::PhysicsFeatureUnit::HasInstance())
    {
        Ptr<Property> physicsProperty  = factory->CreateProperty("PhysicsFeature::PhysicsProperty");
        gameEntity->AttachProperty(physicsProperty);
    }
    #endif
    
    if (GraphicsFeatureUnit::HasInstance())
    {
        Ptr<Property> graphicsProperty = factory->CreateProperty("GraphicsFeature::GraphicsProperty");    
        gameEntity->AttachProperty(graphicsProperty);
    }

    // attach game entity to world
    EntityManager::Instance()->AttachEntity(gameEntity);
#endif    
}

//------------------------------------------------------------------------------
/**
    Return true if an environment entity with the given id exists.
*/
bool
EnvEntityManager::EnvEntityExists(const Util::String& id) const
{
    n_assert(id.IsValid());

    // first check pooled entity
    if (this->envGraphicsProperty.isvalid() && this->envGraphicsProperty->HasGraphicsEntities(id))
    {
        return true;
    }
#if __USE_PHYSICS__    
    if (this->envCollideProperty.isvalid() && this->envCollideProperty->HasShapes(id))
    {
        return true;
    }
#endif    

    // not a pooled entity, check conventional entities
    bool exists = EntityManager::Instance()->ExistsEntityByAttr(Attr::Attribute(Attr::_ID, id));
    return exists;
}

//------------------------------------------------------------------------------
/**
    Set transformation of an environment entity.
*/
void
EnvEntityManager::SetEnvEntityTransform(const Util::String& id, const matrix44& worldTransform)
{
    n_assert(id.IsValid());

    // first check pooled graphics entities
    bool handled = false;
    if (this->envGraphicsProperty.isvalid() && this->envGraphicsProperty->HasGraphicsEntities(id))
    {
        const Util::Array<Ptr<Graphics::ModelEntity> >& gfxEntities = this->envGraphicsProperty->GetGraphicsEntities(id);
        const Util::Array<matrix44>& localMatrices = this->envGraphicsProperty->GetLocalMatrices(id);
        n_assert(gfxEntities.Size() == localMatrices.Size());

        // set graphics entities transform
        IndexT i;
        for (i = 0; i < gfxEntities.Size(); i++)
        {
            // localize, and move to new world position
            matrix44 m = matrix44::multiply(localMatrices[i], worldTransform);
            gfxEntities[i]->SetTransform(m);
        }
        handled = true;
    }

#if __USE_PHYSICS__
    // now check pooled physics shapes
    if (this->envCollideProperty.isvalid() && this->envCollideProperty->HasShapes(id))
    {
        const Util::Array<Ptr<Physics::Shape> >& shapes = this->envCollideProperty->GetShapes(id);
        const Util::Array<matrix44>& localMatrices = this->envCollideProperty->GetLocalMatrices(id);

        // set shape transforms
        IndexT i;
        for (i = 0; i < shapes.Size(); i++)
        {
            // localize, and move to new world position
            matrix44 m = matrix44::multiply(localMatrices[i], worldTransform);
            shapes[i]->SetTransform(m);
        }
        handled = true;
    }
#endif

    // ok, in case the environment entity exists as true game entity (because it is
    // animated or has physics) we handle the position update through a normal
    // SetTransform message
    if (!handled)
    {
        Ptr<Game::Entity> entity = EntityManager::Instance()->GetEntityByAttr(Attr::Attribute(Attr::_ID, id), true);
        Ptr<SetTransform> msg = SetTransform::Create();
        msg->SetMatrix(worldTransform);
        entity->SendSync(msg.upcast<Messaging::Message>());
    }
}

//------------------------------------------------------------------------------
/**
    Delete an environment entity.
*/
void
EnvEntityManager::DeleteEnvEntity(const Util::String& id)
{
    if (this->envGraphicsProperty.isvalid() && this->envGraphicsProperty->HasGraphicsEntities(id))
    {
        this->envGraphicsProperty->DeleteGraphicsEntities(id);
    }
#if __USE_PHYSICS__    
    if (this->envCollideProperty.isvalid() && this->envCollideProperty->HasShapes(id))
    {
        this->envCollideProperty->DeleteShapes(id);
    }
#endif    

    EntityManager* entityManager = EntityManager::Instance();
    if (entityManager->ExistsEntityByAttr(Attr::Attribute(Attr::_ID, id)))
    {
        Ptr<Game::Entity> entity = entityManager->GetEntityByAttr(Attr::Attribute(Attr::_ID, id), true);
        entityManager->RemoveEntity(entity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
EnvEntityManager::ClearEnvEntity()
{
    if (this->envEntity.isvalid())
    {
        EntityManager* entityManager = EntityManager::Instance();
        entityManager->RemoveEntity(this->envEntity);
        this->envEntity = 0;
        this->envGraphicsProperty = 0; 
    #if __USE_PHYSICS__
        this->envCollideProperty = 0;
    #endif
    }
}

//------------------------------------------------------------------------------
/**
    Returns the graphics entities of the given environment entity.
*/
Util::Array<Ptr<Graphics::ModelEntity> >
EnvEntityManager::GetGraphicsEntities(const Util::String& id) const
{
    Util::Array<Ptr<Graphics::ModelEntity> > result;
    if (this->envGraphicsProperty.isvalid() && this->envGraphicsProperty->HasGraphicsEntities(id))
    {
        result = this->envGraphicsProperty->GetGraphicsEntities(id);
    }
    return result;
}

} // namespace Managers
