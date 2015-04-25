#pragma once

#include <ostream>

#include "Event.h"

class DisconnectEvent : public Event {
public:
    DisconnectEvent(unsigned int playerId);
    ~DisconnectEvent();
    DisconnectEvent();

    const unsigned int &getPlayerId() const;
    void setPlayerId(const unsigned int &playerId);

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

    friend std::ostream& operator<<(std::ostream &os, const DisconnectEvent &o) {
        os << "DisconnectEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
};
