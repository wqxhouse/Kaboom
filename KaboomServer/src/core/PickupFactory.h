#pragma once

#include <math/Vec3.h>

class Entity;
class EntityManager;
enum EntityType;

class PickupFactory {
public:
    PickupFactory(EntityManager &entityManager);

    Entity *createPickup(
            EntityType type,
            const Vec3 &position,
            int amount = 1,
			float radius = 1.0) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, const Vec3 &position, int amount, float radius) const;
    void createBombPickup(Entity *entity) const;
};
