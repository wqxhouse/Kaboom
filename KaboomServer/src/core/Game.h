#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>

#include "../network/GameServer.h"
#include "../util/ConfigSettings.h"

class Game {
public:
    Game(ConfigSettings *config);
    ~Game();

    void loadMap();
    void addEntity(Entity *entity);
    void update(float timestep);

private:
    ConfigSettings *config;
    GameServer *server;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;

    std::vector<Entity *> entites;
};
