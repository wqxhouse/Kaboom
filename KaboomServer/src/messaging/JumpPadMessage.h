#pragma once

#include <unordered_set>

#include "Message.h"
#include "MessageType.h"

class JumpPadMessage : public Message {
public:
	JumpPadMessage(
            Game *game,
            Entity *entity,
            const std::unordered_set<Entity *> &nearbyEntities)
            : Message(MessageType::JUMP_PAD, game, entity),
              nearbyEntities(nearbyEntities) {
    }

    inline const std::unordered_set<Entity *> &getNearbyEntities() const {
        return nearbyEntities;
    }

private:
    const std::unordered_set<Entity *> &nearbyEntities;
};
