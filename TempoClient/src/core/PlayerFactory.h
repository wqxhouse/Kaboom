#pragma once

#include <core/EntityManager.h>
#include "ClientPlayer.h"

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    ClientPlayer *createPlayer() const;
    ClientPlayer *createPlayer(float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
