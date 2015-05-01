#pragma once

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <btBulletDynamicsCommon.h>

class Entity;

class World {
public:
    World();
    ~World();

    void loadMap();

    void stepSimulation(float timeStep);

    void addRigidBody(btRigidBody *rigidBody);
    void removeRigidBody(btRigidBody *rigidBody);

    void addTrigger(btGhostObject *ghostObject);
    void removeTrigger(btGhostObject *ghostObject);

    void setGravity(float gravity);

    void onTick(btScalar timeStep);

    const btCollisionDispatcher &getDispatcher() const;

private:
    class TriggerCallback;

    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;

    void addStaticPlane(btVector3 origin, btVector3 normal);
    void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

    void handleCollision(Entity *entityA, Entity *entityB) const;
};

class World::TriggerCallback : public btGhostPairCallback {
public:
    virtual btBroadphasePair *addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1);

private:
    void handleCollision(Entity *entityA, Entity *entityB) const;
};
