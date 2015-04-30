#include "PhysicsSystem.h"

#include "Game.h"

PhysicsSystem::PhysicsSystem(Game *game)
        : EntitySystem(game) {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);
    }
}
