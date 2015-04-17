#pragma once

#include <core/EntityManager.h>

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    Entity *createPlayer(unsigned int id, float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
