#pragma once

#include <core/EntityManager.h>

#include "BombFactory.h"
#include "FiringSystem.h"
#include "CollisionSystem.h"
#include "ExplosionSystem.h"
#include "InitializationSystem.h"
#include "InputSystem.h"
#include "PickupFactory.h"
#include "PickupSystem.h"
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
    PickupFactory pickupFactory;

    InitializationSystem initSystem;
    InputSystem inputSystem;
    PickupSystem pickupSystem;
	FiringSystem firingSystem;
    CollisionSystem collisionSystem;
    ExplosionSystem explosionSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

    void stepSimulation(float timeStep, int maxSubSteps);
};
