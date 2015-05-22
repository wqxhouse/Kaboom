#pragma once

#include <ostream>
#include <string>

#include "Event.h"
#include "../core/GameMode.h"
#include "../util/Timer.h"

class MatchStateEvent : public Event {
public:
    MatchStateEvent(GameMode::MatchState state = GameMode::MatchState::ENTER_MAP, Timer timer = Timer())
            : Event(EVENT_MATCH_STATE, sizeof(MatchStateEvent)),
              state(state),
              timer(timer) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(MatchStateEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(MatchStateEvent));
    }

    inline GameMode::MatchState getState() const {
        return state;
    }

    inline const Timer &getTimer() const {
        return timer;
    }

    friend std::ostream& operator<<(std::ostream &os, const MatchStateEvent &o) {
        os << "MatchStateEvent: {" << std::endl;
        os << "    state: " << static_cast<int>(o.state) << std::endl;
        os << "    timer: {" << std::endl;
        os << "        duration: " << o.getTimer().getDuration() << std::endl;
        os << "        startTime: " << o.getTimer().getStartTime() << std::endl;
        os << "    }" << std::endl;
        os << "}";

        return os;
    }

private:
    GameMode::MatchState state;
    Timer timer;
};
