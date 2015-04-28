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

    const btCollisionDispatcher &getDispatcher() const;

private:
    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfiguration;
    btCollisionDispatcher dispatcher;
    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld world;
};
