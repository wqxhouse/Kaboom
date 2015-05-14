#pragma once

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
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, int amount, float radius, float x, float y, float z) const;
    void createBombPickup(Entity *entity) const;
};
