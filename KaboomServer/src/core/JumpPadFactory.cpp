#include "JumpPadFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>

#include <core/Entity.h>
#include <core/EntityManager.h>

#include "../components/RespawnComponent.h"
#include "../components/JumpPadComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/TriggerComponent.h"
#include "../messaging/JumpPadMessageHandler.h"
#include "../messaging/MessageHandlerChain.h"

JumpPadFactory::JumpPadFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *JumpPadFactory::createJumpPad(
		EntityType type,
		const Vec3 &boxSize,
		const Vec3 &position,
		const Vec3 &launchSpeedVec,
		int launchDuration) const {

    Entity *entity = entityManager.createEntity(type);

	createBase(entity, boxSize, position, launchSpeedVec, launchDuration);

    if ((type & CAT_MASK) == CAT_JUMPPAD) {
        createJumpPad(entity);
    }

    return entity;
}

void JumpPadFactory::createBase(Entity *entity, const Vec3 &boxSize, const Vec3 &position, const Vec3 &launchSpeedVec, int launchDuration) const {
    const EntityType type = entity->getType();

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(position.x, position.y, position.z));

    btGhostObject *ghostObject = new btGhostObject();
	ghostObject->setCollisionShape(new btBoxShape(btVector3(boxSize.x, boxSize.y, boxSize.z)));
    ghostObject->setUserPointer(entity);
    ghostObject->setWorldTransform(worldTrans);

    MessageHandlerChain *chain = new MessageHandlerChain();

    entity->attachComponent(new PositionComponent(position));
	entity->attachComponent(new RotationComponent());
    entity->attachComponent(new TriggerComponent(ghostObject));
	entity->attachComponent(new JumpPadComponent(launchSpeedVec.x, launchSpeedVec.y, launchSpeedVec.z, launchDuration));
    entity->attachComponent(new MessageHandlerComponent(chain));
}

void JumpPadFactory::createJumpPad(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

	static JumpPadMessageHandler jumpPadMessageHandler;
	chain->addHandler(&jumpPadMessageHandler);
}
