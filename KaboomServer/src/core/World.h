#pragma once

#include <string>
#include <unordered_map>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class ConfigSettings;
class Entity;

class World {
public:
    World(ConfigSettings* configSettings);

    void load(const std::string &mapXMLFile);

    virtual void stepSimulation(float timeStep, int maxSubSteps);

    virtual void addRigidBody(btRigidBody *rigidBody);
    void addTrigger(btGhostObject *ghostObject);

    virtual void removeRigidBody(btRigidBody *rigidBody);
    void removeTrigger(btGhostObject *ghostObject);

    void setGravity(float gravity);

    void onTick(btScalar timeStep);

    const btCollisionDispatcher &getDispatcher() const;

protected:
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;

private:
    class TriggerCallback;

    ConfigSettings* configSettings;

    void addStaticPlane(btVector3 origin, btVector3 normal);
    void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

    void handleCollision(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const;
    bool isCollidingGround(const btManifoldPoint &contactPoint) const;
};

class World::TriggerCallback : public btGhostPairCallback{
public:
    virtual btBroadphasePair *addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1);
    virtual void *removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher);

private:
    void addTriggerEntity(Entity *entityA, Entity *entityB) const;
    void removeTriggerEntity(Entity *entityA, Entity *entityB) const;
};
