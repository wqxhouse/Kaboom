#include "World.h"

World::World()
        : dispatcher(&collisionConfiguration),
          world(&dispatcher, &broadphase, &solver, &collisionConfiguration) {
    setGravity(4.0f);
}

World::~World() {
}

void World::loadMap() {
    // TODO: Fix memory leak.
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1)));

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    addRigidBody(groundRigidBody);
}

void World::stepSimulation(float timeStep) {
    world.stepSimulation(timeStep);
}

void World::addRigidBody(btRigidBody *rigidBody) {
    world.addRigidBody(rigidBody);
}

void World::removeRigidBody(btRigidBody *rigidBody) {
    world.removeRigidBody(rigidBody);
}

void World::setGravity(float gravity) {
    world.setGravity(btVector3(0, 0, -gravity));
}

const btCollisionDispatcher &World::getDispatcher() const {
    return dispatcher;
}
