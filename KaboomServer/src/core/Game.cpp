#include "Game.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>

#include "../components/CollisionComponent.h"
#include "../components/InputComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TriggerComponent.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/FiringSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/ExplosionSystem.h"
#include "../systems/InitializationSystem.h"
#include "../systems/InputSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/PickupSystem.h"
#include "../systems/TimerSystem.h"

Game::Game(ConfigSettings *config)
        : characterFactory(entityManager),
          bombFactory(entityManager),
          pickupFactory(entityManager),
	      eventHandlerLookup(this),
	      server(config, eventHandlerLookup) {
    world.loadMap();
    systemManager.addSystem(new InitializationSystem(this));
    systemManager.addSystem(new InputSystem(this));
    systemManager.addSystem(new FiringSystem(this));
    systemManager.addSystem(new PhysicsSystem(this));
    systemManager.addSystem(new CollisionSystem(this));
    systemManager.addSystem(new TimerSystem(this));
    systemManager.addSystem(new PickupSystem(this));
    systemManager.addSystem(new ExplosionSystem(this));

	//TODO Wai Ho problems with pickup being of class bomb which causes some problems in logic commented it out for now. 
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
        Entity *player = characterFactory.createCharacter(DEFAULT_CHARACTER, 0.0f, -5.0f, 5.0f);
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

    systemManager.processSystems(this);

    server.sendGameStatePackets(getEntityManager().getEntityList());
}

EntityManager &Game::getEntityManager() {
    return entityManager;
}

const CharacterFactory &Game::getCharacterFactory() const {
    return characterFactory;
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
