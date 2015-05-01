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
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;

    void addStaticPlane(btVector3 origin, btVector3 normal);
    void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

    void handleCollision(Entity *entityA, Entity *entityB);
};
