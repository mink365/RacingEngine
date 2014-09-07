#ifndef BULLETTEST_H
#define BULLETTEST_H

#include "BaseTest.h"

#include "LinearMath/btAlignedObjectArray.h"

class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;

class BulletTest : public BaseTest
{
public:
    BulletTest();

    virtual void Init() override;
    virtual void Update(float dt) override;

protected:
    void initPhysics();
    void exitPhysics();

    void initView();
    void syncView();

private:
    //keep the collision shapes, for deletion/cleanup
    btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

    btDynamicsWorld* m_dynamicsWorld;

    btBroadphaseInterface*	m_broadphase;

    btCollisionDispatcher*	m_dispatcher;

    btConstraintSolver*	m_solver;

    btDefaultCollisionConfiguration* m_collisionConfiguration;

    std::vector<SceneNodePtr> boxList;
};

#endif // BULLETTEST_H
