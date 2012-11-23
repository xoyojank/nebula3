//------------------------------------------------------------------------------
//  bcapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "bcapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "debug/debugfloat.h"

#if __PS3__
#include "BulletMultiThreaded/btThreadSupportInterface.h"
#include "SpuDispatch/BulletCollisionSpursSupport.h"
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#endif


Math::matrix44 operator*(const Math::matrix44 &a, const Math::matrix44 &b)
{
    return Math::matrix44::multiply(a, b);
}

Math::vector Bt2NebVector(const btVector3 &btV)
{
    return Math::vector(btV.x(), btV.y(), btV.z());
}

Math::point Bt2NebPoint(const btVector3 &btV)
{
    return Math::point(btV.x(), btV.y(), btV.z());
}

Math::matrix44 Bt2NebMatrix3x3(const btMatrix3x3 &m)
{
    return Math::matrix44(Math::float4(m.getRow(0).x(), m.getRow(1).x(), m.getRow(2).x(), 0.0f), 
                          Math::float4(m.getRow(0).y(), m.getRow(1).y(), m.getRow(2).y(), 0.0f), 
                          Math::float4(m.getRow(0).z(), m.getRow(1).z(), m.getRow(2).z(), 0.0f), 
                          Math::float4(0.0f, 0.0f, 0.0f, 1.0f));
}

Math::matrix44 Bt2NebTransform(const btTransform &t)
{
    const btMatrix3x3 &rot = t.getBasis();
    Math::matrix44 m = Bt2NebMatrix3x3(rot);
    m.set_position(Bt2NebPoint(t.getOrigin()));
    return m;
}

btMatrix3x3 Neb2BtM44M33(const Math::matrix44 &m)
{
    return btMatrix3x3(m.getrow0().x(), m.getrow1().x(), m.getrow2().x(), 
                       m.getrow0().y(), m.getrow1().y(), m.getrow2().y(), 
                       m.getrow0().z(), m.getrow1().z(), m.getrow2().z());
}

btTransform Neb2BtM44Transform(const Math::matrix44 &m)
{
    btTransform transform;
    transform.setBasis(Neb2BtM44M33(m));
    transform.setOrigin(Neb2BtPoint(m.get_position()));
    return transform;
}

btVector3 Neb2BtPoint(const Math::point &p)
{
    return btVector3(p.x(), p.y(), p.z());
}

void    
PhysicsObject::SetPosition(const Math::point &pos)
{
    n_assert(this->bulletGeo.collisionObject);
    btTransform &transform = this->bulletGeo.collisionObject->getWorldTransform();
    transform.setOrigin(Neb2BtPoint(pos));   
    this->UpdateNebulaStateFromBullet();
}

void                
PhysicsObject::SetTransform(const btTransform &t)
{
    n_assert(this->bulletGeo.collisionObject);
    this->bulletGeo.collisionObject->setWorldTransform(t);
    this->UpdateNebulaStateFromBullet();
}

void                    
PhysicsObject::SetTransform(const Math::matrix44 &t)
{
    btTransform trans = Neb2BtM44Transform(t);
    this->bulletGeo.collisionObject->setWorldTransform(trans);
    this->UpdateNebulaStateFromBullet();
}

void    
PhysicsObject::UpdateNebulaStateFromBullet()
{
/*
    n_assert(this->bulletGeo.collisionObject);
    n_assert(this->nebulaGfxModel.model.isvalid());

    Math::matrix44 trans = Bt2NebTransform(this->bulletGeo.collisionObject->getWorldTransform());
    this->nebulaGfxModel.model->SetTransform(trans);
*/
}

Math::matrix44
PhysicsObject::GetTransform() const
{
    n_assert(this->bulletGeo.collisionObject);
    return Bt2NebTransform(this->bulletGeo.collisionObject->getWorldTransform());
}

const btTransform&      
PhysicsObject::GetBtTransform() const
{
    return this->bulletGeo.collisionObject->getWorldTransform();
}


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

