#pragma once

#include <btBulletDynamicsCommon.h>

class World {
public:
    World();
    ~World();

    void loadMap();

    void stepSimulation(float timeStep);

    void addRigidBody(btRigidBody *rigidBody);
    void removeRigidBody(btRigidBody *rigidBody);

    void setGravity(float gravity);


	void addStaticPlane(btVector3 origin, btVector3 normal);
	void addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation);

    const btCollisionDispatcher &getDispatcher() const;


private:
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;
};
