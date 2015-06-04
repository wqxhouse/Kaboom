#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"

class PlayerSelectionEvent : public Event {
public:
    PlayerSelectionEvent(unsigned int playerId = 0, EntityType characterType = NONE)
            : Event(EVENT_PLAYER_SELECTION, sizeof(PlayerSelectionEvent)),
              playerId(playerId),
              characterType(characterType) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerSelectionEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerSelectionEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline void setPlayerId(unsigned int playerId) {
        this->playerId = playerId;
    }

    inline EntityType getCharacterType() const {
        return characterType;
    }

    inline void setCharacterType(EntityType characterType) {
        this->characterType = characterType;
    }

    friend std::ostream& operator<<(std::ostream &os, const PlayerSelectionEvent &o) {
        os << "PlayerSelectionEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    characterType: " << o.characterType << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
    EntityType characterType;
};
