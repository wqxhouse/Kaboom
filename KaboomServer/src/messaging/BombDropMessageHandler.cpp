#include "BombDropMessageHandler.h"

#include <limits>

#include <components/InventoryComponent.h>
#include <components/PositionComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "BombDropMessage.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/DestroyComponent.h"
#include "../components/TimerComponent.h"
#include "../messaging/MessageHandlerChain.h"
#include "../messaging/BombDropDespawnMessageHandler.h"
#include "../core/EntityConfigLookup.h"
#include "../core/Game.h"
#include "../math/util.h"

bool BombDropMessageHandler::handle(const Message &message) const {
    if (message.getType() == MessageType::BOMB_DROP) {
		return handle(static_cast<const BombDropMessage &>(message));
    }

    return false;
}

bool BombDropMessageHandler::handle(const BombDropMessage &message) const {
    Entity *player = message.getEntity();
	Game* game = message.getGame();
	auto charPosComp = player->getComponent<PositionComponent>();
	auto charInvComp = player->getComponent<InventoryComponent>();

	//spawn a weapon pickup base on the bomb still in the player's inventory
	//select the bomb that's op or the player have most

	
	auto weaponAmountPair = charInvComp->getWeaponDropFromInventory();

	if (weaponAmountPair.second > 0) { //make sure we only spawn the weapon if the player actually have bombs left

		Entity *entity = game->getPickupFactory().createPickup(
			weaponAmountPair.first,
			charPosComp->getPosition(),
			weaponAmountPair.second,
			0.5); //this is hard coded for now as we don't have a corresponding radius for the pickup we are dropping. (although 0.5f radius seems good enough)

		//add a Timer Component to the pickup, so that it will disappear after a certain time
		entity->attachComponent(new TimerComponent(new Timer(1000 * 30)));//hard coded to 30 secs for now
		auto messageHandlerComp = entity->getComponent<MessageHandlerComponent>();
		auto chain = static_cast<MessageHandlerChain *>(messageHandlerComp->getHandler());
		static BombDropDespawnMessageHandler bombPickupDespawnMessageHandler;
		chain->addHandler(&bombPickupDespawnMessageHandler);

		game->addEntity(entity);
	}

    return true;
}