#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "BombFactory.h"
#include "InputSystem.h"
#include "PlayerFactory.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"

class Game {
public:
    std::vector<Entity *> players;
	std::vector<Entity *> bombs;
    Game(ConfigSettings *config);
    ~Game();

    void loadMap();
    void update(float timestep);

    void addPhysicsEntity(Entity *entity);

    EntityManager &getEntityManager();
	btDiscreteDynamicsWorld *getBtDiscreteDynamicsWorld();
    const PlayerFactory &getPlayerFactory() const;
    const BombFactory &getBombFactory() const;
    const GameServer &getGameServer() const;

private:
    ConfigSettings *config;

    EntityManager entityManager;
    PlayerFactory playerFactory;
    BombFactory bombFactory;

    InputSystem inputSystem;

    ServerEventHandlerLookup eventHandlerLookup;
    GameServer server;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
};
