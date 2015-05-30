#pragma once

#include <ostream>
#include <string>

#include "Event.h"
#include "../core/GameMode.h"
#include "../util/Timer.h"

class MatchStateEvent : public Event {
public:
    MatchStateEvent()
            : MatchStateEvent(
                    GameMode::MatchState::ENTER_MAP,
                    std::chrono::milliseconds(0),
                    std::chrono::milliseconds(0)) {
    }

    MatchStateEvent(
            GameMode::MatchState state,
            const std::chrono::milliseconds &duration,
            const std::chrono::milliseconds &remainingTime)
            : Event(EVENT_MATCH_STATE, sizeof(MatchStateEvent)),
              state(state),
              duration(duration),
              remainingTime(remainingTime) {
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

    inline const std::chrono::milliseconds &getDuration() const {
        return duration;
    }

    inline const std::chrono::milliseconds &getRemainingTime() const {
        return remainingTime;
    }

    friend std::ostream& operator<<(std::ostream &os, const MatchStateEvent &o) {
        os << "MatchStateEvent: {" << std::endl;
        os << "    state: " << static_cast<int>(o.state) << std::endl;
        os << "    duration: " << o.duration.count() << std::endl;
        os << "    remainingTime: " << o.remainingTime.count() << std::endl;
        os << "}";

        return os;
    }

private:
    GameMode::MatchState state;
    std::chrono::milliseconds duration;
    std::chrono::milliseconds remainingTime;
};
