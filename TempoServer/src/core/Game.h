#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    void loadMap();
	void addPlayer(Player *player);
    void update(float timestep);

private:
	typedef std::vector<Player *> PlayerList;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;

	PlayerList players;
};
