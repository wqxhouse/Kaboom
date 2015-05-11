#pragma once

#include "Message.h"
#include "MessageType.h"

class NoAttackMessage : public Message {
public:
    NoAttackMessage(Game *game, Entity *entity)
            : Message(MessageType::NO_ATTACK, game, entity) {
    }
};
