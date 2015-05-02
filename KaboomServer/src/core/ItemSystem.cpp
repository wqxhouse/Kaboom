#include "ItemSystem.h"

#include <core/WeaponPickupComponent.h>
#include "Game.h"
#include "TriggerComponent.h"


ItemSystem::ItemSystem(Game *game)
        : EntitySystem(game) {
}

void ItemSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *itemEntity : entities) {
        if ((itemEntity->getType() & CAT_PICKUP) != CAT_PICKUP) {
            continue;
        }

		TriggerComponent *triggerComp = itemEntity->getComponent<TriggerComponent>();

        auto triggerEntities = triggerComp->getTriggerEntities();

        for (Entity *triggerentity : triggerEntities) {
            if ((triggerentity->getType() & CAT_PLAYER) != CAT_PLAYER) {
                continue;
            }

            // TODO: Handle item pickup logic here.
			// Do the Upper cap for max amount of bombs here
			WeaponPickupComponent* weaponPickupComponent = itemEntity->getComponent<WeaponPickupComponent>();
			if (weaponPickupComponent != nullptr) {
				triggerentity->getComponent<BombContainerComponent>()->addBombToInv(weaponPickupComponent->getBombType(), weaponPickupComponent->getAmount());
			}

			//TODO: to avoid multiple people picking up the item at the same time, we need to delete the itemEntity right after
			
        }
    }
}
