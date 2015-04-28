#include "BombFactory.h"

#include <stdexcept>

#include <btBulletDynamicsCommon.h>

#include <osgDB/XmlParser>

#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <util/XMLLoader.h>

#include "PhysicsComponent.h"

BombFactory::BombDataLookup BombFactory::lookup("data/bombs.xml");

BombFactory::BombFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

BombFactory::~BombFactory() {
}

Entity *BombFactory::createBomb(const BombType &type) const {
	return createBomb(type, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(const BombType &type, float x, float y, float z) const {
    return createBomb(type, x, y, z, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(const BombType &type, float x, float y, float z, float vx, float vy, float vz) const {
    const BombData &data = lookup[type];

    Entity *entity = entityManager.createEntity();

    btTransform startTrans = btTransform::getIdentity();
    startTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(startTrans);
    btCollisionShape *collisionShape = new btSphereShape(data.size);
    collisionShape->setUserPointer(entity);

    btRigidBody *rigidBody = new btRigidBody(data.mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setLinearVelocity(btVector3(vx, vy, vz));

    entity->attachComponent(new CharacteristicComponent(BOMB, 0, 0));
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));

    return entity;
}

BombFactory::BombDataLookup::BombDataLookup(const std::string &filename) {
    loadXMLFile(filename);
}

void BombFactory::BombDataLookup::loadXMLNode(osgDB::XmlNode *xmlRoot) {
    if (xmlRoot->type == osgDB::XmlNode::ROOT) {
        for (auto child : xmlRoot->children) {
            if (child->name == "bombs") {
                return loadXMLNode(child);
            }
        }

        return;
    }

    std::unordered_map<unsigned int, BombType> types = {
        { 0, BombType::BOM_BOM }
    };

    for (auto bombNode : xmlRoot->children) {
        if (bombNode->name != "bomb") {
            continue;
        }

        BombData data;
        memset(&data, 0, sizeof(BombData));

        for (auto dataNode : bombNode->children) {
            if (dataNode->name == "id") {
                loadUint(dataNode, data.id);
            } else if (dataNode->name == "name") {
                loadString(dataNode, data.name);
            } else if (dataNode->name == "size") {
                loadFloat(dataNode, data.size);
            } else if (dataNode->name == "mass") {
                loadFloat(dataNode, data.mass);
            }
        }

        bombs[types[data.id]] = data;
    }
}

const BombFactory::BombData &BombFactory::BombDataLookup::operator[](const BombType &type) const {
    return bombs.at(type);
}
