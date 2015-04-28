#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <core/EntityManager.h>
#include <util/ConfigSettings.h>

#include "BombFactory.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
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
    void addBombExplosion(Entity *bomb, float x, float y, float z);

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
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

    struct BombExplosion {
        Entity *bomb;
        float x;
        float y;
        float z;
    };

    std::vector<BombExplosion> bombExplosions;
};
