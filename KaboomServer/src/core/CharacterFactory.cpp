#include "CharacterFactory.h"

#include <btBulletDynamicsCommon.h>

#include <components/BombContainerComponent.h>
#include <components/EquipmentComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/HealthComponent.h>
#include <core/EntityManager.h>
#include <util/Configuration.h>

#include "common.h"
#include "EntityConfigLookup.h"
#include "../components/CharacterRotationComponent.h"
#include "../components/InputComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/JetpackComponent.h"
#include "../components/JumpComponent.h"
#include "../messaging/DefaultCharacterMessageHandler.h"
#include "../messaging/MessageHandlerChain.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(
        EntityType characterType,
        const Vec3 &position,
        const Quat &rotation) const {
    auto &charConfig = EntityConfigLookup::get(characterType);

    Entity *entity = entityManager.createEntity(characterType);

    createBase(entity, position, rotation);

    switch (characterType) {
        case DEFAULT_CHARACTER: {
            createDefaultCharacter(entity);
            break;
        }
    }

    return entity;
}

void CharacterFactory::createBase(
        Entity *entity,
        const Vec3 &position,
        const Quat &rotation) const {
    auto &config = EntityConfigLookup::get(entity->getType());

    float collisionRadius = config.getFloat("collision-radius");
    float collisionHeight = config.getFloat("collision-height");
    float mass = config.getFloat("mass");
    InventoryType *startingInventory = config.getPointer<InventoryType *>("starting-inventory");
    int healthStart = config.getInt("health-start");
    int healthCap = config.getInt("health-cap");

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(position.x, position.y, position.z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
	btCollisionShape *collisionShape = new btCapsuleShapeZ(btScalar(collisionRadius), btScalar(collisionHeight));

    btRigidBody *rigidBody = new btRigidBody(btScalar(mass), motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setUserPointer(entity);
    rigidBody->setAngularFactor(btVector3(0, 0, 1)); // NOTE : prevent body from rotating in the x and y axis

    BombContainerComponent::InventoryType inventory;

    for (auto kv : *startingInventory) {
        int cooldown = EntityConfigLookup::get(kv.first).getInt("cooldown");
        inventory[kv.first] = { kv.second, Timer(cooldown) };
    }

    MessageHandlerChain *chain = new MessageHandlerChain();

    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent(rotation));
    entity->attachComponent(new CharacterRotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent(inventory));
    entity->attachComponent(new PlayerStatusComponent());
    entity->attachComponent(new HealthComponent(healthStart, healthCap));
    entity->attachComponent(new JumpComponent);
    entity->attachComponent(new MessageHandlerComponent(chain));
}

void CharacterFactory::createDefaultCharacter(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static DefaultCharacterMessageHandler defaultCharHandler;
    chain->addHandler(&defaultCharHandler);

    entity->attachComponent(new JetpackComponent());
    entity->attachComponent(new EquipmentComponent(KABOOM_V2));
}
