#pragma once

#include <unordered_map>

#include <core/EntityType.h>

#include "BombData.h"

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            const EntityType &type,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float vx = 0.0f,
            float vy = 0.0f,
            float vz = 0.0f) const;

private:
    EntityManager &entityManager;
};
