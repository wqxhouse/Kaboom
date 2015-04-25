#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "BombFactory.h"
#include "InputSystem.h"
#include "PlayerFactory.h"
#include "World.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

class Game {
public:
    std::vector<Entity *> players;
	std::vector<Entity *> bombs;

    Game(ConfigSettings *config);
    ~Game();

    void update(float timestep);

    void addEntityToWorld(Entity *entity);

    EntityManager &getEntityManager();
    const PlayerFactory &getPlayerFactory() const;
    const BombFactory &getBombFactory() const;

    const GameServer &getGameServer() const;

    World &getWorld();

private:
    ConfigSettings *config;

    EntityManager entityManager;
    PlayerFactory playerFactory;
    BombFactory bombFactory;

    InputSystem inputSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;
};
