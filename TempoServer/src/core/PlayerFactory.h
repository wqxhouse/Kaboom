#pragma once

#include "EntityManager.h"
#include "Player.h"

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    Player *createPlayer() const;
    Player *createPlayer(float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
