#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <core/EntityManager.h>

#include "PlayerFactory.h"

#include "../network/GameServer.h"
#include "../util/ConfigSettings.h"

class GameServer;

class Game {
public:
    std::vector<ServerPlayer *> players;

    Game(ConfigSettings *config);
    ~Game();

    void loadMap();
    void addEntity(Entity *entity);
    void update(float timestep);

private:
    ConfigSettings *config;
    GameServer *server;

    EntityManager entityManager;
    PlayerFactory playerFactory;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
};
