#pragma once

#include <unordered_map>

#include <core/EntityType.h>
#include <util/XMLLoader.h>

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            const EntityType &type,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float vx = 0.0f,
            float vy = 0.0f,
            float vz = 0.0f) const;

private:
    class BombDataLookup;

    struct BombData {
        unsigned int id;
        std::string name;
        float size;
        float mass;
        float explosionRadius;
        unsigned int cooldown;
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
