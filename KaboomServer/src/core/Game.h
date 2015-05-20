#pragma once

#include <core/EntityManager.h>
#include <util/Configuration.h>
#include <util/IdPool.h>
#include <util/Timer.h>

#include "BombFactory.h"
#include "CharacterFactory.h"
#include "PickupFactory.h"
#include "JumpPadFactory.h"
#include "../debug/DebugWorld.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/SystemManager.h"

class ConfigSettings;
class Player;

class Game {
public:
    typedef std::unordered_map<unsigned int, Player *> IdToPlayerMap;
    typedef std::unordered_map<std::string, Configuration> SpawnPointToConfigMap;

    Game(ConfigSettings *configSettings);
    ~Game();

    void update(float timeStep, int maxSubSteps);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    inline Player *getPlayerByEntityId(unsigned int id) const {
        return entityIdToPlayer.at(id);
    }

    inline SpawnPointToConfigMap &getSpawnPointConfigs() {
        return world.getSpawnPointConfigs();
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

    inline const PickupFactory &getPickupFactory() const {
        return pickupFactory;
    }

	inline const JumpPadFactory &getJumpPadFactory() const {
		return jumpPadFactory;
	}

    inline const GameServer &getGameServer() const {
        return server;
    }

    inline DebugWorld &getWorld() {
        return world;
    }

    inline std::unordered_map<std::string, Timer> &getPickupRequest() {
        return pickupSpawnRequest;
    }

    inline std::vector<std::string> &getPlayerSpawnPointList() {
        return playerSpawnPointList;
    }

	inline std::vector<std::string> &getJumpPadSpawnPointList() {
		return jumpPadSpawnPointList;
	}

private:
    EntityManager entityManager;
    SystemManager systemManager;

    CharacterFactory characterFactory;
    BombFactory bombFactory;
    PickupFactory pickupFactory;
	JumpPadFactory jumpPadFactory;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    DebugWorld world;

    std::unordered_map<std::string, Timer> pickupSpawnRequest;
    std::vector<std::string> playerSpawnPointList;
	std::vector<std::string> jumpPadSpawnPointList;

    IdPool<unsigned int> playerIdPool;
    IdToPlayerMap entityIdToPlayer;

    void loadWorld(const std::string &mapFilename, const std::string &entitiesFilename);
};
