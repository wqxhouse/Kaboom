#include "PhysicsSystem.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>

#include "../components/CharacterRotationComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/DestroyComponent.h"
#include "../components/PhysicsComponent.h"
#include "../core/Game.h"

PhysicsSystem::PhysicsSystem(Game *game, World &world)
        : EntityProcessingSystem(game),
          world(world) {
}

void PhysicsSystem::preprocessEntities(std::vector<Entity *> entities) {
    world.stepSimulation(1.0f / game->FPS, game->MAX_SUB_STEPS);
}

bool PhysicsSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() &&
            entity->hasComponent<PhysicsComponent>();
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
