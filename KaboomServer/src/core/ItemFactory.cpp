#include "ItemFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/CharacteristicComponent.h>
#include <core/Entity.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>

#include "TriggerComponent.h"

ItemFactory::ItemFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *ItemFactory::createBomb(const BombType &type, float x, float y, float z) const {
    const float ITEM_RADIUS = 1.0f;

    Entity *entity = entityManager.createEntity();

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(ITEM_RADIUS));
    ghostObject->setUserPointer(entity);
    ghostObject->setWorldTransform(worldTrans);

    entity->attachComponent(new CharacteristicComponent(EntityType::ITEM, 0, 0));
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new TriggerComponent(ghostObject));

    return entity;
}
