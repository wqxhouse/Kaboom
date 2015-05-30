#include "BombPickupMessageHandler.h"

#include <limits>

#include <components/InventoryComponent.h>
#include <components/PositionComponent.h>
#include <components/WeaponPickupComponent.h>
#include <components/HealthComponent.h>
#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "PickupMessage.h"
#include "../components/DestroyComponent.h"
#include "../components/PickupRespawnComponent.h"
#include "../components/PlayerDeathComponent.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

bool BombPickupMessageHandler::handle(const Message &message) const {
    if (message.getType() == MessageType::PICKUP) {
        return handle(static_cast<const PickupMessage &>(message));
    }

    return false;
}

bool BombPickupMessageHandler::handle(const PickupMessage &message) const {
    Entity *pickup = message.getEntity();
    auto posComp = pickup->getComponent<PositionComponent>();

    Entity *closestEntity = nullptr;
    float minDistance = FLT_MAX;

    auto &nearbyEntities = message.getNearbyEntities();

    // Find closest character
    for (Entity *character : nearbyEntities) {
        auto charPosComp = character->getComponent<PositionComponent>();

		if (charPosComp == nullptr || !character->hasComponent<InventoryComponent>() || character->hasComponent<PlayerDeathComponent>()) {
            continue;
        }

        const float distance = getDistance(posComp->getPosition(), charPosComp->getPosition());

        if (distance < minDistance) {
            closestEntity = character;
            minDistance = distance;
        }
    }

    // Add bomb to character's inventory and remove pickup from world
    if (closestEntity != nullptr) {
        auto weaponPickupComp = pickup->getComponent<WeaponPickupComponent>();
        auto invComp = closestEntity->getComponent<InventoryComponent>();

        EntityType bombType = weaponPickupComp->getType();

		//if pick up is an health pack we want to add to health instead
		if (bombType == HEALTH_PACK) { 
			const int healAmount = weaponPickupComp->getAmount();
			auto charHealthComp = closestEntity->getComponent<HealthComponent>();
			charHealthComp->addAmount(healAmount);

		} else {
			// TODO: Detect and handle maximum number of bombs a character can hold
			const int capacity = EntityConfigLookup::get(bombType).getInt("capacity");
			const int invAmount = invComp->getAmount(bombType);
			const int pickupAmount = weaponPickupComp->getAmount();

			int amount = capacity - invAmount;

			if (weaponPickupComp->getAmount() < amount) {
				amount = weaponPickupComp->getAmount();
			}

			invComp->add(bombType, amount);
		}

        Game *game = message.getGame();

        //if the pickup has a spawn component, meaning it is a pickup that respawn over time, 
        //add it to the pickupSpawnTimer maps, for requesting a respawn, later in the future
		if (pickup->hasComponent<PickupRespawnComponent>()) {
			PickupRespawnComponent* respawnComp = pickup->getComponent<PickupRespawnComponent>();
			game->getPickupRequest().insert(std::make_pair(respawnComp->getSpawnPointName(), Timer(respawnComp->getDuration())));
        }

        pickup->attachComponent(new DestroyComponent());
    }

    return true;
}
