#pragma once

#include <core/EntityManager.h>
#include <core/PlayerManager.h>
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
class Player;

class Game {
public:
    Game(ConfigSettings *configSettings);
    ~Game();

    void update(float timeStep, int maxSubSteps);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    
    inline PlayerManager &getPlayerManager() {
        return playerManager;
    }

    inline EntityManager &getEntityManager() {
        return entityManager;
    }

    inline const CharacterFactory &getCharacterFactory() const {
        return characterFactory;
    }

    inline const BombFactory &getBombFactory() const {
        return bombFactory;
    }

    inline const PickupFactory & getPickupFactory() const {
        return pickupFactory;
    }

    inline const GameServer &getGameServer() const {
        return server;
    }

    inline DebugWorld &getWorld() {
        return world;
    }

    inline std::unordered_map<std::string, Timer> & getPickupSpawnPointTimerMap() {
        return pickupSpawnPointTimerMap;
    }

    inline std::vector<std::string> & getPlayerSpawnPointList() {
        return playerSpawnPointList;
    }

private:
    PlayerManager playerManager;

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
