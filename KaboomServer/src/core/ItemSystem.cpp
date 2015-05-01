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

        btGhostObject *ghostObject = triggerComp->getGhostObject();
        int numObjects = ghostObject->getNumOverlappingObjects();

        for (int i = 0; i < numObjects; ++i) {
            btCollisionObject *obj = ghostObject->getOverlappingObject(i);
            
            Entity *target = static_cast<Entity *>(obj->getUserPointer());

            if (target == nullptr) {
                continue;
            }

            if (target->getComponent<CharacteristicComponent>()->getType() != EntityType::PLAYER) {
                continue;
            }

            // TODO: Handle item pickup logic here.
        }
    }
}
