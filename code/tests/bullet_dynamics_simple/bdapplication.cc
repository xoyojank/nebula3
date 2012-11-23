//------------------------------------------------------------------------------
//  bdapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "bdapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "debug/debugfloat.h"
#include "debugdrawer.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletMultiThreaded/btThreadSupportInterface.h"
#include "audio2/audio2interface.h"
#include "audio2/audio2player.h"
#include "audio2/audio2listener.h"
#include "audio2/audio2server.h"
#include "NebulaMotionState.h"

#if __PS3__
#include <BulletMultiThreaded/btThreadSupportInterface.h>
#include <SpuDispatch/BulletCollisionSpursSupport.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#endif

namespace Tools
{
using namespace CoreGraphics;
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;
using namespace Debug;
using namespace Input;

Audio2::FmodEventProjectId  BDApplication::fmodProjectId("bullet_dynamics");
Audio2::FmodEventId         BDApplication::fmodCollideEventId[3] =
{
    Audio2::FmodEventId(fmodProjectId, "effects/amb_hammer_wood_01"),
    Audio2::FmodEventId(fmodProjectId, "effects/amb_hammer_wood_02"),
    Audio2::FmodEventId(fmodProjectId, "effects/amb_hammer_wood_03"),
};

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::Object::UpdateNebulaModelTransform()
{
#if HAVE_BOX_NEB_MODEL
    if(!this->model.isvalid()) return;
/*    
    btTransform transform;
    this->body->getMotionState()->getWorldTransform(transform);
    this->model->SetTransform(Bt2NebTransform(transform));
*/
    NebulaMotionState *state = static_cast<NebulaMotionState*>(this->body->getMotionState());
    this->model->SetTransform(state->GetTransform());

    //Math::matrix44 trans = Bt2NebTransform(this->body->getWorldTransform());
    //this->model->SetTransform(trans);
#endif
}

//------------------------------------------------------------------------------
/**
*/
BDApplication::BDApplication() :
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false),
    groundShape(NULL),
    groundBody(NULL),
    objectHalfWidth(1.0f),
    debugDrawer(NULL),
    blnSimulationRunning(false),
    collisionSpheresCharacter(NULL)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BDApplication::~BDApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
BDApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(0.4, 0.4, 0.4f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 0.25f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.1f, 0.1f, 0.1f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        // create a few point lights
        IndexT lightIndex;
        const SizeT numLights = 0;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {
            float x = n_rand() * 10.0f - 20.0f;
            float y = 3;
            float z = n_rand() * 10.0f - 20.0f;
            float r = n_rand() + 0.2f;
            float g = n_rand() + 0.2f;
            float b = n_rand() + 0.2f;
            matrix44 pointLightTransform = matrix44::multiply(matrix44::scaling(7.0f, 7.0f, 7.0f),
                                                              matrix44::translation(x, y, z));

            Ptr<PointLightEntity> pointLight = PointLightEntity::Create();
            pointLight->SetTransform(pointLightTransform);
            pointLight->SetColor(float4(r, g, b, 1.0f));
            pointLight->SetCastShadows(false);
            this->pointLights.Append(pointLight);
            this->lightTransforms.Append(pointLightTransform);
            this->stage->AttachEntity(pointLight.cast<GraphicsEntity>());
        }

        const SizeT numSpotLights = 0;
        for (lightIndex = 0; lightIndex < numSpotLights; lightIndex++)
        {
            float x = n_rand() * 25.0f - 50.0f;
            float y = 10;
            float z = n_rand() * 25.0f - 50.0f;
            float r = n_rand() + 0.1f;
            float g = n_rand() + 0.1f;
            float b = n_rand() + 0.1f;
            matrix44 spotLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
            spotLightTransform.scale(point(7,7,7));
            spotLightTransform.set_position(point(x,y,z));
            Ptr<SpotLightEntity> spotLight = SpotLightEntity::Create();
            spotLight->SetTransform(spotLightTransform);
            spotLight->SetColor(float4(r, g, b, 1.0f));
            spotLight->SetCastShadows(false);
            this->spotLights.Append(spotLight);
            this->stage->AttachEntity(spotLight.cast<GraphicsEntity>());
        }

        // setup models
        this->groundModel = ModelEntity::Create();
        this->groundModel->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
        this->groundModel->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(this->groundModel.cast<GraphicsEntity>());

        IndexT j;
        IndexT i;
        bool createStatic = false;
        for (j = 0; j < 1; ++j)
        {
            for (i = 0; i < 1; ++i)
            {
                Ptr<ModelEntity> model = ModelEntity::Create();
                model->SetTransform(matrix44::translation(2.0f * i, 0.0, 0.0f));
                //model->SetTransform(matrix44::translation(2.0f * i, 2.0, 2.0f * j));
                if (createStatic)
                {
                    model->SetResourceId(ResourceId("mdl:examples/tiger.n3"));
                    this->stage->AttachEntity(model.cast<GraphicsEntity>());
                }
                else
                {
                    model->SetResourceId(ResourceId("mdl:characters/mensch_m.n3"));
                    this->stage->AttachEntity(model.cast<GraphicsEntity>());

                    // apply skin
                    Ptr<Graphics::ApplySkinList> skinList = Graphics::ApplySkinList::Create();
                    skinList->SetSkinList(StringAtom("mann_nackt"));
                    model->SendMsg(skinList.cast<GraphicsEntityMessage>());

                    // play animation
                    Ptr<Graphics::AnimPlayClip> playLoop = Graphics::AnimPlayClip::Create();
                    //playLoop->SetBlendPriority(0);
                    playLoop->SetBlendWeight(1.0f);
                    playLoop->SetTimeFactor(1.0f);
                    playLoop->SetStartTime(0);
                    playLoop->SetTimeOffset(0);
                    playLoop->SetFadeInTime(200);
                    playLoop->SetFadeOutTime(200);
                    playLoop->SetClipName(StringAtom("gehen_01"));
                    model->SendMsg(playLoop.cast<GraphicsEntityMessage>());
                }
                // createStatic  = !createStatic;
                this->models.Append(model);
            }
        }

        this->InitPhysicsWorld();    
        this->InitPhysicsObjects();

        // wait for resources to be loaded
        GraphicsInterface::Instance()->WaitForPendingResources();

        this->audioInterface = Audio2::Audio2Interface::Create();
        this->audioInterface->Open();

        this->audioServer = Audio2::Audio2Server::Create();
        this->audioServer->Open();
        this->audioServer->LoadEventProject(fmodProjectId);

        // audio server creates it for us
        this->audioListener = Audio2::Audio2Listener::Instance();

        this->audioPlayer = Audio2::Audio2Player::Create();                

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::InitPhysicsWorld()
{
    this->physics.collisionConfiguration = n_new(btDefaultCollisionConfiguration());

    const btVector3 worldAabbMin(-200,-10,-200);
    const btVector3 worldAabbMax(200,200,200);

    this->physics.broadphase = n_new(btAxisSweep3(worldAabbMin, worldAabbMax));
    //this->physics.broadphase = n_new(btDbvtBroadphase());
    this->physics.constraintSolver = n_new(btSequentialImpulseConstraintSolver());
#if __PS3__
    CellSpurs* spursInst = Jobs::JobSystem::Instance()->PS3GetSpursInstance();
    int numSpuTasks = 6;
    this->physics.colThreadSupport = n_new(BulletCollisionSpursSupport(spursInst, numSpuTasks, numSpuTasks));
    this->physics.dispatcher = n_new(SpuGatheringCollisionDispatcher(this->physics.colThreadSupport, numSpuTasks, this->physics.collisionConfiguration));
#else
    this->physics.dispatcher = n_new(btCollisionDispatcher(this->physics.collisionConfiguration));
#endif
    this->physics.dynamicsWorld = n_new(btDiscreteDynamicsWorld(this->physics.dispatcher, this->physics.broadphase, this->physics.constraintSolver, this->physics.collisionConfiguration));

    this->debugDrawer = n_new(DebugDrawer);
    this->debugDrawer->setDebugMode(
        btIDebugDraw::DBG_DrawFeaturesText +
        btIDebugDraw::DBG_DrawText +
        btIDebugDraw::DBG_DrawWireframe +
        btIDebugDraw::DBG_DrawContactPoints/* +
        btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits*/);
    this->physics.dynamicsWorld->setDebugDrawer(this->debugDrawer);
}

//------------------------------------------------------------------------------
/**
*/
btTransform 
BDApplication::GetNextTransform(int index) const
{
    btTransform ret;
    btMatrix3x3 m3x3;
    m3x3.setEulerYPR((btScalar)(index / 5), (btScalar)(index % 5), 0.0f);
    ret.setBasis(m3x3);
    const btVector3 pos(-10.0f + ((index % BOX_COUNT_PER_ROW) * 2.5f), 
                        5.0f + ((index / BOX_COUNT_PER_ROW) * 2.5f),
                        0.0f);
    ret.setOrigin(pos);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::ResetBodies()
{
    int index = 0;

    for(int i = 0; i < BOX_COUNT; ++i)
    {
        btRigidBody *box = this->boxes[i].body;
        const btTransform trans = GetNextTransform(index++);
        box->clearForces();
        box->setCenterOfMassTransform(trans);
        box->setLinearVelocity(btVector3(0,0.0f,0));
        box->setAngularVelocity(btVector3(0,0,0));
        box->activate(true);
        this->boxes[i].UpdateNebulaModelTransform();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::ShootBox()
{
    Object *box = n_new(Object);
    this->flyingBoxes.Append(box);

    float boxMass = 200.0f;
	btBoxShape *boxShape = n_new(btBoxShape(btVector3(this->objectHalfWidth, this->objectHalfWidth, this->objectHalfWidth)));
    btVector3 invInertia(0,0,0);
    boxShape->calculateLocalInertia(boxMass, invInertia);

    NebulaMotionState* motionState = n_new(NebulaMotionState());
    btRigidBody::btRigidBodyConstructionInfo boxInfo(boxMass, motionState, boxShape, invInertia);
    box->body = n_new(btRigidBody(boxInfo));
    this->physics.dynamicsWorld->addRigidBody(box->body);

#if HAVE_BOX_NEB_MODEL
    box->model = ModelEntity::Create();
    box->model->SetResourceId(ResourceId("mdl:examples/boxunit.n3"));
    this->stage->AttachEntity(box->model.cast<GraphicsEntity>());
#endif

    btTransform trans;
    const Math::point &cam_pos = this->camera->GetTransform().get_position();
    Math::vector cam_dir = -this->camera->GetTransform().getrow2();
    const Math::vector &cam_right = this->camera->GetTransform().getrow0();
    Math::point start_pos = cam_pos + (cam_dir * 2.0f);
    btMatrix3x3 m33 = Neb2BtM44M33(this->camera->GetTransform());
    trans.setOrigin(btVector3(start_pos.x(), start_pos.y(), start_pos.z()));
    trans.setBasis(m33);
    box->body->clearForces();
    box->body->setCenterOfMassTransform(trans);
    box->body->setAngularVelocity(btVector3(0,0,0));
    Math::matrix44 rot = matrix44::rotationaxis(cam_right, 0.1f);
    vector velocity = matrix44::transform(cam_dir, rot);
    velocity *= 50.0f;
    box->body->setLinearVelocity(btVector3(velocity.x(), velocity.y(), velocity.z()));
    box->body->activate(true);
    box->UpdateNebulaModelTransform();
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::InitPhysicsObjects()
{
    // ground
    {
	    this->groundShape = n_new(btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f));
        const btVector3 localInertia(0,0,0);
        NebulaMotionState* motionState = n_new(NebulaMotionState());
	    const btRigidBody::btRigidBodyConstructionInfo cInfo(0.0f, motionState, this->groundShape, localInertia);
	    btTransform groundTransform;
        groundTransform.setIdentity();
	    this->groundBody = n_new(btRigidBody(cInfo));
        this->groundBody->setWorldTransform(groundTransform);
        this->physics.dynamicsWorld->addRigidBody(this->groundBody);
    }

    // collision for character
    {
        float height = 2.0f;
        float width = 0.333f;
	    btVector3 spherePositions[3];
	    btScalar sphereRadii[3];    	
	    sphereRadii[0] = width;
	    sphereRadii[1] = width;
	    sphereRadii[2] = width;
	    spherePositions[0] = btVector3 (0.0, width, 0.0);
	    spherePositions[1] = btVector3 (0.0, width * 3, 0.0);
	    spherePositions[2] = btVector3 (0.0, width * 5, 0.0);

        this->collisionSpheresCharacter = n_new(btMultiSphereShape(spherePositions, sphereRadii, 3));
	    btTransform startTransform;
	    startTransform.setIdentity ();
	    startTransform.setOrigin (btVector3(0.0, 0.0, 0.0));
	    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	    btRigidBody::btRigidBodyConstructionInfo cInfo(0.0, myMotionState, this->collisionSpheresCharacter);
	    this->rigidbodySpheresCharacter = n_new(btRigidBody(cInfo));
	    // kinematic vs. static doesn't work
	    //m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	    this->rigidbodySpheresCharacter->setSleepingThresholds (0.0, 0.0);
	    this->rigidbodySpheresCharacter->setAngularFactor (0.0);
        this->physics.dynamicsWorld->addRigidBody(this->rigidbodySpheresCharacter);
    }

	{
    	float boxMass = 5.0f;
    	this->boxShapeTemplate = n_new(btBoxShape(btVector3(this->objectHalfWidth, this->objectHalfWidth, this->objectHalfWidth)));
        btVector3 invInertia(0,0,0);
        this->boxShapeTemplate->calculateLocalInertia(boxMass, invInertia);
        for(int i = 0; i < BOX_COUNT; ++i)
        {
            NebulaMotionState* motionState = n_new(NebulaMotionState());
	        btRigidBody::btRigidBodyConstructionInfo boxInfo(boxMass, motionState, this->boxShapeTemplate, invInertia);
            this->boxes[i].body = n_new(btRigidBody(boxInfo));
            this->physics.dynamicsWorld->addRigidBody(this->boxes[i].body);

#if HAVE_BOX_NEB_MODEL
            this->boxes[i].model = ModelEntity::Create();
            this->boxes[i].model->SetResourceId(ResourceId("mdl:examples/boxunit.n3"));
            this->stage->AttachEntity(this->boxes[i].model.cast<GraphicsEntity>());
#endif

#if CONSTRAINT_TEST_2BOXES
            if(i & 1)
            {
                btRigidBody *bodyA, *bodyB;
                bodyA = this->boxes[i-1].body;
                bodyB = this->boxes[i].body;
        		btVector3 pivotInA(this->objectHalfWidth + 0.4, 0.0f, 0.0f);
                btVector3 pivotInB(-this->objectHalfWidth - 0.4, 0.0f, 0.0f);
                btPoint2PointConstraint* p2p = n_new(btPoint2PointConstraint(*bodyA, *bodyB, pivotInA, pivotInB));
/*
                p2p->m_setting.m_tau = 0.3f;
                p2p->m_setting.m_damping = 1.0f;
                p2p->m_setting.m_impulseClamp = 0.0f;
*/
                this->physics.dynamicsWorld->addConstraint(p2p);
            }
#endif

#if CONSTRAINT_TEST_CHAINS
            if(i % BOX_COUNT_PER_ROW)
            {
                btRigidBody *bodyA, *bodyB;
                bodyA = this->boxes[i-1].body;
                bodyB = this->boxes[i].body;

        		//btVector3 pivotInA(this->objectHalfWidth + 0.4, 0.0f, 0.0f);
                //btVector3 pivotInB(-this->objectHalfWidth - 0.4, 0.0f, 0.0f);

        		btVector3 pivotInA(this->objectHalfWidth + 0.1f, 0.0f, 0.0f);
                btVector3 pivotInB(-this->objectHalfWidth - 0.1f, 0.0f, 0.0f);

        		//////btVector3 pivotInA(this->objectHalfWidth + 1.0, 0.0f, 0.0f);
                //////btVector3 pivotInB(this->objectHalfWidth + 1.0, 0.0f, 0.0f);

                btPoint2PointConstraint* p2p = n_new(btPoint2PointConstraint(*bodyA, *bodyB, pivotInA, pivotInB));
/*
                p2p->m_setting.m_tau = 0.3f;
                p2p->m_setting.m_damping = 1.0f;
                p2p->m_setting.m_impulseClamp = 0.0f;
*/
                this->physics.dynamicsWorld->addConstraint(p2p);
            }
#endif
        }
	}
	
    this->ResetBodies();
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::DestroyRigidBody(btRigidBody **body)
{
    if(!body || (!(*body))) return;

    btMotionState *motionState = (*body)->getMotionState();
    n_delete(motionState);
    motionState = NULL;
    (*body)->setMotionState(NULL);

    this->physics.dynamicsWorld->removeRigidBody(*body);
    n_delete(*body);
    *body = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::DestroyPhysicsObjects()
{
    if(!this->physics.dynamicsWorld) return;

    // ground
    if(this->groundBody)
    {
        this->DestroyRigidBody(&this->groundBody);
    }
    if(this->groundShape)
    {
        n_delete(this->groundShape);
        this->groundShape = NULL;
    }
    
    // destroy collision for character
    if(this->rigidbodySpheresCharacter)
    {
        this->DestroyRigidBody(&this->rigidbodySpheresCharacter);
    }
    if(this->collisionSpheresCharacter)
    {
        n_delete(this->collisionSpheresCharacter);
        this->collisionSpheresCharacter = NULL;
    }

    for(int i = 0; i < BOX_COUNT; ++i)
    {
        this->DestroyRigidBody(&this->boxes[i].body);

#if HAVE_BOX_NEB_MODEL
        this->stage->RemoveEntity(this->boxes[i].model.cast<GraphicsEntity>());
        this->boxes[i].model = NULL;
#endif
    }

    if(this->boxShapeTemplate)
    {
        n_delete(this->boxShapeTemplate);
        this->boxShapeTemplate = NULL;
    }

    for(int i = 0; i < this->flyingBoxes.Size(); ++i)
    {
        this->DestroyRigidBody(&this->flyingBoxes[i]->body);

#if HAVE_BOX_NEB_MODEL
        this->stage->RemoveEntity(this->flyingBoxes[i]->model.cast<GraphicsEntity>());
        this->flyingBoxes[i]->model = NULL;
#endif

        n_delete(this->flyingBoxes[i]);
        this->flyingBoxes[i] = NULL;
    }
    this->flyingBoxes.Reset();
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::DestroyPhysicsWorld()
{
    if(!this->physics.dynamicsWorld) return;

    n_delete(this->physics.dynamicsWorld);
    this->physics.dynamicsWorld = NULL;

    n_delete(this->physics.collisionConfiguration);
    this->physics.collisionConfiguration = NULL;

    n_delete(this->physics.broadphase);
    this->physics.broadphase = NULL;

    n_delete(this->physics.constraintSolver);
    this->physics.constraintSolver = NULL;

#if __PS3__
    n_delete(this->physics.colThreadSupport);
    this->physics.colThreadSupport = NULL;
#endif

    n_delete(this->physics.dispatcher);
    this->physics.dispatcher = NULL;

    n_delete(this->debugDrawer);
    this->debugDrawer = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::Close()
{
    this->audioServer->UnloadEventProject(fmodProjectId);
    this->audioServer->Close();
    this->audioServer = 0;

    this->audioInterface->Close();
    this->audioInterface = 0;

    this->audioPlayer = 0;
    this->audioListener = 0;

    this->DestroyPhysicsObjects();
    this->DestroyPhysicsWorld();

    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    this->stage->RemoveEntity(this->groundModel.cast<GraphicsEntity>());
    this->globalLight = 0;
    this->groundModel = 0;

    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->pointLights[i].cast<GraphicsEntity>());
    }
    this->pointLights.Clear();
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->spotLights[i].cast<GraphicsEntity>());
    }
    this->spotLights.Clear();

    for (i = 0; i < this->models.Size(); i++)
    {
        this->stage->RemoveEntity(this->models[i].cast<GraphicsEntity>());
    }
    this->models.Clear();

    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();

    // enable/disable debug view
    if (kbd->KeyDown(Key::F4))
    {
        // turn on debug rendering
        Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
        renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
        renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
        Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
        this->renderDebug = !this->renderDebug;
    }
    else if (kbd->KeyDown(Key::F3))
    {
        this->benchmarkmode = !this->benchmarkmode;
        // reset to start pos
        this->mayaCameraUtil.Reset();
    }
	else if (kbd->KeyDown(Key::A))
	{
		this->ShootBox();
	}
	else if ( kbd->KeyDown(Key::B))
	{
		this->blnSimulationRunning = !this->blnSimulationRunning;
	}
	else if ( kbd->KeyDown(Key::Space))
	{
		this->ResetBodies();
	}
	else if ( kbd->KeyDown(Key::D))
	{
		this->DestroyPhysicsObjects();
		this->DestroyPhysicsWorld();
	}


    const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);
    if (gamePad->IsConnected())
    {
        if (gamePad->ButtonDown(GamePad::R1Button))
        {
            this->ResetBodies();
        }
        if (gamePad->ButtonDown(GamePad::L1Button))
        {
            this->blnSimulationRunning = !this->blnSimulationRunning;
        }

        if (gamePad->ButtonDown(GamePad::CircleButton))
        {
            this->ShootBox();
        }

        if (gamePad->ButtonDown(GamePad::SelectButton))
        {
            this->DestroyPhysicsObjects();
            this->DestroyPhysicsWorld();
        }
    }

    if (this->benchmarkmode)
    {
        // auto rotate
        this->mayaCameraUtil.SetOrbiting(Math::float2(0.02,0));
        this->mayaCameraUtil.SetOrbitButton(true);
        this->mayaCameraUtil.Update();
        this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
    }
    else
    {
        ViewerApplication::OnProcessInput();
    }    
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::DrawShapeInfo(const btRigidBody *rigidBody)
{
    const btCollisionShape *shape = rigidBody->getCollisionShape();
    const btTransform &transform = rigidBody->getWorldTransform();

	/// for polyhedral shapes
	if (/*debugMode==btIDebugDraw::DBG_DrawFeaturesText && */(shape->isPolyhedral()))
	{
		btPolyhedralConvexShape* polyshape = (btPolyhedralConvexShape*) shape;

		{
			int i;
			for (i=0;i<polyshape->getNumVertices();i++)
			{
				btVector3 vtx;
				polyshape->getVertex(i,vtx);
                vtx = transform * vtx;
				char buf[12];
				sprintf(buf," %d",i);
                this->debugDrawer->draw3dText(vtx, buf);
			}

			for (i=0;i<polyshape->getNumPlanes();i++)
			{
				btVector3 normal;
				btVector3 vtx;
				polyshape->getPlane(normal,vtx,i);
				btScalar d = vtx.dot(normal);

                btVector3 tmp = vtx + btVector3(normal.x()*d, normal.y()*d, normal.z()*d);
                vtx = transform * vtx;
                tmp = transform * tmp;

                this->debugDrawer->drawLine(vtx, tmp, btVector3(0.4f, 0.4f, 0.4f));

                char buf[12];
				sprintf(buf," plane %d",i);
                this->debugDrawer->draw3dText(tmp, buf);
			}
		}
	}
}

#if 1
#  define printM(...)
#else
#  define printM(...) n_printf(__VA_ARGS__)
#endif

void DebugPrint(const char *tab, const char *msg, const btVector3 &v)
{
    printM("%s%-25s: %f %f %f\n", tab, msg, v.x(), v.y(), v.z());    
}

void DebugPrint(const char *tab, const char *msg, const btScalar &s)
{
    printM("%s%-25s: %f\n", tab, msg, s);    
}

void DebugPrint(const char *tab, const char *msg, const int &i)
{
    printM("%s%-25s: %d\n", tab, msg, i);    
}

void DebugPrint(const char *tab, const char *msg, const void *p)
{
    printM("%s%-25s: 0x%p\n", tab, msg, p);    
}

void DebugPrint(const char *tab, const char *msg, bool b)
{
    printM("%s%-25s: %s\n", tab, msg, b ? "yes" : "no");    
}

void PrintManifold(const btManifoldPoint &m)
{
    const char *tab = "          ";
    DebugPrint(tab, "btManifoldPoint", (void*)&m);
    //DebugPrint(tab, "m_localPointA", m.m_localPointA);
    //DebugPrint(tab, "m_localPointB", m.m_localPointB);
    DebugPrint(tab, "m_positionWorldOnB", m.m_positionWorldOnB);
    DebugPrint(tab, "m_positionWorldOnA", m.m_positionWorldOnA);
    //DebugPrint(tab, "m_normalWorldOnB", m.m_normalWorldOnB);
    //DebugPrint(tab, "m_distance1", m.m_distance1);
    //DebugPrint(tab, "m_combinedFriction", m.m_combinedFriction);
    //DebugPrint(tab, "m_combinedRestitution", m.m_combinedRestitution);		
    //DebugPrint(tab, "m_partId0", m.m_partId0);
    //DebugPrint(tab, "m_partId1", m.m_partId1);
    //DebugPrint(tab, "m_index0", m.m_index0);
    //DebugPrint(tab, "m_index1", m.m_index1);				
    //DebugPrint(tab, "m_userPersistentData", m.m_userPersistentData);
    //DebugPrint(tab, "m_appliedImpulse", m.m_appliedImpulse);
    //DebugPrint(tab, "m_lateralFrictionInitialized", m.m_lateralFrictionInitialized);
    //DebugPrint(tab, "m_appliedImpulseLateral1", m.m_appliedImpulseLateral1);
    //DebugPrint(tab, "m_appliedImpulseLateral2", m.m_appliedImpulseLateral2);
    DebugPrint(tab, "m_lifeTime", m.m_lifeTime);   
    //DebugPrint(tab, "m_lateralFrictionDir1", m.m_lateralFrictionDir1);
    //DebugPrint(tab, "m_lateralFrictionDir2", m.m_lateralFrictionDir2);
}

//------------------------------------------------------------------------------
/**
*/
void 
BDApplication::PlayCollisionSounds()
{
    int numManifolds = this->physics.dynamicsWorld->getDispatcher()->getNumManifolds();
    if(!numManifolds) return;
    btPersistentManifold* cc = this->physics.dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(0);
    if(!cc->getNumContacts()) return;

    printM("\n\n###############################################\n\nPlayCollisionSounds\n\n");
    DebugPrint("  ", "numManifolds", numManifolds);
    for (int i=0;i<numManifolds;i++)
    {
        DebugPrint("    ", "manifold", i);
	    btPersistentManifold* contactManifold = this->physics.dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
	    btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
	    btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

        static const char *tab = "      ";
        DebugPrint(tab, "contactManifold", (void*)contactManifold);
        DebugPrint(tab, "obA", (void*)obA);
        DebugPrint(tab, "obA vel", obA->getInterpolationLinearVelocity());
        DebugPrint(tab, "obA speed", obA->getInterpolationLinearVelocity().length());
        DebugPrint(tab, "obB", (void*)obB);
        DebugPrint(tab, "obB vel", obB->getInterpolationLinearVelocity());
        DebugPrint(tab, "obB speed", obB->getInterpolationLinearVelocity().length());
        btScalar speed = (obA->getInterpolationLinearVelocity() - obB->getInterpolationLinearVelocity()).length();
        DebugPrint(tab, "rel speed", speed);

        if(speed < 0.2f) continue;

	    int numContacts = contactManifold->getNumContacts();
        DebugPrint(tab, "numContacts", numContacts);
	    for (int j=0;j<numContacts;j++)
	    {
            DebugPrint("        ", "contact", j);
		    btManifoldPoint& cp = contactManifold->getContactPoint(j);
            PrintManifold(cp);

            if(cp.m_lifeTime > 1) continue;
            const btVector3 &pos = cp.getPositionWorldOnA();
            //float vol = speed / 10.0f;
            float vol = speed / 5.0f;
            if(vol > 1.0f) vol = 1.0f;
            this->audioPlayer->PlayEvent(fmodCollideEventId[rand()%3], 
                                         Math::matrix44::translation(pos.x(), pos.y(), pos.z()),
                                         Math::vector::nullvec(),
                                         vol);
	    }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BDApplication::OnUpdateFrame()
{    
    this->audioListener->SetTransform(this->camera->GetTransform());
    this->audioServer->OnFrame();

    Timing::Time delta_t = this->GetFrameTime();
    if(delta_t > 0.0f && this->blnSimulationRunning)
    {
        this->physics.dynamicsWorld->stepSimulation((btScalar)delta_t, 10000);

#ifndef BT_NO_PROFILE
	    {
		    static int i = 0;
		    if(i > 10)
		    {
			    CProfileManager::dumpAll();
			    i = 0;
		    }
		    else
            {
			    ++i;
            }
	    }
#endif
    }

    for(int i = 0; i < BOX_COUNT; ++i)
    {
        this->boxes[i].UpdateNebulaModelTransform();
    }

    for(int i = 0; i < this->flyingBoxes.Size(); ++i)
    {
        this->flyingBoxes[i]->UpdateNebulaModelTransform();
    }

    //this->physics.dynamicsWorld->debugDrawWorld();
    //this->DrawShapeInfo(this->rigidbodySpheresCharacter);

    //this->physics.dynamicsWorld->debugDrawObject(this->boxBody->getWorldTransform(), this->boxBody->getCollisionShape(), btVector3(0.0f, 1.0f, 0.0f));
    //this->physics.dynamicsWorld->debugDrawObject(this->groundBody->getWorldTransform(), this->groundBody->getCollisionShape(), btVector3(0.0f, 0.0f, 1.0f));

    //this->PlayCollisionSounds();

    // test text rendering
    Timing::Time frameTime = (float)this->GetFrameTime();
    Util::String fpsTxt;
    fpsTxt.Format("Game FPS: %.2f", 1/frameTime);
    _debug_text(fpsTxt, Math::float2(0.0,0.0), Math::float4(1,1,1,1));
    _debug_text("Toggle Benchmark with F3!", Math::float2(0.0,0.025), Math::float4(1,1,1,1));

    if (this->benchmarkmode)
    {
        // benchmark with avg frames for every 100 frames
        if (this->frameTimes.Size() > 0 || frameTime < 0.08)
        {
            this->frameTimes.Append(frameTime);
        }
        if (this->frameTimes.Size() > 0 && this->frameTimes.Size() % 50 == 0)
        {
            this->avgFPS = 0;
            IndexT i;
            for (i = 0; i < this->frameTimes.Size(); ++i)
            {
        	    this->avgFPS += this->frameTimes[i];
            }
            this->avgFPS /=  this->frameTimes.Size();
        }
        if (this->avgFPS > 0)
        {
            fpsTxt.Format("Benchmarking: Avg Game FPS: %.2f", 1/this->avgFPS);
        }
        else
        {
            fpsTxt = "Benchmarking: Wait for measuring average framerate";
        }
        _debug_text(fpsTxt, Math::float2(0.0,0.05f), Math::float4(1,1,1,1));
    }

    float time = (float) this->GetTime();
    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        matrix44 m = this->lightTransforms[i];
        float4 t(5 * n_sin(0.1f * i * time), 0.0f, 5 * n_sin(0.05f * i * time), 0.0f);
        m.translate(t);
        this->pointLights[i]->SetTransform(m);
    }

    // rotate spotlights
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        matrix44 spotLightTransform = matrix44::rotationyawpitchroll(time * 2 * i / this->spotLights.Size(), n_deg2rad(-55), 0);
        spotLightTransform.scale(point(i * 0.2f + 15, 30, i * 0.2f + 15));
        spotLightTransform.set_position(this->spotLights[i]->GetTransform().get_position());
        this->spotLights[i]->SetTransform(spotLightTransform);
    }

    ViewerApplication::OnUpdateFrame();
}
} // namespace Tools
