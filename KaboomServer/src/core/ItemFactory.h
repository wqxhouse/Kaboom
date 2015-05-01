#pragma once

#include <core/BombType.h>

class Entity;
class EntityManager;

class ItemFactory {
public:
    ItemFactory(EntityManager &entityManager);

    Entity *createItem(const BombType &type, unsigned int amount, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
