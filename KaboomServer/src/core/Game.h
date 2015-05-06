#pragma once

#include <core/EntityManager.h>

#include "BombFactory.h"
#include "World.h"
#include "CharacterFactory.h"
#include "PickupFactory.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/FiringSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/ExplosionSystem.h"
#include "../systems/InitializationSystem.h"
#include "../systems/InputSystem.h"
#include "../systems/PickupSystem.h"
#include "../systems/TimerSystem.h"

class ConfigSettings;

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void update(float timeStep, int maxSubSteps);

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    EntityManager &getEntityManager();
    const CharacterFactory &getCharacterFactory() const;
    const BombFactory &getBombFactory() const;

    const GameServer &getGameServer() const;

private:
    EntityManager entityManager;
    CharacterFactory characterFactory;
    BombFactory bombFactory;
    PickupFactory pickupFactory;

    InitializationSystem initSystem;
    InputSystem inputSystem;
    PickupSystem pickupSystem;
	FiringSystem firingSystem;
    CollisionSystem collisionSystem;
    TimerSystem timerSystem;
    ExplosionSystem explosionSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

    void stepSimulation(float timeStep, int maxSubSteps);
};
