#include "PickupFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>
#include <core/EntityManager.h>

#include "../components/TriggerComponent.h"

PickupFactory::PickupFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *PickupFactory::createPickup(
        const EntityType &pickupType,
        unsigned int amount,
        float x,
        float y,
        float z) const {
    const float PICKUP_RADIUS = 1.0f;

    Entity *entity = entityManager.createEntity(pickupType);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(PICKUP_RADIUS));
    ghostObject->setUserPointer(entity);
    ghostObject->setWorldTransform(worldTrans);

    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new TriggerComponent(ghostObject));
    entity->attachComponent(new WeaponPickupComponent(pickupType, amount));

    return entity;
}
