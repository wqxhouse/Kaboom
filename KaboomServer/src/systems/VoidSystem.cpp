#include "VoidSystem.h"

#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>

#include "../components/DestroyComponent.h"

VoidSystem::VoidSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool VoidSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PositionComponent>();
}

void VoidSystem::processEntity(Entity *entity) {
    auto posComp = entity->getComponent<PositionComponent>();
    auto healthComp = entity->getComponent<HealthComponent>();

    if (posComp->getPosition().z < -100) { // TODO: Extract constant to XML
        if (healthComp != nullptr) {
            healthComp->setAmount(0);
        } else {
            entity->attachComponent(new DestroyComponent());
        }
    }
}
