#include "BombFactory.h"

#include <stdexcept>

#include <btBulletDynamicsCommon.h>

#include <pugixml.hpp>

#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "PhysicsComponent.h"

BombFactory::BombDataMap BombFactory::bombs = BombFactory::loadXml("data/bombs.xml");

BombFactory::BombDataMap BombFactory::loadXml(const std::string &filename) {
    static std::unordered_map<unsigned int, BombType> types = {
        { 0, BombType::BOM_BOM }
    };

    BombDataMap bombs;

    pugi::xml_document doc;

    if (!doc.load_file(filename.c_str())) {
        throw std::runtime_error("Unable to read bomb definitions.");
    }

    pugi::xpath_node_set xpathNodes = doc.select_nodes("/bombs/bomb");

    for (auto xpathNode : xpathNodes) {
        auto node = xpathNode.node();

        BombData data;
        data.id = node.attribute("id").as_uint();
        data.name = node.attribute("name").as_string();
        data.size = node.attribute("size").as_float();
        data.mass = node.attribute("mass").as_float();

        bombs[types[data.id]] = data;
    }

    return bombs;
}

BombFactory::BombFactory(EntityManager *entityManager)
        : entityManager(entityManager) {
}

BombFactory::~BombFactory() {
}

Entity *BombFactory::createBomb(BombType type) const {
	return createBomb(type, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(BombType type, float x, float y, float z) const {
    return createBomb(type, x, y, z, 0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(BombType type, float x, float y, float z, float vx, float vy, float vz) const {
    const BombData &data = bombs[type];

    btTransform startTrans = btTransform::getIdentity();
    startTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(startTrans);
    btCollisionShape *collisionShape = new btSphereShape(data.size);

    btRigidBody *rigidBody = new btRigidBody(data.mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setLinearVelocity(btVector3(vx, vy, vz));

    Entity *entity = entityManager->createEntity();
    entity->attachComponent(new CharacteristicComponent(BOMB, 0, 0));
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));

    return entity;
}
