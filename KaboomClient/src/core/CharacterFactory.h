#pragma once

#include <math/Quat.h>

static const int IDLE = 101;
static const int RUNNING = 102;

class Entity;
enum EntityType;
class EntityManager;
class Vec3;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(
            unsigned int id,
            EntityType characterType,
            const Vec3 &position,
            Quat rotation) const;

private:
    EntityManager &entityManager;
};
