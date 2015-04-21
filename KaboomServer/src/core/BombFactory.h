#pragma once

#include <core/EntityManager.h>

class BombFactory {
public:
	BombFactory(EntityManager *entityManager);
	~BombFactory();

	Entity *createBomb() const;
	Entity *createBomb(float x, float y, float z) const;

private:
	EntityManager *entityManager;
};
