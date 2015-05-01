#pragma once

#include <core/BombType.h>

class Entity;
class EntityManager;

class ItemFactory {
public:
    ItemFactory(EntityManager &entityManager);

    Entity *createBomb(const BombType &type, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
