#pragma once

class Entity;
enum EntityType;
class EntityManager;

class PickupFactory {
public:
    PickupFactory(EntityManager &entityManager);

    Entity *createPickup(
            const EntityType &type,
            unsigned int amount = 1,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f) const;

private:
    EntityManager &entityManager;
};
