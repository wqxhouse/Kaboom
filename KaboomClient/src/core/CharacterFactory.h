#pragma once

class Entity;
class EntityManager;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(unsigned int id, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
