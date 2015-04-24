#include "Game.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>

#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

Game::Game(ConfigSettings *config)
    : config(config),
    playerFactory(&entityManager),
	bombFactory(&entityManager),
    eventHandlerLookup(this),
    server(config, eventHandlerLookup) {

    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    world->setGravity(btVector3(0, 0, -1)); // TODO: Extract gravity constant
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
        Entity *player = playerFactory.createPlayer(0, 0, 5);
        players.push_back(player);
        addPhysicsEntity(player);

		//notify client, a player spawn occurs
		//server->sendPlayerSpawnEvent(player, players);
		server.sendEntitySpawnEvent(player);
		server.sendAllEntitiesSpawnEvent(player,entityManager.getEntityList());
        //first have the client first preload the information about the world
        server.sendGameStatePackets(this);
    }

    server.receive();

    // Handle game logic here

    // TODO: Extract this into MovementSystem class.
    for (Entity *entity : entityManager.getEntityList()) {
        InputComponent *inputCom = entity->getComponent<InputComponent>();
        PhysicsComponent *physCom = entity->getComponent<PhysicsComponent>();

        if (inputCom == nullptr || physCom == nullptr) {
            continue;
        }

        btRigidBody *rigidBody = physCom->getRigidBody();
        btVector3 velocity = rigidBody->getLinearVelocity();

        if (inputCom->isMovingForward()) {
            velocity.setY(1);
        } else if (inputCom->isMovingBackward()) {
            velocity.setY(-1);
        } else {
            velocity.setY(0);
        }

        if (inputCom->isMovingLeft()) {
            velocity.setX(-1);
        } else if (inputCom->isMovingRight()) {
            velocity.setX(1);
        } else {
            velocity.setX(0);
        }

        rigidBody->setLinearVelocity(velocity);
    }

	for (Entity *entity : entityManager.getEntityList()) {
		entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);
    }

    world->stepSimulation(timeStep);
    
	for (Entity *entity : entityManager.getEntityList()) {
		PositionComponent *positionCom = entity->getComponent<PositionComponent>();
		PhysicsComponent *physicsCom = entity->getComponent<PhysicsComponent>();

        const btVector3 &position = physicsCom->getRigidBody()->getWorldTransform().getOrigin();
        positionCom->setPosition(position.getX(), position.getY(), position.getZ());
    }

    server.sendGameStatePackets(this);
}

const EntityManager &Game::getEntityManager() const {
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
