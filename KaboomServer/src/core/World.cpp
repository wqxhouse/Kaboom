#include "World.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <core/Entity.h>
#include <core/EntityType.h>

#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>


#include "../core/OsgObjectConfigLoader.h"
#include "../components/CollisionComponent.h"
#include "../components/TriggerComponent.h"
#include "../components/JumpComponent.h"

void onTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    World *w = static_cast<World *>(world->getWorldUserInfo());
    w->onTick(timeStep);
}

World::World(ConfigSettings * configSettings)
        : dispatcher(&collisionConfiguration),
          world(&dispatcher, &broadphase, &solver, &collisionConfiguration),
		  config(configSettings){

	config->getValue(ConfigSettings::str_mediaFilePath, mediaPath);

    setGravity(9.8f);
    broadphase.getOverlappingPairCache()->setInternalGhostPairCallback(new TriggerCallback());
    world.setInternalTickCallback(onTickCallback, this);


	std::string debug;
	config->getValue(ConfigSettings::str_server_debug_mode, debug);
	debugMode = debug == "true" ? true : false;

	
	if (debugMode) {
		debugViewer = new OsgBulletDebugViewer(config);
		debugViewer->init();
		world.setDebugDrawer(debugViewer->getDbgDraw());
	}
}

World::~World() {
}

void World::loadMap() {

    addStaticPlane(btVector3(0, 0, 0), btVector3(0, 0, 1));//floor
    addStaticPlane(btVector3(0, -10, 0), btVector3(0, 1, 0));//back wall
    addStaticPlane(btVector3(0, 10, 0), btVector3(0, -1, 0));//front wall
    addStaticPlane(btVector3(-10, 0, 0), btVector3(1, 0, 0));//left wall
    addStaticPlane(btVector3(10, 0, 0), btVector3(-1, 0, 0));//right wall

    //add a ramp

    btQuaternion quat;
    quat.setRotation(btVector3(0, 1, 0), btRadians(btScalar(30)));
    addStaticPlane(btVector3(-5, 0, 0), btVector3(0, 0, 1), quat);
}

void World::loadMapFromXML(const std::string &mapXMLFile){

	OsgObjectConfigLoader osgObjectConfigLoader(osgNodeConfigMap);
	osgObjectConfigLoader.load(mapXMLFile);

	for (auto it = osgNodeConfigMap.begin(); it != osgNodeConfigMap.end(); ++it) {
		Configuration osgObjectConfig = it->second;

		std::string filePath= osgObjectConfig.getString("file");
		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(mediaPath + filePath);
		osg::ref_ptr<osg::MatrixTransform> transfromNode = new osg::MatrixTransform;
		transfromNode->addChild(node);

		osg::Matrix mat;

		osg::Vec3 pos, scale;
		osg::Quat rot, so;
		mat.decompose(pos, rot, scale, so);

		mat.makeTranslate(osgObjectConfig.getVec3("position"));
		mat.preMult(osg::Matrix::rotate(osg::Quat(osgObjectConfig.getVec4("orientation"))));
		mat.preMult(osg::Matrix::scale(osgObjectConfig.getVec3("scale")));

		transfromNode->setMatrix(mat);

		osgbDynamics::MotionState * motion = new osgbDynamics::MotionState;
		motion->setTransform(transfromNode);

		btCollisionShape * collisionShape = osgbCollision::btTriMeshCollisionShapeFromOSG(transfromNode);
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motion, collisionShape, btVector3(0, 0, 0));
		btRigidBody * rigidbody = new btRigidBody(rigidBodyCI);
		addRigidBody(rigidbody);

		if (debugMode) {
			debugViewer->addNode(transfromNode);
		}

	}
}

void World::stepSimulation(float timeStep, int maxSubSteps) {


    world.stepSimulation(timeStep, maxSubSteps);

	if (debugMode) {
		debugViewer->getDbgDraw()->BeginDraw();
		world.debugDrawWorld();
		debugViewer->getDbgDraw()->EndDraw();
	}
}

void World::addRigidBody(btRigidBody *rigidBody) {
	world.addRigidBody(rigidBody);
}

void World::addRigidBodyAndConvertToOSG(btRigidBody *rigidBody){
	if (debugMode) {
		osg::ref_ptr<osg::Node> node = osgbCollision::osgNodeFromBtCollisionShape(rigidBody->getCollisionShape());
		debugViewer->addNode(node);
	}

	world.addRigidBody(rigidBody);
}
void World::removeRigidBody(btRigidBody *rigidBody) {
    world.removeRigidBody(rigidBody);
}

void World::addTrigger(btGhostObject *ghostObject) {
    ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    world.addCollisionObject(
            ghostObject,
            btBroadphaseProxy::SensorTrigger,
            btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::SensorTrigger ^ btBroadphaseProxy::StaticFilter);
}

void World::removeTrigger(btGhostObject *ghostObject) {
    world.removeCollisionObject(ghostObject);
}

void World::setGravity(float gravity) {
    world.setGravity(btVector3(0, 0, -gravity));
}

