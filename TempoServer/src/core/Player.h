#pragma once

#include <btBulletDynamicsCommon.h>

#include "PlayerData.h"

class Player {
public:
    static btCollisionShape *collisionShape;
    static const btScalar mass;

    btMotionState *motionState;
    btRigidBody *rigidBody;

    Player(PlayerData *data);
    ~Player();

    void update(btScalar timeStep);

private:
    PlayerData *data;
};
