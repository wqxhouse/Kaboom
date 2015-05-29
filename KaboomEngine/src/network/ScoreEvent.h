#pragma once

#include <ostream>

#include "Event.h"

class ScoreEvent : public Event {
public:
    ScoreEvent(unsigned int playerId = 0, int kills = 0, int deaths = 0)
            : Event(EVENT_SCORE, sizeof(ScoreEvent)),
              playerId(playerId),
              kills(kills),
              deaths(deaths) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(ScoreEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(ScoreEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline int getKills() const {
        return kills;
    }

    inline int getDeaths() const {
        return deaths;
    }

    friend std::ostream &operator<<(std::ostream &os, const ScoreEvent &o) {
        os << "ScoreEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    kills: " << o.kills << std::endl;
        os << "    deaths: " << o.deaths << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;

    int kills;
    int deaths;
};
