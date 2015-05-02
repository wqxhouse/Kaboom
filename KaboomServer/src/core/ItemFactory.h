#pragma once

#include <core/EntityType.h>

class Entity;
class EntityManager;

class ItemFactory {
public:
    ItemFactory(EntityManager &entityManager);

    Entity *createItem(const EntityType &type, unsigned int amount, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
