#include "World.h"

World::World()
        : dispatcher(&collisionConfiguration),
          world(&dispatcher, &broadphase, &solver, &collisionConfiguration) {
    setGravity(4.0f);
}

World::~World() {
}

void World::loadMap() {
	/*
	btTransform transform;
	transform.setIdentity();//reset x,y,z and rotation to 0
	transform.setOrigin(btVector3(0,0,0)); //origin at 0
	
    // TODO: Fix memory leak.
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0); //have the plane lays on the z axis, 1 means to face up
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(transform);//btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1)));
															 //mass, motionshape,		collisionShape, localInertia
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,    groundMotionState, groundShape, btVector3(0, 0, 0));//mass = 0, means static objects!
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    addRigidBody(groundRigidBody);

	//add a wall infront
	btTransform transform2;
	transform2.setIdentity();//reset x,y,z and rotation to 0
	transform2.setOrigin(btVector3(0, -10, 0)); 

	btCollisionShape *groundShape2 = new btStaticPlaneShape(btVector3(0, 1, 0), 0); //have the plane lays on the y axis, 1 means to face up
	btDefaultMotionState *groundMotionState2 = new btDefaultMotionState(transform2);//btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1)));
															//mass, motionshape,		collisionShape, localInertia
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI2(0, groundMotionState2, groundShape2, btVector3(0, 0, 0));//mass = 0, means static objects!
	btRigidBody *groundRigidBody2 = new btRigidBody(groundRigidBodyCI2);

	addRigidBody(groundRigidBody2);
	*/



	addStaticPlane(btVector3(0,0,0), btVector3(0,0,1));//floor
	addStaticPlane(btVector3(0, -10, 0), btVector3(0, 1, 0));//back wall
	addStaticPlane(btVector3(0, 10, 0), btVector3(0, -1, 0));//front wall
	addStaticPlane(btVector3(-10, 0, 0), btVector3(1, 0, 0));//left wall
	addStaticPlane(btVector3(10, 0, 0), btVector3(-1, 0, 0));//right wall

	//add a ramp
	
	btQuaternion quat;
	quat.setRotation(btVector3(0, 1, 0), btRadians(btScalar(30)));
	addStaticPlane(btVector3(-5, 0, 0), btVector3(0, 0, 1), quat);
	
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


void World::addStaticPlane(btVector3 origin, btVector3 normal) {
	btTransform startTrans;
	startTrans.setIdentity();
	startTrans.setOrigin(origin);

	btCollisionShape *groundShape = new btStaticPlaneShape(normal, 0);
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(startTrans);
															//mass, motionshape,		collisionShape, localInertia
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));//mass = 0, means static objects!
	btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

	addRigidBody(groundRigidBody);
}


void World::addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation) {

	btTransform startTrans;
	startTrans.setIdentity();
	startTrans.setOrigin(origin);
	startTrans.setRotation(rotation);

	btCollisionShape *groundShape = new btStaticPlaneShape(normal, 0);
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(startTrans);
	//mass, motionshape,		collisionShape, localInertia
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));//mass = 0, means static objects!
	btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

	addRigidBody(groundRigidBody);
}
const btCollisionDispatcher &World::getDispatcher() const {
    return dispatcher;

}
