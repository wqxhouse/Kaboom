#include "BombPickupMessageHandler.h"

#include <limits>

#include <components/BombContainerComponent.h>
#include <components/PositionComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "PickupMessage.h"
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

    const float pickupPosX = posComp->getX();
    const float pickupPosY = posComp->getY();
    const float pickupPosZ = posComp->getZ();

    Entity *closestEntity = nullptr;
    float minDistance = FLT_MAX;

    auto &nearbyEntities = message.getNearbyEntities();

    // Find closest character
    for (Entity *character : nearbyEntities) {
        auto charPosComp = character->getComponent<PositionComponent>();

        if (charPosComp == nullptr || !character->hasComponent<BombContainerComponent>()) {
            continue;
        }

        const float charPosX = charPosComp->getX();
        const float charPosY = charPosComp->getY();
        const float charPosZ = charPosComp->getZ();

        const float distance = getDistance(pickupPosX, pickupPosY, pickupPosZ, charPosX, charPosY, charPosZ);

        if (distance < minDistance) {
            closestEntity = character;
            minDistance = distance;
        }
    }

    // Add bomb to character's inventory and remove pickup from world
    if (closestEntity != nullptr) {
        auto weaponPickupComp = pickup->getComponent<WeaponPickupComponent>();
        auto invComp = closestEntity->getComponent<BombContainerComponent>();

        EntityType bombType = weaponPickupComp->getType();

        // TODO: Detect and handle maximum number of bombs a character can hold

        const int capacity = EntityConfigLookup::instance()[bombType].getInt("capacity");
        const int invAmount = invComp->getAmount(bombType);
        const int pickupAmount = weaponPickupComp->getAmount();

        int amount = capacity - invAmount;

        if (weaponPickupComp->getAmount() < amount) {
            amount = weaponPickupComp->getAmount();
        }

        invComp->addToInventory(bombType, amount);

        Game *game = message.getGame();
        game->getGameServer().sendDestroyEvent(pickup);
        game->removeEntity(pickup);
    }

    return true;
}
