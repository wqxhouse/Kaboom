#pragma once

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
            float yaw,
            float pitch) const;

private:
    EntityManager &entityManager;
};
