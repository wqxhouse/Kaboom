#pragma once

#include "GameMode.h"

#include "../util/Timer.h"

class DeathmatchMode : public GameMode {
public:
    DeathmatchMode(int preMatchDuration, int matchDuration, int postMatchDuration);

    inline MatchState getMatchState() const {
        return state;
    }

    void setMatchState(MatchState state);

    bool updateMatchState();

private:
    int preMatchDuration;
    int matchDuration;
    int postMatchDuration;

    Timer timer;
    MatchState state;
};
