#pragma once

#include <unordered_set>

#include "Message.h"
#include "MessageType.h"

class PickupMessage : public Message {
public:
    PickupMessage(
            Game *game,
            Entity *entity,
            const std::unordered_set<Entity *> &nearbyEntities)
            : Message(MessageType::PICKUP, game, entity),
              nearbyEntities(nearbyEntities) {
    }

    inline const std::unordered_set<Entity *> &getNearbyEntities() const {
        return nearbyEntities;
    }

private:
    const std::unordered_set<Entity *> &nearbyEntities;
};
