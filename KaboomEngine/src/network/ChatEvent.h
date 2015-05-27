#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class ChatEvent : public Event {
public:
    static const int MAX_MESSAGE_LENGTH = 256;

    ChatEvent(unsigned int playerId = 0, const std::string &message = "")
            : Event(EVENT_CHAT, sizeof(ChatEvent)),
              playerId(playerId) {
        strncpy_s(this->message, MAX_MESSAGE_LENGTH, message.c_str(), MAX_MESSAGE_LENGTH - 1);
        this->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(ChatEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(ChatEvent));
        this->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline void setPlayerId(unsigned int playerId) {
        this->playerId = playerId;
    }

    inline const char *getMessage() const {
        return message;
    }

    friend std::ostream& operator<<(std::ostream &os, const ChatEvent &o) {
        os << "ChatEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    message: " << o.message << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
    char message[MAX_MESSAGE_LENGTH];
};
