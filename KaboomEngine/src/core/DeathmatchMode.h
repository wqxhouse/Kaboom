#pragma once

#include "GameMode.h"

class DeathmatchMode : public GameMode {
public:
    DeathmatchMode(int preMatchDuration, int matchDuration, int postMatchDuration);

    virtual void setMatchState(MatchState state);
    virtual bool updateMatchState();

private:
    int preMatchDuration;
    int matchDuration;
    int postMatchDuration;
};
