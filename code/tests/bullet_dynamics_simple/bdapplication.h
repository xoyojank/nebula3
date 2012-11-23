#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::BDApplication

    (C) 2006 Radon Labs GmbH
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "audio2/fmodeventprojectid.h"
#include "audio2/fmodeventid.h"

#define HAVE_BOX_NEB_MODEL          1

// bind 2 boxes together
#define CONSTRAINT_TEST_2BOXES      0
// bind all boxes in a row together
#define CONSTRAINT_TEST_CHAINS      1

class btTransform;
class btStaticPlaneShape;
class btRigidBody;
class btDefaultCollisionConfiguration;
class btBroadphaseInterface;
class btConstraintSolver;
#if __PS3__
class btThreadSupportInterface;
#endif
class btCollisionDispatcher;
class btAxisSweep3;
//class btDbvtBroadphase;
class btDiscreteDynamicsWorld;
class btBoxShape;
class btMultiSphereShape;

namespace Audio2 { class Audio2Interface; class Audio2Server; class Audio2Player; class Audio2Listener; };

class DebugDrawer;

//------------------------------------------------------------------------------
namespace Tools
{
class BDApplication : public App::ViewerApplication
{
public:
    /// constructor
    BDApplication();
    /// destructor
    virtual ~BDApplication();
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
    void DestroyRigidBody(btRigidBody **body);
    //btRigidBody* createRigidBodyBox(const Vectormath::Aos::Vector3 &boxSize, const float boxMass);
    //btRigidBody* createRigidBodySphere(const float radius, const float boxMass);
    // reset all bodies to start position and state
    void ResetBodies();
    // generate box start transform by its index
    btTransform GetNextTransform(int index) const;
    void DrawShapeInfo(const btRigidBody *rigidBody);
    void PlayCollisionSounds();

    Ptr<Graphics::ModelEntity> groundModel;
    Util::Array<Ptr<Graphics::ModelEntity> > models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity> > pointLights;
    Util::Array<Ptr<Graphics::SpotLightEntity> > spotLights;
    Util::Array<Math::matrix44> lightTransforms;
    Util::Array<Timing::Time> frameTimes;
    Ptr<Audio2::Audio2Interface> audioInterface;
    Ptr<Audio2::Audio2Server> audioServer;
    Ptr<Audio2::Audio2Player> audioPlayer;
    Ptr<Audio2::Audio2Listener> audioListener;
    Timing::Time avgFPS;
    bool benchmarkmode;
    bool renderDebug;

    struct _physics
    {
        _physics() : collisionConfiguration(NULL), broadphase(NULL),
                     constraintSolver(NULL), 
#if __PS3__
                     colThreadSupport(NULL),
#endif
                     dispatcher(NULL), dynamicsWorld(NULL)            
        {}

        btDefaultCollisionConfiguration *collisionConfiguration;
        btBroadphaseInterface *broadphase;
        btConstraintSolver *constraintSolver;
#if __PS3__
        btThreadSupportInterface *colThreadSupport;
#endif
        btCollisionDispatcher* dispatcher;
        btDiscreteDynamicsWorld *dynamicsWorld;
    } physics;

    btStaticPlaneShape *groundShape;
    btRigidBody *groundBody;

    enum { BOX_COUNT = 200, BOX_COUNT_PER_ROW = 10 };

    float objectHalfWidth;  
    struct Object
    {
        Object() : body(NULL) {}

        void UpdateNebulaModelTransform();
     
        btRigidBody* body;
#if HAVE_BOX_NEB_MODEL
        Ptr<Graphics::ModelEntity> model;
#endif
    };

    btBoxShape* boxShapeTemplate;
    Object boxes[BOX_COUNT];   

    void ShootBox();
    Util::Array<Object*> flyingBoxes;

    DebugDrawer *debugDrawer;
    bool blnSimulationRunning;

    btMultiSphereShape *collisionSpheresCharacter;
    btRigidBody *rigidbodySpheresCharacter;

    static Audio2::FmodEventProjectId fmodProjectId;
    static Audio2::FmodEventId fmodCollideEventId[3];
};

} // namespace Test
//------------------------------------------------------------------------------
