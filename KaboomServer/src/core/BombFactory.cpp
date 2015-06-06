#include "BombFactory.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include <util/Configuration.h>
#include <util/Timer.h>
#include <util/XMLLoader.h>

#include "EntityConfigLookup.h"
#include "../components/CollisionComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TimerComponent.h"
#include "../components/TriggerComponent.h"
#include "../math/util.h"
#include "../messaging/DefaultExplosionMessageHandler.h"
#include "../messaging/FakeBombMessageHandler.h"
#include "../messaging/KaboomV2MessageHandler.h"
#include "../messaging/MessageHandlerChain.h"
#include "../messaging/RemoteDetonatorMessageHandler.h"
#include "../messaging/SaltyMartyBombMessageHandler.h"
#include "../messaging/TimeBombMessageHandler.h"

BombFactory::BombFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *BombFactory::createBomb(
        EntityType type,
        const Vec3 &position,
        const Vec3 &velocity) const {
    Entity *entity = entityManager.createEntity(type);

    createBase(entity, position, velocity);

    switch (type) {
        case KABOOM_V2: {
            createKaboomV2(entity);
            break;
        }
        case TIME_BOMB: {
            createTimeBomb(entity);
            break;
        }
        case REMOTE_DETONATOR: {
            createRemoteDetonator(entity);
            break;
        }
        case SALTY_MARTY_BOMB: {
            createSaltyMartyBomb(entity);
            break;
        }
        case FAKE_BOMB: {
            createFakeBomb(entity);
            break;
        }
    }

    return entity;
}

void BombFactory::createBase(
        Entity *entity,
        const Vec3 &position,
        const Vec3 &velocity) const {
    auto &config = EntityConfigLookup::get(entity->getType());

    float size = config.getFloat("size");
    float mass = config.getFloat("mass");
    float explosionRadius = config.getFloat("explosion-radius");

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(position.x, position.y, position.z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btSphereShape(size);

    btVector3 localInertia;
    collisionShape->calculateLocalInertia(mass, localInertia);

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, localInertia);
    rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    rigidBody->setAngularVelocity(btVector3(randDecimal(-2.0f, 2.0f), randDecimal(-2.0f, 2.0f), randDecimal(-2.0f, 2.0f)));
    rigidBody->setUserPointer(entity);

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(explosionRadius));
    ghostObject->setWorldTransform(worldTrans);
    ghostObject->setUserPointer(entity);

    static DefaultExplosionMessageHandler explosionHandler;

    MessageHandlerChain *chain = new MessageHandlerChain();
    chain->addHandler(&explosionHandler);

    entity->attachComponent(new PositionComponent(position));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new TriggerComponent(ghostObject));
    entity->attachComponent(new MessageHandlerComponent(chain));
}

void BombFactory::createKaboomV2(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static KaboomV2MessageHandler kaboomV2Handler;
    chain->addHandler(&kaboomV2Handler);

    entity->attachComponent(new CollisionComponent());
}

void BombFactory::createTimeBomb(Entity *entity) const {
    auto &config = EntityConfigLookup::get(entity->getType());
    auto physComp = entity->getComponent<PhysicsComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static TimeBombMessageHandler timeBombHandler;
    chain->addHandler(&timeBombHandler);

    int delay = config.getInt("delay");
    entity->attachComponent(new TimerComponent(new Timer(delay)));

    float restitution = config.getFloat("restitution");
	physComp->getRigidBody()->setRestitution(restitution);
}

void BombFactory::createRemoteDetonator(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    entity->attachComponent(new CollisionComponent());

    static RemoteDetonatorMessageHandler remoteDetonatorHandler;
    chain->addHandler(&remoteDetonatorHandler);
}

void BombFactory::createSaltyMartyBomb(Entity *entity) const {
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static SaltyMartyBombMessageHandler saltyMartyBombMessageHandler;
    chain->addHandler(&saltyMartyBombMessageHandler);

    entity->attachComponent(new CollisionComponent());
}

void BombFactory::createFakeBomb(Entity *entity) const {
    auto &config = EntityConfigLookup::get(entity->getType());
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    auto chain = static_cast<MessageHandlerChain *>(handlerComp->getHandler());

    static FakeBombMessageHandler fakeBombMessageHandler;
    chain->addHandler(&fakeBombMessageHandler);

    entity->attachComponent(new CollisionComponent());
    entity->attachComponent(new TimerComponent(new Timer(config.getInt("delay"))));
}
