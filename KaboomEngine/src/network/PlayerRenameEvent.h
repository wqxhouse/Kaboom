#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class PlayerRenameEvent : public Event {
public:
    static const int MAX_NAME_LENGTH = 32;

    PlayerRenameEvent(unsigned int playerId = 0, const std::string &name = "")
            : Event(EVENT_PLAYER_RENAME, sizeof(PlayerRenameEvent)),
              playerId(playerId) {
        strncpy_s(this->name, MAX_NAME_LENGTH, name.c_str(), MAX_NAME_LENGTH - 1);
        this->name[MAX_NAME_LENGTH - 1] = '\0';
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerRenameEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerRenameEvent));
        this->name[MAX_NAME_LENGTH - 1] = '\0';
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline void setPlayerId(unsigned int playerId) {
        this->playerId = playerId;
    }

    inline const char *getName() const {
        return name;
    }

    friend std::ostream& operator<<(std::ostream &os, const PlayerRenameEvent &o) {
        os << "PlayerRenameEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    name: " << o.name << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
    char name[MAX_NAME_LENGTH];
};
