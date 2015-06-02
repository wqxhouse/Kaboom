#include "BombDropDespawnMessageHandler.h"

#include <core/Entity.h>

#include "Message.h"
#include "MessageType.h"
#include "BombDropDespawnMessage.h"
#include "../components/DestroyComponent.h"
#include "../components/TimerComponent.h"
#include "../core/Game.h"
#include "../math/util.h"

bool BombDropDespawnMessageHandler::handle(const Message &message) const {
	if (message.getType() == MessageType::BOMB_DROP_DESPAWN) {
		return handle(static_cast<const BombDropDespawnMessage &>(message));
	}

	return false;
}

bool BombDropDespawnMessageHandler::handle(const BombDropDespawnMessage &message) const {
	Entity *bombDrop = message.getEntity();
	Game* game = message.getGame();

	//Mark the bombdrop as ready to be destroy after staying in the world for a time period.
	bombDrop->attachComponent(new DestroyComponent());
	return true;
}