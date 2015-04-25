#pragma once

class Entity;
class EntityManager;

class BombFactory {
public:
	BombFactory(EntityManager *entityManager);
	~BombFactory();

	Entity *createBomb() const;
    Entity *createBomb(float x, float y, float z) const;
    Entity *createBomb(float x, float y, float z, float vx, float vy, float vz) const;

private:
	EntityManager *entityManager;
};
