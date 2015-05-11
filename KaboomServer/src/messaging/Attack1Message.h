#pragma once

#include "Message.h"
#include "MessageType.h"

class Attack1Message : public Message {
public:
    Attack1Message(Game *game, Entity *entity)
            : Message(MessageType::ATTACK1, game, entity) {
    }
};
