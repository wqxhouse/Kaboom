#include "CharacterFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/BombContainerComponent.h>
#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/HealthComponent.h>
#include <core/EntityManager.h>

#include "BombDataLookup.h"
#include "CharacterData.h"
#include "CharacterDataLookup.h"
#include "../components/InputComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/JetpackComponent.h"

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

    btCollisionShape *collisionShape = new btCapsuleShapeZ(btScalar(characterData.collisionRadius), btScalar(characterData.collisionHeight)); //TODO we need to load this from character.xml

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
