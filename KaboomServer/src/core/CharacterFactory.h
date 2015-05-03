#pragma once

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
            float yaw = 0.0f,
            float pitch = 0.0f) const;

private:
    EntityManager &entityManager;
};
