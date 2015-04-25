#pragma once

#include <unordered_map>

#include "BombType.h"

class Entity;
class EntityManager;

class BombFactory {
public:
	BombFactory(EntityManager *entityManager);
	~BombFactory();

    Entity *createBomb(BombType type) const;
    Entity *createBomb(BombType type, float x, float y, float z) const;
    Entity *createBomb(BombType type, float x, float y, float z, float vx, float vy, float vz) const;

private:
    struct BombData {
        unsigned int id;
        std::string name;
        float size;
        float mass;
    };

    typedef std::unordered_map<BombType, BombData> BombDataMap;

    static BombDataMap bombs;

    static BombDataMap loadXml(const std::string &filename);

	EntityManager *entityManager;
};
