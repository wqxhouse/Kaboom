#pragma once

#include <unordered_map>

#include <core/BombType.h>
#include <util/XMLLoader.h>

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);
    ~BombFactory();

    Entity *createBomb(const BombType &type) const;
    Entity *createBomb(const BombType &type, float x, float y, float z) const;
    Entity *createBomb(const BombType &type, float x, float y, float z, float vx, float vy, float vz) const;

private:
    class BombDataLookup;

    struct BombData {
        unsigned int id;
        std::string name;
        float size;
        float mass;
        float explosionRadius;
    };

    static BombDataLookup lookup;

    EntityManager &entityManager;
};

class BombFactory::BombDataLookup : public XMLLoader {
public:
    BombDataLookup(const std::string &filename);

    const BombData &operator[](const BombType &type) const;

private:
    std::unordered_map<BombType, BombData> bombs;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
};
