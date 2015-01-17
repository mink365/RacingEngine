#include "BulletTest.h"

#include "ShapeGenerater.h"

#include "btBulletDynamicsCommon.h"

///create 125 (5x5x5) dynamic object
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 5

//maximum number of objects (and allow user to shoot additional boxes)
#define MAX_PROXIES (ARRAY_SIZE_X*ARRAY_SIZE_Y*ARRAY_SIZE_Z + 1024)

///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 3.
#define START_POS_X -5
#define START_POS_Y -5
#define START_POS_Z -3

///The MyOverlapCallback is used to show how to collect object that overlap with a given bounding box defined by aabbMin and aabbMax.
///See m_dynamicsWorld->getBroadphase()->aabbTest.
struct	MyOverlapCallback : public btBroadphaseAabbCallback
{
    btVector3 m_queryAabbMin;
    btVector3 m_queryAabbMax;

    int m_numOverlap;
    MyOverlapCallback(const btVector3& aabbMin, const btVector3& aabbMax ) : m_queryAabbMin(aabbMin),m_queryAabbMax(aabbMax),m_numOverlap(0)	{}
    virtual bool	process(const btBroadphaseProxy* proxy)
    {
        btVector3 proxyAabbMin,proxyAabbMax;
        btCollisionObject* colObj0 = (btCollisionObject*)proxy->m_clientObject;
        colObj0->getCollisionShape()->getAabb(colObj0->getWorldTransform(),proxyAabbMin,proxyAabbMax);
        if (TestAabbAgainstAabb2(proxyAabbMin,proxyAabbMax,m_queryAabbMin,m_queryAabbMax))
        {
            m_numOverlap++;
        }
        return true;
    }
};

BulletTest::BulletTest()
{
    this->name = "BulletTest";

    this->m_dynamicsWorld = nullptr;
    this->m_collisionConfiguration = nullptr;
}

void BulletTest::Init()
{
    if (this->m_dynamicsWorld) {
        this->exitPhysics();
    }

    this->initPhysics();

    this->initView();
}

void BulletTest::Update(float dt)
{
    //simple dynamics world doesn't handle fixed-time-stepping
    float ms = 20000;

    ///step the simulation
    if (m_dynamicsWorld)
    {
        m_dynamicsWorld->stepSimulation(ms / 1000000.f);
//        //optional but useful: debug drawing
//        m_dynamicsWorld->debugDrawWorld();

        btVector3 aabbMin(1,1,1);
        btVector3 aabbMax(2,2,2);

        MyOverlapCallback aabbOverlap(aabbMin,aabbMax);
        m_dynamicsWorld->getBroadphase()->aabbTest(aabbMin,aabbMax,aabbOverlap);

        if (aabbOverlap.m_numOverlap)
            printf("#aabb overlap = %d\n", aabbOverlap.m_numOverlap);
    }

    this->syncView();
}

void BulletTest::initPhysics()
{
    ///collision configuration contains default setup for memory, collision setup
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    //m_collisionConfiguration->setConvexConvexMultipointIterations();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

    m_broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
//    m_dynamicsWorld->setDebugDrawer(&gDebugDraw);

    m_dynamicsWorld->setGravity(btVector3(0,-10,0));

    ///create a few basic rigid bodies
    btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
    //groundShape->initializePolyhedralFeatures();
//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);

    m_collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,-50,0));

    //We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
    {
        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass,localInertia);

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        m_dynamicsWorld->addRigidBody(body);
    }


    {
        //create a few dynamic rigidbodies
        // Re-using the same collision is better for memory usage and performance

        btBoxShape* colShape = new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
        //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        m_collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar	mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass,localInertia);

        float start_x = START_POS_X - ARRAY_SIZE_X/2;
        float start_y = START_POS_Y;
        float start_z = START_POS_Z - ARRAY_SIZE_Z/2;

        for (int k=0;k<ARRAY_SIZE_Y;k++)
        {
            for (int i=0;i<ARRAY_SIZE_X;i++)
            {
                for(int j = 0;j<ARRAY_SIZE_Z;j++)
                {
                    startTransform.setOrigin(SCALING*btVector3(
                                        btScalar(2.0*i + start_x),
                                        btScalar(20+2.0*k + start_y),
                                        btScalar(2.0*j + start_z)));


                    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
                    btRigidBody* body = new btRigidBody(rbInfo);

                    m_dynamicsWorld->addRigidBody(body);
                }
            }
        }
    }
}

void BulletTest::exitPhysics()
{
    //cleanup in the reverse order of creation/initialization

    //remove the rigidbodies from the dynamics world and delete them
    int i;
    for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject( obj );
        delete obj;
    }

    //delete collision shapes
    for (int j=0;j<m_collisionShapes.size();j++)
    {
        btCollisionShape* shape = m_collisionShapes[j];
        delete shape;
    }
    m_collisionShapes.clear();

    delete m_dynamicsWorld;

    delete m_solver;

    delete m_broadphase;

    delete m_dispatcher;

    delete m_collisionConfiguration;

    boxList.clear();
}

void BulletTest::initView()
{
    TextureParser::getInstance().addTextures("Textures/Box", "png|jpg");
    auto texture = TextureManager::getInstance().getTexture("cube2");

    this->camera->setView(Vec3(0, 100, 170), Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->camera->setDepthField(10, 1320);
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });

    auto geometry = ShapeGenerater::getInstance().CreatePlane(100, 100, 30, 30);
    auto groundNode = CreateMeshNode();
    SetMeshData(groundNode, geometry, texture);

    groundNode->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(-90*DEG_TO_RAD, 0, 0)));
    rootNode->addChild(groundNode);

    texture = TextureManager::getInstance().getTexture("cube3");
    auto box = ShapeGenerater::getInstance().CreateBox(6, 6, 6);

    auto boxNode = CreateMeshNode();
    SetMeshData(groundNode, box, texture);

    for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=1 ;i--)
    {
        auto node = boxNode->clone();
        rootNode->addChild(node);
        boxList.push_back(node);
    }
}

void BulletTest::syncView()
{
    btTransform transform;

    for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=1 ;i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);

        body->getMotionState()->getWorldTransform(transform);

        auto v = transform.getOrigin();
        auto r = transform.getRotation();

//        std::cout << "Index: " << i << " Position: " << v.x() << " " << v.y() << " " << v.z() << std::endl;

        auto boxNode = boxList.at(i - 1);
        TransformPtr& transform = boxNode->getTransform();
        transform->setLocalTranslation(Vec3(v.x(),v.y(),v.z()));
        transform->setLocalRotation(Quat(r.x(),r.y(),r.z(),r.w()));
    }
}
