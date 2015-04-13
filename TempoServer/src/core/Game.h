#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    void loadMap();
    void addEntity(Entity *entity);
    void update(float timestep);

private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;

    std::vector<Entity *> entites;
};
