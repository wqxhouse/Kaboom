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
#include "../systems/SpawnSystem.h"

Game::Game(ConfigSettings *configSettings)
        : characterFactory(entityManager),
          bombFactory(entityManager),
          pickupFactory(entityManager),
	      eventHandlerLookup(this),
          server(configSettings, eventHandlerLookup), 
		  world(configSettings){

    //world.loadMap();
	std::string str_mediaPath = "";
	std::string str_world_xml = "";

	configSettings->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
	configSettings->getValue(ConfigSettings::str_world_xml, str_world_xml);

	str_world_xml = str_mediaPath + str_world_xml;

	std::cout << str_world_xml << std::endl;
	world.loadMapFromXML(str_world_xml);


    systemManager.addSystem(new InitializationSystem(this));
	systemManager.addSystem(new SpawnSystem(this));
    systemManager.addSystem(new InputSystem(this));
    systemManager.addSystem(new FiringSystem(this));
    systemManager.addSystem(new PhysicsSystem(this, world));
    systemManager.addSystem(new CollisionSystem(this));
    systemManager.addSystem(new TimerSystem(this));
    systemManager.addSystem(new PickupSystem(this));
    systemManager.addSystem(new ExplosionSystem(this));

	//TODO Wai Ho problems with pickup being of class bomb which causes some problems in logic commented it out for now. 
   // addEntity(pickupFactory.createPickup(KABOOM_V2, 5, 1 ,5, -5, 3)); // Spawn five Kaboom 2.0 at origin
}

Game::~Game() {
}

void Game::addEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.addRigidBodyAndConvertToOSG(physicsComp->getRigidBody());
    }

    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        world.addTrigger(triggerComp->getGhostObject());
    }

	server.sendSpawnEvent(entity);
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

	server.sendDestroyEvent(entity);
    entityManager.destroyEntity(entity->getId());
}

void Game::update(float timeStep, int maxSubSteps) {

	//HERE is where the client first connect to server,
    //we want to have client load the gameworld first,
    //then create the player, and send the spawn player event to client
	if (server.acceptNewClient(entityManager.generateId())) {

		//now we create a new player
        Entity *player = characterFactory.createCharacter(DEFAULT_CHARACTER, 0.0f, -5.0f, 5.0f);
        
		//first notify the new client what entityId it should keep track of
		server.sendAssignEvent(player->getId());

		//send the new spawn player entity to all the clients
		addEntity(player);

        //second send the new client about all the exisiting entities
        server.sendInitializeEvent(player, entityManager.getEntityList());

        //lastly send the game state for each entity
        server.sendGameStatePackets(getEntityManager().getEntityList());
    }

    server.receive(this);

    systemManager.processSystems(this);

    server.sendGameStatePackets(getEntityManager().getEntityList());

	//TODO put an on/off switch here
	world.renderDebugFrame();
}

Configuration &Game::getConfiguration() {
    return config;
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

const PickupFactory &Game::getPickupFactory() const {
	return pickupFactory;
}

const GameServer &Game::getGameServer() const {
    return server;
}

const World &Game::getWorld() const{
	return world;
}

std::unordered_map<std::string, Timer> & Game::getPickupSpawnPointTimerMap() {
	return pickupSpawnPointTimerMap;
}