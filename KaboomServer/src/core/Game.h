#pragma once

#include <core/EntityManager.h>

#include "BombFactory.h"
#include "CollisionSystem.h"
#include "ExplosionSystem.h"
#include "InitializationSystem.h"
#include "InputSystem.h"
#include "ItemFactory.h"
#include "ItemSystem.h"
#include "PlayerFactory.h"
#include "World.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

class ConfigSettings;

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void update(float timeStep, int maxSubSteps);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    EntityManager &getEntityManager();
    const PlayerFactory &getPlayerFactory() const;
    const BombFactory &getBombFactory() const;

    const GameServer &getGameServer() const;

private:
    EntityManager entityManager;
    PlayerFactory playerFactory;
    BombFactory bombFactory;
    ItemFactory itemFactory;

    InitializationSystem initSystem;
    InputSystem inputSystem;
    ItemSystem itemSystem;
    CollisionSystem collisionSystem;
    ExplosionSystem explosionSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

    void stepSimulation(float timeStep, int maxSubSteps);
};
