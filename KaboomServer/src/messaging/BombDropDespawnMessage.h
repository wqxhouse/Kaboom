#pragma once

#include "Message.h"
#include "MessageType.h"

class BombDropDespawnMessage : public Message {
public:
	BombDropDespawnMessage(
		Game *game,
		Entity *entity)
		: Message(MessageType::BOMB_DROP_DESPAWN, game, entity) {
	}
};
