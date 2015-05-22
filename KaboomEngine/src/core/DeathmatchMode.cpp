#include "DeathmatchMode.h"

DeathmatchMode::DeathmatchMode(int preMatchDuration, int matchDuration, int postMatchDuration)
        : GameMode(MatchState::ENTER_MAP, Timer(preMatchDuration)),
          preMatchDuration(preMatchDuration),
          matchDuration(matchDuration),
          postMatchDuration(postMatchDuration) {
}

void DeathmatchMode::setMatchState(MatchState state) {
    this->state = state;

    switch (state) {
        case MatchState::PRE_MATCH: {
            timer.setDuration(preMatchDuration);
            timer.start();
            break;
        }
        case MatchState::IN_PROGRESS: {
            timer.setDuration(matchDuration);
            timer.start();
            break;
        }
        case MatchState::POST_MATCH: {
            timer.setDuration(postMatchDuration);
            timer.start();
            break;
        }
    }
}

bool DeathmatchMode::updateMatchState() {
    switch (state) {
        case MatchState::ENTER_MAP: {
            setMatchState(MatchState::PRE_MATCH);
            break;
        }
        case MatchState::PRE_MATCH: {
            if (timer.isExpired()) {
                setMatchState(MatchState::IN_PROGRESS);
                return true;
            }
            break;
        }
        case MatchState::IN_PROGRESS: {
            if (timer.isExpired()) {
                setMatchState(MatchState::POST_MATCH);
                return true;
            }
            break;
        }
        case MatchState::POST_MATCH: {
            if (timer.isExpired()) {
                setMatchState(MatchState::LEAVE_MAP);
                return true;
            }
            break;
        }
        case MatchState::LEAVE_MAP: {
            setMatchState(MatchState::ENTER_MAP);
            break;
        }
    }

    return false;
}
