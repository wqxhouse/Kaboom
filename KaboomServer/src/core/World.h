#pragma once

#include <string>
#include <unordered_map>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <util/Configuration.h>

class ConfigSettings;
class Entity;

class World {
public:
    typedef std::unordered_map<std::string, Configuration> SpawnPointToConfigMap;

    World(ConfigSettings* configSettings);

    void load(const std::string &mapFilename, const std::string &entitiesFilename);

    virtual void stepSimulation(float timeStep, int maxSubSteps);

    virtual void addRigidBody(btRigidBody *rigidBody);
    virtual void removeRigidBody(btRigidBody *rigidBody);

    void addTrigger(btGhostObject *ghostObject);
    void removeTrigger(btGhostObject *ghostObject);

    void setGravity(float gravity);

    void onTick(btScalar timeStep);

    inline SpawnPointToConfigMap &getSpawnPointConfigs() {
        return spawnPointConfigs;
    }

protected:
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;

private:
    ConfigSettings* configSettings;

    SpawnPointToConfigMap spawnPointConfigs;

    void addStaticPlane(btVector3 origin, btVector3 normal);
    void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

    void handleCollision(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const;
    void handleTrigger(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const;

    bool isCollidingGround(const btManifoldPoint &contactPoint) const;
};
