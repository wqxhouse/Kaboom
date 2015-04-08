#pragma once

#include <btBulletDynamicsCommon.h>

#include "PlayerData.h"

class Player {
public:
	static btCollisionShape *collisionShape;

	btScalar mass;
	btMotionState *motionState;

    Player(PlayerData *data);
	~Player();

	void update(btScalar timeStep);

private:
    PlayerData *data;
};
