#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(
            unsigned int id,
            EntityType characterType,
            float x,
            float y,
            float z,
            Quat rotation) const;

private:
    EntityManager &entityManager;
};