void World::onTick(btScalar timeStep) {
    int numManifolds = dispatcher.getNumManifolds();

    for (int i = 0; i < numManifolds; ++i) {
        const btPersistentManifold *manifold = dispatcher.getManifoldByIndexInternal(i);

        int numContacts = manifold->getNumContacts();

        if (numContacts == 0) {
            continue;
        }

        const btCollisionObject *collisionObjA = static_cast<const btCollisionObject *>(manifold->getBody0());
        const btCollisionObject *collisionObjB = static_cast<const btCollisionObject *>(manifold->getBody1());

		btManifoldPoint contactPoint = manifold->getContactPoint(0);
		std::cout << "a: " << contactPoint.m_normalWorldOnB.getX() << " " <<
			contactPoint.m_normalWorldOnB.getY() << " " <<
			contactPoint.m_normalWorldOnB.getZ() << " " << std::endl;

        // Ignore ghost objects.
        if (!collisionObjA->hasContactResponse() || !collisionObjB->hasContactResponse()) {
            continue;
        }

        Entity *entityA = static_cast<Entity *>(collisionObjA->getUserPointer());
        Entity *entityB = static_cast<Entity *>(collisionObjB->getUserPointer());

        handleCollision(entityA, entityB, contactPoint);
        handleCollision(entityB, entityA, contactPoint);
    }
}

const btCollisionDispatcher &World::getDispatcher() const {
    return dispatcher;
}

void World::addStaticPlane(btVector3 origin, btVector3 normal) {
    addStaticPlane(origin, normal, btQuaternion::getIdentity());
}

void World::addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation) {
    btTransform startTrans;
    startTrans.setIdentity();
    startTrans.setOrigin(origin);
    startTrans.setRotation(rotation);

    btCollisionShape *groundShape = new btStaticPlaneShape(normal, 0);
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(startTrans);

    //mass, motionshape, collisionShape, localInertia
    //mass = 0, means static objects!
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    addRigidBody(groundRigidBody);
}

void World::handleCollision(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const {
    if (entityA != nullptr) {
      /*  CollisionComponent *colComp = entityA->getComponent<CollisionComponent>();

        if (colComp != nullptr) {
            colComp->setCollided(true);

            if (entityB != nullptr) {
                colComp->addContactEntity(entityB);*/
	
				// only test if one of them is a character
				if (entityA->hasComponent<JumpComponent>())
				{
					bool collideGround = isCollidingGround(contactPoint);
					if (collideGround)
					{
						entityA->getComponent<JumpComponent>()->setJumping(false);
					}
				}
				/*else if (entityB->hasComponent<JumpComponent>())
				{
				bool collideGround = isCollidingGround(contactPoint);
				entityB->getComponent<JumpComponent>()->setJumping(false);
				}*/
            //}
        //}
    }
}

bool World::isCollidingGround(const btManifoldPoint &contactPoint) const
{
	btVector3 normal = contactPoint.m_normalWorldOnB;
	btVector3 up = btVector3(0, 0, 1);
	double dot = normal.dot(up);

	// allow for error
	return dot > 0.05 ? true : false; 
}

void World::renderDebugFrame() {
	if (debugMode) {
		debugViewer->renderFrame();
	}
}

void World::debugDrawWorld() {
	world.debugDrawWorld();
}

btBroadphasePair *World::TriggerCallback::addOverlappingPair(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) {
    btCollisionObject *colObj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
    btCollisionObject *colObj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

    Entity *entity0 = static_cast<Entity *>(colObj0->getUserPointer());
    Entity *entity1 = static_cast<Entity *>(colObj1->getUserPointer());

    addTriggerEntity(entity0, entity1);
    addTriggerEntity(entity1, entity0);

    return btGhostPairCallback::addOverlappingPair(proxy0, proxy1);
}



void *World::TriggerCallback::removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher) {
    btCollisionObject *colObj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
    btCollisionObject *colObj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

    Entity *entity0 = static_cast<Entity *>(colObj0->getUserPointer());
    Entity *entity1 = static_cast<Entity *>(colObj1->getUserPointer());

    removeTriggerEntity(entity0, entity1);
    removeTriggerEntity(entity1, entity0);

    return btGhostPairCallback::removeOverlappingPair(proxy0, proxy1, dispatcher);
}

void World::TriggerCallback::addTriggerEntity(Entity *entityA, Entity *entityB) const {
    if (entityA != nullptr) {
        TriggerComponent *triggerComp = entityA->getComponent<TriggerComponent>();

        if (triggerComp != nullptr && entityB != nullptr) {
            triggerComp->addTriggerEntity(entityB);
        }
    }
}

void World::TriggerCallback::removeTriggerEntity(Entity *entityA, Entity *entityB) const {
    if (entityA != nullptr) {
        TriggerComponent *triggerComp = entityA->getComponent<TriggerComponent>();

        if (triggerComp != nullptr && entityB != nullptr) {
            triggerComp->removeTriggerEntity(entityB);
        }
    }
}
