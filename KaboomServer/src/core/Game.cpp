#include "Game.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "ExplosionComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

Game::Game(ConfigSettings *config)
        : config(config),
          playerFactory(entityManager),
          bombFactory(entityManager),
          inputSystem(this),
          physicsSystem(this),
          collisionSystem(this),
          explosionSystem(this),
	      eventHandlerLookup(this),
	      server(config, eventHandlerLookup) {
    world.loadMap();
}

Game::~Game() {
}

void Game::addEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.addRigidBody(physicsComp->getRigidBody());
    }

    ExplosionComponent *expComp = entity->getComponent<ExplosionComponent>();

    if (expComp != nullptr) {
        world.addCollisionObject(expComp->getGhostObject());
    }
}

void Game::removeEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.removeRigidBody(physicsComp->getRigidBody());
    }

    ExplosionComponent *expComp = entity->getComponent<ExplosionComponent>();

    if (expComp != nullptr) {
        world.removeCollisionObject(expComp->getGhostObject());
    }

    entityManager.destroyEntity(entity->getId());
}

void Game::update(float timeStep) {

	//HERE is where the client first connect to server,
    //we want to have client load the gameworld first,
    //then create the player, and send the spawn player event to client
	if (server.acceptNewClient(entityManager.generateId())) {

		//now we create a new player
        Entity *player = playerFactory.createPlayer(0, -5, 5);
        players.push_back(player);
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
    inputSystem.update(timeStep);
    physicsSystem.update(timeStep);
    collisionSystem.update(timeStep);
    explosionSystem.update(timeStep);

    for (Entity *entity : entityManager.getEntityList()) {
        PositionComponent *posCom = entity->getComponent<PositionComponent>();
        PhysicsComponent *physCom = entity->getComponent<PhysicsComponent>();

        const btTransform &worldTrans = physCom->getRigidBody()->getWorldTransform();
        const btVector3 &pos = worldTrans.getOrigin();
        
        posCom->setPosition(pos.getX(), pos.getY(), pos.getZ());

        auto *expComp = entity->getComponent<ExplosionComponent>();

        if (expComp != nullptr) {
            expComp->getGhostObject()->setWorldTransform(worldTrans);
        }
    }

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

World &Game::getWorld() {
    return world;
}
