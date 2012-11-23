#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::BCApplication

    (C) 2006 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "btBulletCollisionCommon.h"
#include "BulletMultiThreaded/btThreadSupportInterface.h"

// nebula meetz bullet
Math::vector                Bt2NebVector(const btVector3 &btV);
Math::point                 Bt2NebPoint(const btVector3 &btV);
Math::matrix44              Bt2NebMatrix3x3(const btMatrix3x3 &m);
Math::matrix44              Bt2NebTransform(const btTransform &t);

btMatrix3x3                 Neb2BtM44M33(const Math::matrix44 &m);
btTransform                 Neb2BtM44Transform(const Math::matrix44 &m);
btVector3                   Neb2BtPoint(const Math::point &p);

struct PhysicsObject
{
    // aller setter setzen die bullet member, nebula wird davon upgedatet
    void                    SetTransform(const btTransform &t);
    void                    SetTransform(const Math::matrix44 &t);
    void                    SetPosition(const Math::point &pos);

    Math::matrix44          GetTransform() const;
    const btTransform&      GetBtTransform() const;

private:
    // sets model transform from bullet data
    void                    UpdateNebulaStateFromBullet();

public:
    struct _bulletGeo
    {
        _bulletGeo() : shape(NULL), collisionObject(NULL) {}

        btConvexInternalShape   *shape;
        btCollisionObject       *collisionObject;
    } bulletGeo;

    Math::vector halfBoxExtents;
/*
    struct
    {
        Ptr<Graphics::ModelEntity> model;
    } nebulaGfxModel;
*/
};

//------------------------------------------------------------------------------
namespace Tools
{
class BCApplication : public App::ViewerApplication
{
public:
    /// constructor
    BCApplication();
    /// destructor
    virtual ~BCApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnUpdateFrame();
    /// called to configure display
    virtual void OnConfigureDisplay();
    /// init bullet stuff
    void InitPhysicsWorld();
    /// destroy bullet stuff
    void DestroyPhysicsWorld();
    /// init sample objects
    void InitPhysicsObjects();
    /// destroy sample objects
    void DestroyPhysicsObjects();

    Ptr<Graphics::ModelEntity> ground;
    Util::Array<Ptr<Graphics::ModelEntity> > models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity> > pointLights;
    Ptr<Graphics::SpotLightEntity> testSpotLight;
    Util::Array<Ptr<Graphics::SpotLightEntity> > spotLights;
    Util::Array<Math::matrix44> lightTransforms;
    Util::Array<Timing::Time> frameTimes;
    Timing::Time avgFPS;
    bool benchmarkmode;
    bool renderDebug;
    float rotX;

    struct _physics
    {
        _physics() : collisionConfiguration(NULL), dispatcher(NULL), broadphase(NULL), collisionWorld(NULL) {}

        btDefaultCollisionConfiguration *collisionConfiguration;
#if __PS3__
        btThreadSupportInterface *colThreadSupport;
#endif
        btCollisionDispatcher* dispatcher;
        btAxisSweep3* broadphase;
        btCollisionWorld *collisionWorld;
    } physics;

    // physics objects
    enum            { NUM_PHYSICS_OBJECTS = 4 };
    PhysicsObject   objects[NUM_PHYSICS_OBJECTS];
    int physicsInputMode;

};

} // namespace Test
//------------------------------------------------------------------------------
