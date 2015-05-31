#pragma once

#include "Message.h"
#include "MessageType.h"

class TickMessage : public Message {
public:
    TickMessage(Game *game, Entity *entity)
            : Message(MessageType::TICK, game, entity) {
    }
};
