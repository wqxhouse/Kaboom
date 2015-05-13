#include "VoidSystem.h"

#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>

bool VoidSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PositionComponent>() &&
            entity->hasComponent<HealthComponent>();
}

void VoidSystem::processEntity(Entity *entity) {
    auto posComp = entity->getComponent<PositionComponent>();
    auto healthComp = entity->getComponent<HealthComponent>();

    if (posComp->getZ() < -100) {
        healthComp->setHealthAmount(0);
    }
}
