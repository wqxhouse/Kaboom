#include "Game.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "CollisionComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "TriggerComponent.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

Game::Game(ConfigSettings *config)
        : playerFactory(entityManager),
          bombFactory(entityManager),
          pickupFactory(entityManager),
          initSystem(this),
          inputSystem(this),
          pickupSystem(this),
		  firingSystem(this),
          collisionSystem(this),
          explosionSystem(this),
	      eventHandlerLookup(this),
	      server(config, eventHandlerLookup) {
    world.loadMap();
    addEntity(pickupFactory.createPickup(KABOOM_V2, 5)); // Spawn five Kaboom 2.0 at origin
}

Game::~Game() {
}

void Game::addEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.addRigidBody(physicsComp->getRigidBody());
    }

    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        world.addTrigger(triggerComp->getGhostObject());
    }
}

void Game::removeEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.removeRigidBody(physicsComp->getRigidBody());
    }

    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        world.removeTrigger(triggerComp->getGhostObject());
    }

    entityManager.destroyEntity(entity->getId());
}

void Game::update(float timeStep, int maxSubSteps) {

	//HERE is where the client first connect to server,
    //we want to have client load the gameworld first,
    //then create the player, and send the spawn player event to client
	if (server.acceptNewClient(entityManager.generateId())) {

		//now we create a new player
        Entity *player = playerFactory.createPlayer(0, -5, 5);
        addEntity(player);

        //first notify the new client what entityId it should keep track of
        server.sendAssignEvent(player->getId());

        //second send the new client about all the exisiting entities
        server.sendInitializeEvent(player, entityManager.getEntityList());

        //then send the new spawn player entity to all the clients
        server.sendSpawnEvent(player);

        //lastly send the game state for each entity
        server.sendGameStatePackets(getEntityManager().getEntityList());
    }

    server.receive(this);

    // Handle game logic here
    initSystem.update(timeStep);
    inputSystem.update(timeStep);
	firingSystem.update(timeStep);

    stepSimulation(timeStep, maxSubSteps);

    collisionSystem.update(timeStep);
    pickupSystem.update(timeStep);
    explosionSystem.update(timeStep);

    server.sendGameStatePackets(getEntityManager().getEntityList());
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

void Game::stepSimulation(float timeStep, int maxSubSteps) {
    world.stepSimulation(timeStep, maxSubSteps);

    // Update position component and rotation component based on simulation result
    auto entities = getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        PhysicsComponent *physComp = entity->getComponent<PhysicsComponent>();

        if (physComp == nullptr) {
            continue;
        }

        const btTransform &worldTrans = physComp->getRigidBody()->getWorldTransform();

        PositionComponent *posComp = entity->getComponent<PositionComponent>();

        if (posComp != nullptr) {
            const btVector3 &pos = worldTrans.getOrigin();
            posComp->setPosition(pos.getX(), pos.getY(), pos.getZ());
        }

        RotationComponent *rotComp = entity->getComponent<RotationComponent>();

        if (rotComp != nullptr) {
            btQuaternion rot = worldTrans.getRotation();
            // TODO: Set rotComp
        }
    }
}
