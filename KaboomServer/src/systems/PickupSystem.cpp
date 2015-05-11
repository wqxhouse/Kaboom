#include "PickupSystem.h"

#include <components/WeaponPickupComponent.h>

#include "../components/TriggerComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"

PickupSystem::PickupSystem(Game *game)
        : game(game) {
}

bool PickupSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<WeaponPickupComponent>() &&
            entity->hasComponent<TriggerComponent>();
}

void PickupSystem::processEntity(Entity *entity) {
    WeaponPickupComponent *weaponPickupComp = entity->getComponent<WeaponPickupComponent>();
    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    auto triggerEntities = triggerComp->getTriggerEntities();

    for (Entity *player : triggerEntities) {
        if ((player->getType() & CAT_CHARACTER) != CAT_CHARACTER) {
            continue;
        }

        // TODO: Handle item pickup logic here.
        // Do the Upper cap for max amount of bombs here

        BombContainerComponent *invComp = player->getComponent<BombContainerComponent>();

        const EntityType &bombType = weaponPickupComp->getBombType();

        const int capacity = EntityConfigLookup::instance()[bombType].getInt("capacity");
        const int invAmount = invComp->getAmount(bombType);
        const int pickupAmount = weaponPickupComp->getAmount();

        int amount = capacity - invAmount;

        if (weaponPickupComp->getAmount() < amount) {
            amount = weaponPickupComp->getAmount();
        }

        invComp->addToInventory(bombType, amount);

        game->getGameServer().sendDestroyEvent(entity);
        game->removeEntity(entity);
    }
}
