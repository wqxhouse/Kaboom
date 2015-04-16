#pragma once

#include <btBulletDynamicsCommon.h>

#include <core/Player.h>

class ServerPlayer : public Player {
public:
    ServerPlayer(unsigned int id);
    ~ServerPlayer();

    btRigidBody *getRigidBody() const;

    float getVelocityX() const;
    float getVelocityY() const;
    float getVelocityZ() const;

    void setVelocityX(float x);
    void setVelocityY(float y);
    void setVelocityZ(float z);
};
