#include "ItemSystem.h"

#include <core/CharacteristicComponent.h>

#include "Game.h"
#include "TriggerComponent.h"

ItemSystem::ItemSystem(Game *game)
        : EntitySystem(game) {
}

void ItemSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        if (entity->getComponent<CharacteristicComponent>()->getType() != EntityType::ITEM) {
            continue;
        }

        TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

        auto triggerEntities = triggerComp->getTriggerEntities();

        for (Entity *triggerentity : triggerEntities) {
            if (triggerentity->getComponent<CharacteristicComponent>()->getType() != EntityType::PLAYER) {
                continue;
            }
            int a = 0;
            // TODO: Handle item pickup logic here.
        }
    }
}
