#pragma once

#include <core/EntityManager.h>
#include <util/Configuration.h>

#include "BombFactory.h"
#include "World.h"
#include "CharacterFactory.h"
#include "PickupFactory.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/SystemManager.h"

class ConfigSettings;

class Game {
public:
    Game(ConfigSettings *configSettings);
    ~Game();

    void update(float timeStep, int maxSubSteps);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    Configuration &getConfiguration();

    EntityManager &getEntityManager();
    const CharacterFactory &getCharacterFactory() const;
    const BombFactory &getBombFactory() const;
	const PickupFactory & getPickupFactory() const;

    const GameServer &getGameServer() const;

	const World & getWorld() const;

	std::unordered_map<std::string, Timer> & getPickupSpawnPointTimerMap();

private:
    Configuration config;

    EntityManager entityManager;
    SystemManager systemManager;

    CharacterFactory characterFactory;
    BombFactory bombFactory;
    PickupFactory pickupFactory;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

	std::unordered_map<std::string, Timer> pickupSpawnPointTimerMap;
	
    void stepSimulation(float timeStep, int maxSubSteps);
};
