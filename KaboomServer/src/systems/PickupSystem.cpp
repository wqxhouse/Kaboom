#include "PickupSystem.h"

#include <components/WeaponPickupComponent.h>

#include "../components/TriggerComponent.h"
#include "../core/BombDataLookup.h"
#include "../core/Game.h"

PickupSystem::PickupSystem(Game *game)
        : EntitySystem(game) {
}

void PickupSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *pickup : entities) {
        WeaponPickupComponent* weaponPickupComp = pickup->getComponent<WeaponPickupComponent>(); // TODO: Make this generic

        if (weaponPickupComp == nullptr) {
            continue;
        }

        TriggerComponent *triggerComp = pickup->getComponent<TriggerComponent>();

        auto triggerEntities = triggerComp->getTriggerEntities();

        for (Entity *player : triggerEntities) {
            if ((player->getType() & CAT_CHARACTER) != CAT_CHARACTER) {
                continue;
            }

            // TODO: Handle item pickup logic here.
			// Do the Upper cap for max amount of bombs here

            BombContainerComponent *invComp = player->getComponent<BombContainerComponent>();

            const EntityType &bombType = weaponPickupComp->getBombType();

            const int capacity = BombDataLookup::instance[bombType].capacity;
            const int invAmount = invComp->getAmount(bombType);
            const int pickupAmount = weaponPickupComp->getAmount();

            int amount = capacity - invAmount;

            if (weaponPickupComp->getAmount() < amount) {
                amount = weaponPickupComp->getAmount();
            }

            invComp->addToInventory(bombType, amount);

            game->removeEntity(pickup);
            // TODO: Send remove pickup event to clients

			//TODO: to avoid multiple people picking up the item at the same time, we need to delete the itemEntity right after
			
        }
    }
}
