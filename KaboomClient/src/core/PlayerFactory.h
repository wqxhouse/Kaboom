#pragma once

#include <core/EntityManager.h>

#include "ClientPlayer.h"

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    ClientPlayer *PlayerFactory::createPlayer(unsigned int id, float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
