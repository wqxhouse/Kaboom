#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>

#include "BombType.h"

class Entity;
class EntityManager;

class BombFactory {
public:
	BombFactory(EntityManager *entityManager);
	~BombFactory();

    Entity *createBomb(const BombType &type) const;
    Entity *createBomb(const BombType &type, float x, float y, float z) const;
    Entity *createBomb(const BombType &type, float x, float y, float z, float vx, float vy, float vz) const;

private:
    struct BombData {
        unsigned int id;
        std::string name;
        float size;
        float mass;
    };

    class BombDataLookup : public XMLLoader {
    public:
        BombDataLookup(const std::string &filename);
        const BombData &operator[](const BombType &type) const;
    protected:
        virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
    private:
        std::unordered_map<BombType, BombData> bombs;
    };

    static BombDataLookup lookup;

	EntityManager *entityManager;
};
