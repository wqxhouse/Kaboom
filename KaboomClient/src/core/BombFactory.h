#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            unsigned int id,
            EntityType bombType,
            float x,
            float y,
            float z,
            Quat rotation) const;

private:
    EntityManager &entityManager;
};
