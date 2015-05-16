#pragma once

#include <core/EntityManager.h>
#include <util/Configuration.h>
#include <util/Timer.h>

#include "BombFactory.h"
#include "CharacterFactory.h"
#include "PickupFactory.h"
#include "../debug/DebugWorld.h"
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

	DebugWorld &getWorld();

	std::unordered_map<std::string, Timer> & getPickupSpawnPointTimerMap();
	std::vector<std::string> & getPlayerSpawnPointList();

private:
    Configuration config;

    EntityManager entityManager;
    SystemManager systemManager;

    CharacterFactory characterFactory;
    BombFactory bombFactory;
    PickupFactory pickupFactory;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    DebugWorld world;

	std::unordered_map<std::string, Timer> pickupSpawnPointTimerMap;
	std::vector<std::string> playerSpawnPointList;

    void stepSimulation(float timeStep, int maxSubSteps);
};
