#pragma once

#include "Message.h"
#include "MessageType.h"

class Attack2Message : public Message {
public:
    Attack2Message(Game *game, Entity *entity)
            : Message(MessageType::ATTACK2, game, entity) {
    }
};
