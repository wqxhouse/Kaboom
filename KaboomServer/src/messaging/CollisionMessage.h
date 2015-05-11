#pragma once

#include <unordered_set>

#include "Message.h"
#include "MessageType.h"

class CollisionMessage : public Message {
public:
    CollisionMessage(
            Game *game,
            Entity *entity,
            std::unordered_set<Entity *> contactEntities)
            : Message(MessageType::COLLISION, game, entity),
              contactEntities(contactEntities) {
    }

    inline std::unordered_set<Entity *> getContactEntities() const {
        return contactEntities;
    }

private:
    std::unordered_set<Entity *> contactEntities;
};
