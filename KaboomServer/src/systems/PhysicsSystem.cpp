#include "PhysicsSystem.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>

#include "../components/CharacterRotationComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../core/Game.h"

PhysicsSystem::PhysicsSystem(Game *game, World &world)
        : game(game),
          world(world) {
}

void PhysicsSystem::preprocessEntities(std::vector<Entity *> entities) {
    const clock_t FPS = 60;
    const float TIME_STEP = 1.0f / FPS;
    const int MAX_SUB_STEPS = 1;

    world.stepSimulation(TIME_STEP, MAX_SUB_STEPS); // TODO: Extract constant
}

bool PhysicsSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PhysicsComponent>();
}

void PhysicsSystem::processEntity(Entity *entity) {
    auto physComp = entity->getComponent<PhysicsComponent>();
    auto posComp = entity->getComponent<PositionComponent>();
    auto colComp = entity->getComponent<CollisionComponent>();

    btTransform worldTrans = physComp->getRigidBody()->getWorldTransform();

    if (posComp != nullptr) {
        const btVector3 &pos = worldTrans.getOrigin();
        posComp->setPosition(Vec3(pos.getX(), pos.getY(), pos.getZ()));
    }

    RotationComponent *rotComp = entity->getComponent<RotationComponent>();

    if (rotComp != nullptr && !entity->hasComponent<CharacterRotationComponent>()) {
        btQuaternion rot = worldTrans.getRotation();
        rotComp->setRotation(Quat(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));
    }
}
