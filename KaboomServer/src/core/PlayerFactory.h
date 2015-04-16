#pragma once

#include <core/EntityManager.h>

#include "ServerPlayer.h"

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    ServerPlayer *createPlayer(float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
