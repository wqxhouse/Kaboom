#include "CharacterFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/BombContainerComponent.h>
#include <core/EntityManager.h>
#include <core/EquipmentComponent.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <core/PlayerStatusComponent.h>
#include <core/HealthComponent.h>

#include "BombDataLookup.h"
#include "CharacterDataLookup.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "JetpackComponent.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(
        const EntityType &characterType,
        float x,
        float y,
        float z,
        float yaw,
        float pitch) const {
    const CharacterData &characterData = CharacterDataLookup::instance[characterType];

    Entity *entity = entityManager.createEntity(characterType);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btCapsuleShape(btScalar(.25f), btScalar(.5f));

    btRigidBody *rigidBody = new btRigidBody(characterData.mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setUserPointer(entity);

    BombContainerComponent::InventoryType inventory = BombContainerComponent::InventoryType();

    for (auto kv : characterData.inventory) {
        const BombData &bombData = BombDataLookup::instance[kv.first];
        inventory[kv.first] = { kv.second, Timer(bombData.cooldown) };
    }

    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent(yaw, pitch));
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent(inventory));
    entity->attachComponent(new JetpackComponent());
	entity->attachComponent(new PlayerStatusComponent());
	entity->attachComponent(new HealthComponent(characterData.healthStartAmount, characterData.healthCap));
    entity->attachComponent(new EquipmentComponent(KABOOM_V2));

    return entity;
}
