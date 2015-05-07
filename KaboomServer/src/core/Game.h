#pragma once

#include <core/EntityManager.h>

#include "BombFactory.h"
#include "World.h"
#include "CharacterFactory.h"
#include "PickupFactory.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/SystemManager.h"

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
    SystemManager systemManager;

    CharacterFactory characterFactory;
    BombFactory bombFactory;
    PickupFactory pickupFactory;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    World world;

    void stepSimulation(float timeStep, int maxSubSteps);
};
