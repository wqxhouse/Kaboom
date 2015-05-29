#pragma once

#include "Message.h"
#include "MessageType.h"

class BombDropMessage : public Message {
public:
	BombDropMessage(
            Game *game,
            Entity *entity)
            : Message(MessageType::BOMB_DROP, game, entity) {
    }
};
