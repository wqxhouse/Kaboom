#pragma once

class Entity;
class EntityManager;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(float x = 0.0f, float y = 0.0f, float z = 0.0f) const;

private:
    EntityManager &entityManager;
};
