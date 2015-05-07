#include "CharacterFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/BombContainerComponent.h>
#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/HealthComponent.h>
#include <core/EntityManager.h>

#include "common.h"
#include "EntityConfigLookup.h"
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
    const Configuration &charConfig = EntityConfigLookup::instance()[characterType];

    Entity *entity = entityManager.createEntity(characterType);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);

    btCollisionShape *collisionShape = new btCapsuleShapeZ(btScalar(charConfig.getFloat("collision-radius")), btScalar(charConfig.getFloat("collision-height")));

    btRigidBody *rigidBody = new btRigidBody(charConfig.getFloat("mass"), motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setUserPointer(entity);
	rigidBody->setAngularFactor(btVector3(0, 0, 1));//NOTE : prevent body from rotating in the x and y axis

    BombContainerComponent::InventoryType inventory;

    InventoryType *startingInventory = charConfig.getPointer<InventoryType *>("starting-inventory");

    for (auto kv : *startingInventory) {
        int cooldown = EntityConfigLookup::instance()[kv.first].getInt("cooldown");
        inventory[kv.first] = { kv.second, Timer(cooldown) };
    }

    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent(yaw, pitch));
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent(inventory));
    entity->attachComponent(new JetpackComponent());
	entity->attachComponent(new PlayerStatusComponent());
    entity->attachComponent(new HealthComponent(charConfig.getInt("health-start"), charConfig.getInt("health-cap")));
    entity->attachComponent(new EquipmentComponent(TIME_BOMB));

    return entity;
}
