#include "Game.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

Game::Game(ConfigSettings *config)
	: config(config),
	playerFactory(&entityManager),
	bombFactory(&entityManager),
	inputSystem(this),
	eventHandlerLookup(this),
	server(config, eventHandlerLookup) 
{
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    world->setGravity(btVector3(0, 0, -4.0)); // TODO: Extract gravity constant
}

Game::~Game() {
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
}

void Game::loadMap() {
    // TODO: Handle memory leak
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1)));

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    world->addRigidBody(groundRigidBody);
}

void Game::addPhysicsEntity(Entity *entity) {
    PhysicsComponent *physicsCom = entity->getComponent<PhysicsComponent>();

    if (physicsCom != nullptr) {
        world->addRigidBody(physicsCom->getRigidBody());
    }
}

void Game::update(float timeStep) {

	//HERE is where the client first connect to server,
    //we want to have client load the gameworld first,
    //then create the player, and send the spawn player event to client
	if (server.acceptNewClient(entityManager.getNextId())) {

		//now we create a new player
        Entity *player = playerFactory.createPlayer(0, -5, 5);
        players.push_back(player);
        addPhysicsEntity(player);

        //first notify the new client what entityId it should keep track of
        server.sendAssignPlayerEntity(player->getId());

        //second send the new client about all the exisiting entities
        server.sendAllEntitiesSpawnEvent(player, entityManager.getEntityList());

        //then send the new spawn player entity to all the clients
        server.sendEntitySpawnEvent(player);

        //lastly send the game state for each entity
        server.sendGameStatePackets(this);
    }

    server.receive(this);

    // Handle game logic here
    inputSystem.update(timeStep);

	for (Entity *entity : entityManager.getEntityList())
	{
		entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);
	}

    world->stepSimulation(timeStep);

    for (Entity *entity : entityManager.getEntityList()) {
        PositionComponent *posCom = entity->getComponent<PositionComponent>();
        PhysicsComponent *physCom = entity->getComponent<PhysicsComponent>();
        RotationComponent *rotCom = entity->getComponent<RotationComponent>();

        const btTransform &worldTrans = physCom->getRigidBody()->getWorldTransform();
        const btVector3 &pos = worldTrans.getOrigin();
        
        posCom->setPosition(pos.getX(), pos.getY(), pos.getZ());
        // TODO: set rotation
    }

    server.sendGameStatePackets(this);
}

EntityManager &Game::getEntityManager() {
    return entityManager;
}

const PlayerFactory &Game::getPlayerFactory() const {
    return playerFactory;
}

const BombFactory &Game::getBombFactory() const {
    return bombFactory;
}

const GameServer &Game::getGameServer() const {
    return server;
}
btDiscreteDynamicsWorld *Game::getBtDiscreteDynamicsWorld(){
	return world;
}
