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
	      playerFactory(entityManager),
	      bombFactory(entityManager),
	      inputSystem(this),
	      eventHandlerLookup(this),
	      server(config, eventHandlerLookup) {
    world.loadMap();
}

Game::~Game() {
}

void Game::addEntityToWorld(Entity *entity) {
    PhysicsComponent *physicsCom = entity->getComponent<PhysicsComponent>();

    if (physicsCom != nullptr) {
        world.addRigidBody(physicsCom->getRigidBody());
    }
}

void Game::update(float timeStep) {

	//HERE is where the client first connect to server,
    //we want to have client load the gameworld first,
    //then create the player, and send the spawn player event to client
	if (server.acceptNewClient(entityManager.generateId())) {

		//now we create a new player
        Entity *player = playerFactory.createPlayer(0, -5, 5);
        players.push_back(player);
        addEntityToWorld(player);

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

	for (Entity *entity : entityManager.getEntityList())
	{
		entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);
	}

    world.stepSimulation(timeStep);

    for (Entity *entity : entityManager.getEntityList()) {
        PositionComponent *posCom = entity->getComponent<PositionComponent>();
        PhysicsComponent *physCom = entity->getComponent<PhysicsComponent>();

        const btTransform &worldTrans = physCom->getRigidBody()->getWorldTransform();
        const btVector3 &pos = worldTrans.getOrigin();
        
        posCom->setPosition(pos.getX(), pos.getY(), pos.getZ());
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
