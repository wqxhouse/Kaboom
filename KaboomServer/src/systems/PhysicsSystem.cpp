#include "PhysicsSystem.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>

#include "../core/Game.h"

PhysicsSystem::PhysicsSystem(Game *game)
        : game(game) {
}

void PhysicsSystem::preprocessEntities(std::vector<Entity *> entities) {
    const clock_t FPS = 60;
    const float TIME_STEP = 1.0f / FPS;
    const int MAX_SUB_STEPS = 1;

    game->stepSimulation(TIME_STEP, MAX_SUB_STEPS); // TODO: Extract constant
}

bool PhysicsSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PhysicsComponent>();
}

void PhysicsSystem::processEntity(Entity *entity) {
    PhysicsComponent *physComp = entity->getComponent<PhysicsComponent>();

    const btTransform &worldTrans = physComp->getRigidBody()->getWorldTransform();

    PositionComponent *posComp = entity->getComponent<PositionComponent>();

    if (posComp != nullptr) {
        const btVector3 &pos = worldTrans.getOrigin();
        posComp->setPosition(pos.getX(), pos.getY(), pos.getZ());
    }

    RotationComponent *rotComp = entity->getComponent<RotationComponent>();

    if (rotComp != nullptr) {
        btQuaternion rot = worldTrans.getRotation();
        // TODO: Set rotComp
    }
}
