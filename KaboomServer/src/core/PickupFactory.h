#pragma once

#include <math/Vec3.h>

class Entity;
enum EntityType;
class EntityManager;

class PickupFactory {
public:
    PickupFactory(EntityManager &entityManager);

    Entity *createPickup(
            EntityType type,
            int amount = 1,
			float radius = 1.0,
            const Vec3 &position = Vec3()) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, int amount, float radius, const Vec3 &position) const;
    void createBombPickup(Entity *entity) const;
};
