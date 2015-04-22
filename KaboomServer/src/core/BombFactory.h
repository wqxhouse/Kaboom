#pragma once

class Entity;
class EntityManager;

class BombFactory {
public:
	BombFactory(EntityManager *entityManager);
	~BombFactory();

	Entity *createBomb() const;
	Entity *createBomb(float x, float y, float z) const;

private:
	EntityManager *entityManager;
};
