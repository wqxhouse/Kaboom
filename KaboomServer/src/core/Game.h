#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <core/EntityManager.h>
#include <util/ConfigSettings.h>

#include "BombFactory.h"
#include "CollisionSystem.h"
#include "ExplosionSystem.h"
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

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

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
    ExplosionSystem explosionSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;
};
