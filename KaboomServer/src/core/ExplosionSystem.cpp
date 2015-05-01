#include "ExplosionSystem.h"

#include <core/CharacteristicComponent.h>

#include "ExplosionComponent.h"
#include "Game.h"
#include "TriggerComponent.h"

ExplosionSystem::ExplosionSystem(Game *game)
        : EntitySystem(game) {
}

ExplosionSystem::~ExplosionSystem() {
}

void ExplosionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        if (!entity->hasComponent<ExplosionComponent>()) {
            continue;
        }

        btGhostObject *ghostObject = entity->getComponent<TriggerComponent>()->getGhostObject();

        const auto pairs = ghostObject->getOverlappingPairs();
        const int numPairs = ghostObject->getNumOverlappingObjects();

        for (int i = 0; i < numPairs; i++) {
            // Target is the entity within the range of explosion
            Entity *target = static_cast<Entity *>(pairs[i]->getUserPointer());

            if (target == nullptr) {
                continue;
            }

            // TODO: Handle damage

            CharacteristicComponent *charComp = target->getComponent<CharacteristicComponent>();

            if (charComp != nullptr && charComp->getType() == EntityType::PLAYER) {
                // TODO: Handle knockback
                continue;
            }
        }

        game->getGameServer().sendExplosionEvent(entity);
        game->removeEntity(entity);
    }
}
