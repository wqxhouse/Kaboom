#include "PickupFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/Entity.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/WeaponPickupComponent.h>

#include "TriggerComponent.h"

PickupFactory::PickupFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *PickupFactory::createPickup(
        const EntityType &type,
        unsigned int amount,
        float x,
        float y,
        float z) const {
    const float PICKUP_RADIUS = 1.0f;

    Entity *entity = entityManager.createEntity(CAT_PICKUP);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(PICKUP_RADIUS));
    ghostObject->setUserPointer(entity);
    ghostObject->setWorldTransform(worldTrans);

    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new TriggerComponent(ghostObject));
	entity->attachComponent(new WeaponPickupComponent(type, 5)); //todo this is hard coded right now, we might want to randomize

    return entity;
}
