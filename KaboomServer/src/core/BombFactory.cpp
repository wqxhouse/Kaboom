#include "BombFactory.h"

#include <stdexcept>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <osgDB/XmlParser>

#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <util/XMLLoader.h>

#include "CollisionComponent.h"
#include "ExplosionComponent.h"
#include "PhysicsComponent.h"
#include "TriggerComponent.h"

BombFactory::BombDataLookup BombFactory::lookup("data/bombs.xml");

BombFactory::BombFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

BombFactory::~BombFactory() {
}

Entity *BombFactory::createBomb(const EntityType &type) const {
	return createBomb(type, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(const EntityType &type, float x, float y, float z) const {
    return createBomb(type, x, y, z, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(const EntityType &type, float x, float y, float z, float vx, float vy, float vz) const {
    const BombData &data = lookup[type];

    Entity *entity = entityManager.createEntity(type);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btSphereShape(data.size);

    btRigidBody *rigidBody = new btRigidBody(data.mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setLinearVelocity(btVector3(vx, vy, vz));
    rigidBody->setUserPointer(entity);

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(data.explosionRadius));
    ghostObject->setWorldTransform(worldTrans);
    ghostObject->setUserPointer(entity);

    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new TriggerComponent(ghostObject));
    entity->attachComponent(new CollisionComponent());

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

    std::unordered_map<unsigned int, EntityType> types = {
        { KABOOM_V2, KABOOM_V2 },
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
            } else if (dataNode->name == "explosion-radius") {
                loadFloat(dataNode, data.explosionRadius);
            } else if (dataNode->name == "cooldown") {
                loadUint(dataNode, data.cooldown);
            }
        }

        bombs[types[data.id]] = data;
    }
}

const BombFactory::BombData &BombFactory::BombDataLookup::operator[](const EntityType &type) const {
    return bombs.at(type);
}