//------------------------------------------------------------------------------
/**
*/
BCApplication::BCApplication() :
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false),
    rotX(-90),
    physicsInputMode(-1)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BCApplication::~BCApplication()
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
BCApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(0.1, 0.1, 0.1f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 0.25f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.1f, 0.1f, 0.1f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        // create a few point lights
        IndexT lightIndex;
        const SizeT numLights = 0;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {
            float x = n_rand() * 60.0f - 20.0f;
            float y = 3;
            float z = n_rand() * 60.0f - 20.0f;
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
            float x = n_rand() * 50.0f - 50.0f;
            float y = 10;
            float z = n_rand() * 50.0f - 50.0f;
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

        // create one shadow casting light
        matrix44 transform = matrix44::rotationx(n_deg2rad(this->rotX));
        transform.scale(point(11,11,11));
        transform.set_position(point(0,10,0));
        this->testSpotLight = SpotLightEntity::Create();
        this->testSpotLight->SetTransform(transform);
        this->testSpotLight->SetCastShadows(true);
        this->testSpotLight->SetColor(float4(2,2,1,1));
        this->stage->AttachEntity(this->testSpotLight.cast<GraphicsEntity>());

        // setup models
        this->ground = ModelEntity::Create();
        this->ground->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
        this->ground->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(ground.cast<GraphicsEntity>());

        IndexT j;
        IndexT i;
        bool createStatic = false;
        for (j = 0; j < 1; ++j)
        {
            for (i = 0; i < 1; ++i)
            {
                Ptr<ModelEntity> model = ModelEntity::Create();
                model->SetTransform(matrix44::translation(2.0f * i, 2.0, 0.0f));
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

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::InitPhysicsWorld()
{
    n_assert(!this->physics.collisionConfiguration);
    const btVector3 worldAabbMin(-1000,-1000,-1000);
    const btVector3 worldAabbMax(1000,1000,1000);
/*
    btDefaultCollisionConstructionInfo i;
    i.m_useEpaPenetrationAlgorithm = false;
    this->physics.collisionConfiguration = n_new(btDefaultCollisionConfiguration(i));
*/
    this->physics.collisionConfiguration = n_new(btDefaultCollisionConfiguration());

#if __PS3__
    CellSpurs* spursInst = Jobs::JobSystem::Instance()->PS3GetSpursInstance();
    int numSpuTasks = 6; 
    this->physics.colThreadSupport = n_new(BulletCollisionSpursSupport(spursInst, numSpuTasks, numSpuTasks)); 
    this->physics.dispatcher = n_new(SpuGatheringCollisionDispatcher(this->physics.colThreadSupport, numSpuTasks, this->physics.collisionConfiguration));
#else
    this->physics.dispatcher = n_new(btCollisionDispatcher(this->physics.collisionConfiguration));
#endif
    this->physics.broadphase = n_new(btAxisSweep3(worldAabbMin,worldAabbMax));
    this->physics.collisionWorld = n_new(btCollisionWorld(this->physics.dispatcher, this->physics.broadphase, this->physics.collisionConfiguration));

    //this->physics.collisionWorld->getDispatchInfo().m_enableSPU = true;

#if 0
    //btMatrix3x3 mBt;
    Math::matrix44 mNeb = matrix44::rotationx(1.0f);
    Debug::DebugFloat::print(mNeb, "mNeb");
    float m[16];
    memcpy(m, &mNeb.getrow0(), sizeof(float) * 16);
    n_printf("neb linear:\n");
    for(int i = 0; i < 12; ++i)
    {
        if( (i+1)%4 )
        {
            n_printf("  m[%02i] = %f\n", i, m[i]);
        }
    }
    n_printf("neb:\n");
    float4 row = mNeb.getrow0();
    n_printf("  bt[0][0] = %f\n", row.x());
    n_printf("  bt[0][1] = %f\n", row.y());
    n_printf("  bt[0][2] = %f\n", row.z());
    row = mNeb.getrow1();
    n_printf("  bt[1][0] = %f\n", row.x());
    n_printf("  bt[1][1] = %f\n", row.y());
    n_printf("  bt[1][2] = %f\n", row.z());
    row = mNeb.getrow2();
    n_printf("  bt[2][0] = %f\n", row.x());
    n_printf("  bt[2][1] = %f\n", row.y());
    n_printf("  bt[2][2] = %f\n", row.z());

    btMatrix3x3 mBt;
    mBt.setFromOpenGLSubMatrix(m);

    n_printf("bullet linear:\n");
    memcpy(m, &mBt, sizeof(float)*12);
    for(int i = 0; i < 12; ++i)
    {
        if( (i+1)%4 )
        {
            n_printf("  m[%02i] = %f\n", i, m[i]);
        }
    }
    n_printf("bullet:\n");
    for(int i = 0; i <3; ++i)
    {
        const btVector3 &r = mBt.getRow(i);
        n_printf("  bt[%i][0] = %f\n", i, r.x());
        n_printf("  bt[%i][1] = %f\n", i, r.y());
        n_printf("  bt[%i][2] = %f\n", i, r.z());
    }

    Math::vector vN(0.0f, 1.0f, 0.0f), vNa;
    vNa = matrix44::transform(vN, mNeb);
    n_printf("Neb: %f %f %f\n", vNa.x(), vNa.y(), vNa.z());

    btVector3 v(0.0f, 1.0f, 0.0f), va, vb;
    btMatrix3x3 a, b;

    va = mBt * v;
    vb = v * mBt;

    n_printf("    %f %f %f\n", va.x(), va.y(), va.z());
    n_printf("    %f %f %f\n", vb.x(), vb.y(), vb.z());

#endif

#if 0
    Math::matrix44 mNeb = matrix44::rotationx(1.0f);
    btMatrix3x3 mBt = Neb2BtM44M33(mNeb);

    float m[16];
    memcpy(m, &mNeb.getrow0(), sizeof(float) * 16);
    n_printf("neb linear:\n");
    for(int i = 0; i < 12; ++i)
    {
        if( (i+1)%4 )
        {
            n_printf("  m[%02i] = %f\n", i, m[i]);
        }
    }

    n_printf("bullet linear:\n");
    memcpy(m, &mBt, sizeof(float)*12);
    for(int i = 0; i < 12; ++i)
    {
        if( (i+1)%4 )
        {
            n_printf("  m[%02i] = %f\n", i, m[i]);
        }
    }

    Math::matrix44 test = Bt2NebMatrix3x3(mBt);
    memcpy(m, &test.getrow0(), sizeof(float) * 16);
    n_printf("neb linear:\n");
    for(int i = 0; i < 12; ++i)
    {
        if( (i+1)%4 )
        {
            n_printf("  m[%02i] = %f\n", i, m[i]);
        }
    }
#endif

#if 0
    Math::matrix44 mNebRotX = matrix44::rotationx(1.0f);
    Math::matrix44 mNebRotY = matrix44::rotationy(0.25f);
    Math::matrix44 mNebRot = matrix44::multiply(mNebRotX, mNebRotY);

    btMatrix3x3 mBtRotX = Neb2BtM44M33(mNebRotX);
    btMatrix3x3 mBtRotY = Neb2BtM44M33(mNebRotY);
    btMatrix3x3 mBtRot = mBtRotY * mBtRotX;
    Math::matrix44 test = Bt2NebMatrix3x3(mBtRot);

    Debug::DebugFloat::print(mNebRot, "mNebRot");
    Debug::DebugFloat::print(test, "test");
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::InitPhysicsObjects()
{
    btTransform startTransform[NUM_PHYSICS_OBJECTS];
    Math::vector halfBoxExtents[NUM_PHYSICS_OBJECTS];

    float radius = 1.0f;

    startTransform[0].setIdentity();
    startTransform[0].setOrigin(btVector3(0.0f, radius, 0.0f));
    halfBoxExtents[0].set(radius, radius, radius);

    startTransform[1].setIdentity();
    startTransform[1].setOrigin(btVector3(3.0f, radius, 0.0f)); 
    halfBoxExtents[1].set(radius, radius, radius);

    startTransform[2].setIdentity();
    startTransform[2].setOrigin(btVector3(-2.0f, 1.0f, -2.0f)); 
    halfBoxExtents[2].set(0.3f, 1.0f, 0.7f);

    startTransform[3].setIdentity();
    startTransform[3].setOrigin(btVector3(-2.0f, 1.0f, 2.0f)); 
    halfBoxExtents[3].set(0.3f, 1.0f, 0.7f);

	for(int i = 0; i < NUM_PHYSICS_OBJECTS; ++i)
	{
        this->objects[i].halfBoxExtents = halfBoxExtents[i];

        if(i < 2)
        {
            this->objects[i].bulletGeo.shape = n_new(btSphereShape(this->objects[i].halfBoxExtents.x()));
        }
        else
        {
            this->objects[i].bulletGeo.shape = n_new(btBoxShape(btVector3(this->objects[i].halfBoxExtents.x(), this->objects[i].halfBoxExtents.y(), this->objects[i].halfBoxExtents.z())));
        }

		this->objects[i].bulletGeo.collisionObject = n_new(btCollisionObject());
		this->objects[i].bulletGeo.collisionObject->setCollisionShape(this->objects[i].bulletGeo.shape);
        this->objects[i].bulletGeo.collisionObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		this->physics.collisionWorld->addCollisionObject(this->objects[i].bulletGeo.collisionObject);

/*
        // setup nebula model
        this->objects[i].nebulaGfxModel.model = ModelEntity::Create();
        this->objects[i].nebulaGfxModel.model->SetResourceId(ResourceId("mdl:examples/boxunit.n3"));
        this->stage->AttachEntity(this->objects[i].nebulaGfxModel.model.cast<GraphicsEntity>());
*/

        this->objects[i].SetTransform(startTransform[i]);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::DestroyPhysicsObjects()
{
    if(!this->physics.collisionWorld) return;

    for(int i = 0; i < NUM_PHYSICS_OBJECTS; ++i)
    {
/*
        if(this->objects[i].nebulaGfxModel.model.isvalid())
        {
            this->stage->RemoveEntity(this->objects[i].nebulaGfxModel.model.cast<GraphicsEntity>());
            this->objects[i].nebulaGfxModel.model = 0;
        }
*/
        if(this->objects[i].bulletGeo.collisionObject)
        {
            this->physics.collisionWorld->removeCollisionObject(this->objects[i].bulletGeo.collisionObject);
            n_delete(this->objects[i].bulletGeo.collisionObject);
            this->objects[i].bulletGeo.collisionObject = NULL;
        }

        if(this->objects[i].bulletGeo.shape) 
        {
            n_delete(this->objects[i].bulletGeo.shape);
            this->objects[i].bulletGeo.shape = NULL;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::DestroyPhysicsWorld()
{
    if(!this->physics.collisionWorld) return;

    n_assert(this->physics.broadphase);
    n_assert(this->physics.dispatcher);
    n_assert(this->physics.collisionConfiguration);

    n_delete(this->physics.collisionWorld);
    n_delete(this->physics.broadphase);
    n_delete(this->physics.dispatcher);
#if __PS3__
    n_delete(this->physics.colThreadSupport);
#endif
    n_delete(this->physics.collisionConfiguration);

    this->physics.collisionWorld = NULL;
    this->physics.broadphase = NULL;
    this->physics.dispatcher = NULL;
#if __PS3__
    this->physics.colThreadSupport = NULL;
#endif
    this->physics.collisionConfiguration = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::Close()
{
    this->DestroyPhysicsObjects();
    this->DestroyPhysicsWorld();

    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());
    this->globalLight = 0;
    this->ground = 0;

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

    this->stage->RemoveEntity(this->testSpotLight.cast<GraphicsEntity>());
    this->testSpotLight = 0;

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
BCApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
BCApplication::OnProcessInput()
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

    const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);
    if (gamePad->IsConnected())
    {
        if (gamePad->ButtonDown(GamePad::R1Button))
        {
            ++this->physicsInputMode;
            if(NUM_PHYSICS_OBJECTS == this->physicsInputMode) this->physicsInputMode = -1;
        }
        if(this->physicsInputMode > -1)
        {
            const float frameTime = (float) this->GetFrameTime();

            PhysicsObject &object = this->objects[this->physicsInputMode];
            const Math::matrix44 &transform = object.GetTransform();

            {
                // translate
                const float transAxisX = gamePad->GetAxisValue(GamePad::RightThumbXAxis);
                const float transAxisY = gamePad->GetAxisValue(GamePad::RightThumbYAxis);
                Math::point pos = transform.get_position();
                static const float speedLinear = 2.0f;
                bool posChanged = false;
                if(transAxisX != 0.0f)
                {
                    pos.x() += (transAxisX * speedLinear) * frameTime;
                    posChanged = true;
                }
                if(transAxisY != 0.0f)
                {
                    pos.z() -= (transAxisY * speedLinear) * frameTime;
                    posChanged = true;
                }
                if(posChanged) object.SetPosition(pos);
            }

            {
                // rotate
                const float rotAxisX = gamePad->GetAxisValue(GamePad::LeftThumbXAxis);
                const float rotAxisY = gamePad->GetAxisValue(GamePad::LeftThumbYAxis);
                if(rotAxisX != 0.0f || rotAxisY != 0.0f)
                {
                    static const float speedAngular = 2.5f;
                    Math::vector rotAxis(rotAxisY, rotAxisX, 0.0f);
                    //Math::vector rotAxis(fabs(rotAxisY) > fabs(rotAxisX) ? rotAxisY : 0.0f, 
                    //                     fabs(rotAxisX) > fabs(rotAxisY) ? rotAxisX : 0.0f, 
                    //                     0.0f);
                    rotAxis = float4::normalize(rotAxis);
                    rotAxis = matrix44::transform(rotAxis, Math::matrix44::inverse(transform));
                    Math::matrix44 rot = Math::matrix44::rotationaxis(rotAxis, speedAngular * frameTime);
                    Math::matrix44 newTrans = rot * transform;
                    object.SetTransform(newTrans);
                }
            }

            return;
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
BCApplication::OnUpdateFrame()
{
    this->physics.collisionWorld->performDiscreteCollisionDetection();
    int numManifolds = this->physics.collisionWorld->getDispatcher()->getNumManifolds();

    //if(!numManifolds)
    {
        for(int i = 0; i < NUM_PHYSICS_OBJECTS; ++i)
        {
            //Math::matrix44 t = this->objects[i].GetTransform();
            
            //const Math::vector boxHalfWidth(1.0f, 1.0f, 1.0f);
            
            const Math::vector &boxHalfWidth = this->objects[i].halfBoxExtents;
            Math::matrix44 scale = matrix44::scaling(boxHalfWidth.x(), boxHalfWidth.y(), boxHalfWidth.z());
            const btTransform &btTrans = this->objects[i].GetBtTransform();
            const btMatrix3x3 &m3x3 = btTrans.getBasis();
            const btVector3 &pos = btTrans.getOrigin();
            Math::matrix44 rot = Bt2NebMatrix3x3(m3x3);
            Math::matrix44 translate = matrix44::translation(pos.x(), pos.y(), pos.z());
            
            Math::matrix44 m = scale * rot * translate;

            float red = 0.5f;
            float green = 0.3f;
            float blue = (i == this->physicsInputMode) ? green : 0.1f;
            int shapeType = this->objects[i].bulletGeo.shape->getShapeType();
            switch(shapeType)
            {
                case BOX_SHAPE_PROXYTYPE:
                    DebugShapeRenderer::Instance()->DrawBox(m, float4(red, green, blue, 0.5f));
                    break;
                case SPHERE_SHAPE_PROXYTYPE:
                    DebugShapeRenderer::Instance()->DrawSphere(m, float4(red, green, blue, 0.5f));
                    break;
            }        
        }
    }

    if(numManifolds) n_printf("\n\n");

    bool firstColl = true;
    bool collision = false;
    Math::matrix44 scalePoint = Math::matrix44::scaling(0.01f, 0.01f, 0.01f);
	for(int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = this->physics.collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			btVector3 ptA = pt.getPositionWorldOnA();
			btVector3 ptB = pt.getPositionWorldOnB();

if(firstColl) { n_printf("\n\n"); firstColl = false; }
n_printf("numManifolds: %d\n", i);
n_printf("  contact: %d\n", j);
n_printf("    ptA: %f %f %f\n", ptA.x(), ptA.y(), ptA.z());
n_printf("    ptB: %f %f %f\n", ptB.x(), ptB.y(), ptB.z());
            
            Math::matrix44 m = scalePoint * Math::matrix44::translation(ptA.x(), ptA.y(), ptA.z());
            DebugShapeRenderer::Instance()->DrawSphere(m, float4(1.0f, 0.6f, 0.7f, 1.0f));

            collision = true;
/*
            glBegin(GL_LINES);
			glColor3f(1, 0, 1);
			
			btVector3 ptA = pt.getPositionWorldOnA();
			btVector3 ptB = pt.getPositionWorldOnB();

			glVertex3d(ptA.x(),ptA.y(),ptA.z());
			glVertex3d(ptB.x(),ptB.y(),ptB.z());
			glEnd();
*/
		}

		//you can un-comment out this line, and then all points are removed
		//contactManifold->clearManifold();	
	}

    if(collision)
    {
        this->globalLight->SetColor(float4(0.2f, 0.0f, 0.0f, 1.0f));
    }
    else
    {
        this->globalLight->SetColor(float4(0.1f, 0.1f, 0.1f, 1.0f));
    }

/*
    DebugShapeRenderer::Instance()->DrawBox(matrix44::identity(), float4(1.0f, 0.5f, 0.0f, 0.5f));
    DebugShapeRenderer::Instance()->DrawSphere(matrix44::translation(-2.0f, 0.0f, 0.0f), float4(0.0f, 0.5f, 0.0f, 0.5f));
    DebugShapeRenderer::Instance()->DrawCylinder(matrix44::translation(-1.0f, 3.0f, 0.0f), float4(1.0f, 0.5f, 0.2f, 0.5f));
    DebugShapeRenderer::Instance()->DrawTorus(matrix44::translation(3.0f, 3.0f, 0.0f), float4(0.3f, 0.5f, 0.2f, 0.5f));
*/

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
    // render a few debug shapes
    /*IndexT x;
    for (x = 0; x < 10; x++)
    {
        IndexT y;
        for (y = 0; y < 10; y++)
        {
            quaternion rot = quaternion::rotationyawpitchroll(0.0f, curTime, 0.0f);
            matrix44 m = matrix44::affinetransformation(1.0f, vector::nullvec(), rot, point(x * 2.0f, 1.0f, y * 2.0f));
            DebugShapeRenderer::Instance()->DrawBox(m, float4(1.0f, 0.0f, 0.0f, 0.5f));
        }
    }*/

    // move light
    Ptr<Input::GamePad> pad = Input::InputServer::Instance()->GetDefaultGamePad(0);
    n_assert(pad.isvalid());
    matrix44 tr = this->testSpotLight->GetTransform();
    if (pad->ButtonPressed(Input::GamePad::DPadLeftButton))
    {
        tr.translate(vector(0, 0, -0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadRightButton))
    {
        tr.translate(vector(0, 0, 0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadUpButton))
    {
        tr.translate(vector(0, 0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadDownButton))
    {
        tr.translate(vector(0, -0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::L1Button))
    {
        this->rotX += 0.5f;
        matrix44 rot = matrix44::rotationx(n_deg2rad(this->rotX));
        rot.scale(point(11,11,11));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    if (pad->ButtonPressed(Input::GamePad::R1Button))
    {
        this->rotX -= 0.5f;
        matrix44 rot = matrix44::rotationx(n_deg2rad(this->rotX));
        rot.scale(point(11,11,11));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    this->testSpotLight->SetTransform(tr);

    ViewerApplication::OnUpdateFrame();
}
} // namespace Tools
