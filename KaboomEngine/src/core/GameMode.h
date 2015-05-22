#pragma once

#include "../util/Timer.h"

class GameMode {
public:
    enum class MatchState {
        ENTER_MAP,
        PRE_MATCH,
        IN_PROGRESS,
        POST_MATCH,
        LEAVE_MAP
    };

    GameMode(MatchState state = MatchState::ENTER_MAP, Timer timer = Timer())
            : state(state),
              timer(timer) {
    }

    virtual ~GameMode() {}

    virtual void setMatchState(MatchState state) = 0;
    virtual bool updateMatchState() = 0;

    inline MatchState getMatchState() const {
        return state;
    }

    inline const Timer &getTimer() const {
        return timer;
    }

    inline void setTimer(const Timer &timer) {
        this->timer = timer;
    }

protected:
    MatchState state;
    Timer timer;
};
