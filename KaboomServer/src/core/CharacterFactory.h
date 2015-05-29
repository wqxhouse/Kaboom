#pragma once

#include <math/Quat.h>

class Entity;
class EntityManager;
enum EntityType;
class Vec3;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(
            EntityType characterType,
            const Vec3 &position,
            const Quat &rotation = Quat()) const;

	void resetCharacter(
		    Entity* entity, 
			const Vec3 &position, 
			const Quat &rotation = Quat()) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, const Vec3 &position, const Quat &rotation) const;
    void createDefaultCharacter(Entity *entity) const;
	
};
