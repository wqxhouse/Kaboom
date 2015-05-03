#include "CharacterFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/BombContainerComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <core/PlayerStatusComponent.h>

#include "BombDataLookup.h"
#include "CharacterDataLookup.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "JetpackComponent.h"

CharacterFactory::CharacterFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *CharacterFactory::createCharacter(float x, float y, float z) const {
    Entity *entity = entityManager.createEntity(DEFAULT_CHARACTER);
    const CharacterData &characterData = CharacterDataLookup::instance[DEFAULT_CHARACTER];

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    btRigidBody *rigidBody = new btRigidBody(characterData.mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setUserPointer(entity);

    BombContainerComponent::InventoryType inventory = BombContainerComponent::InventoryType();

    for (auto kv : characterData.inventory) {
        const BombData &bombData = BombDataLookup::instance[kv.first];
        inventory[kv.first] = { kv.second, Timer(bombData.cooldown) };
    }

    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent(inventory));
    entity->attachComponent(new JetpackComponent());
	entity->attachComponent(new PlayerStatusComponent());

    return entity;
}
