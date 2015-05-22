#pragma once

#include <core/EntityManager.h>
#include <core/DeathmatchMode.h>
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

    const clock_t FPS = 60;
    const int MAX_SUB_STEPS = 30;

    Game(ConfigSettings *configSettings);
    ~Game();

    void run();
    void update();

    void addPlayer(Player *player);
    void removePlayer(Player *player);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    Vec3 getPlayerSpawnPoint();

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

    inline const DeathmatchMode &getGameMode() const {
        return gameMode;
    }

    inline DebugWorld &getWorld() {
        return world;
    }

    inline const IdToPlayerMap &getPlayers() const {
        return players;
    }

    inline std::unordered_map<std::string, Timer> &getPickupRequest() {
        return pickupSpawnRequest;
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

    DeathmatchMode gameMode;
    DebugWorld world;

    IdToPlayerMap players;

    std::unordered_map<std::string, Configuration> gameModeConfigs;
    std::unordered_map<std::string, Timer> pickupSpawnRequest;
    std::vector<std::string> playerSpawnPointList;
    std::vector<std::string> jumpPadSpawnPointList;

    void addPlayerToWorld(Player *player);
};
