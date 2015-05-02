#pragma once

#include <unordered_map>

#include <util/XMLLoader.h>
#include <core/EntityType.h>

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);
    ~BombFactory();

    Entity *createBomb(const EntityType &type) const;
    Entity *createBomb(const EntityType &type, float x, float y, float z) const;
    Entity *createBomb(const EntityType &type, float x, float y, float z, float vx, float vy, float vz) const;

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

    const BombData &operator[](const EntityType &type) const;

private:
    std::unordered_map<EntityType, BombData> bombs;

    virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);
};
