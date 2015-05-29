#include "PickupFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>
#include <core/EntityManager.h>

#include "../components/RespawnComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/TriggerComponent.h"
#include "../messaging/BombPickupMessageHandler.h"
#include "../messaging/MessageHandlerChain.h"

PickupFactory::PickupFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *PickupFactory::createPickup(
        EntityType type,
        const Vec3 &position,
        int amount,
		float radius) const {

    Entity *entity = entityManager.createEntity(type);

    createBase(entity, position, amount, radius);

    if ((type & CAT_MASK) == CAT_BOMB) {
        createBombPickup(entity);
    }

    return entity;
}

void PickupFactory::createBase(Entity *entity, const Vec3 &position, int amount, float radius) const {
    const EntityType type = entity->getType();

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(position.x, position.y, position.z));

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(radius));
    ghostObject->setUserPointer(entity);
    ghostObject->setWorldTransform(worldTrans);

    MessageHandlerChain *chain = new MessageHandlerChain();

    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new TriggerComponent(ghostObject));
    entity->attachComponent(new WeaponPickupComponent(type, amount));
    entity->attachComponent(new MessageHandlerComponent(chain));
}

void PickupFactory::createBombPickup(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static BombPickupMessageHandler bombPickupHandler;
    chain->addHandler(&bombPickupHandler);
}
