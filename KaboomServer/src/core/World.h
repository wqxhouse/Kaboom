#pragma once
#include <string>
#include <unordered_map>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <btBulletDynamicsCommon.h>

#include <osg/Node>
#include <osgDB/XmlParser>

#include <osg/MatrixTransform>

#include <osgbCollision/GLDebugDrawer.h>

#include <util/Configuration.h>
#include <util/ConfigSettings.h>

#include "OsgBulletDebugViewer.h"


class Entity;

class World {
public:
    World(ConfigSettings * );

    void loadMap();

	void loadMapFromXML(const std::string &mapXMLFile);

    void stepSimulation(float timeStep, int maxSubSteps);
	
    void addRigidBody(btRigidBody *rigidBody);

	void addRigidBodyAndConvertToOSG(btRigidBody *rigidBody);

    void removeRigidBody(btRigidBody *rigidBody);

    void addTrigger(btGhostObject *ghostObject);
    void removeTrigger(btGhostObject *ghostObject);

    void setGravity(float gravity);

    void onTick(btScalar timeStep);

    const btCollisionDispatcher &getDispatcher() const;

	void renderDebugFrame();

	void debugDrawWorld(); //use for updating the debug world frame

private:
    class TriggerCallback;

	std::string mediaPath;

	bool debugMode;

	std::unordered_map<std::string, Configuration> osgNodeConfigMap;

	OsgBulletDebugViewer* debugViewer;

	ConfigSettings* config;

    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;

    void addStaticPlane(btVector3 origin, btVector3 normal);
    void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

	void handleCollision(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const; 
	bool isCollidingGround(const btManifoldPoint &contactPoint) const;
};

class World::TriggerCallback : public btGhostPairCallback {
public:
    virtual btBroadphasePair *addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1);
    virtual void *removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher);

private:
    void addTriggerEntity(Entity *entityA, Entity *entityB) const;
    void removeTriggerEntity(Entity *entityA, Entity *entityB) const;
};
