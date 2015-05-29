#pragma once

#include <math/Vec3.h>

#include "Message.h"
#include "MessageType.h"

class CharacterSpawnMessage : public Message {
public:
    CharacterSpawnMessage(Game *game, Entity *entity, const Vec3 &position)
            : Message(MessageType::CHARACTER_SPAWN, game, entity) {
    }

    inline const Vec3 &getPosition() {
        return position;
    }

private:
    Vec3 position;
};
