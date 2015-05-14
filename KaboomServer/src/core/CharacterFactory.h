#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(
            const EntityType &characterType,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            Quat rotation = Quat()) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, float x, float y, float z, Quat rotation) const;
    void createDefaultCharacter(Entity *entity) const;
};
