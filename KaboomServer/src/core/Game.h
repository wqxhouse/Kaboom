#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <ConfigSettings.h>
#include <core/EntityManager.h>

#include "PlayerFactory.h"

#include "../network/GameServer.h"

class GameServer;

class Game {
public:
    std::vector<Entity *> players;

    Game(ConfigSettings *config);
    ~Game();

    void loadMap();
    void update(float timestep);

    void addPhysicsEntity(Entity *entity);

    const EntityManager &getEntityManager() const;

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
