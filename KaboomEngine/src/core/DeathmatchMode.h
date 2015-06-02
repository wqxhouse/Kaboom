#pragma once

#include "GameMode.h"

class DeathmatchMode : public GameMode {
public:
    static const int DEFAULT_PRE_MATCH_DURATION = 15 * 1000;
    static const int DEFAULT_MATCH_DURATION = 5 * 60 * 1000;
    static const int DEFAULT_POST_MATCH_DURATION = 15 * 1000;

    DeathmatchMode(
            int preMatchDuration = DEFAULT_PRE_MATCH_DURATION,
            int matchDuration = DEFAULT_MATCH_DURATION,
            int postMatchDuration = DEFAULT_POST_MATCH_DURATION);

    virtual void setMatchState(MatchState state);
    virtual bool updateMatchState();

    inline int getPreMatchDuration() const {
        return preMatchDuration;
    }

    inline int getMatchDuration() const {
        return matchDuration;
    }

    inline int getPostMatchDuration() const {
        return postMatchDuration;
    }

    inline void setPreMatchDuration(int preMatchDuration) {
        this->preMatchDuration = preMatchDuration;
    }

    inline void setMatchDuration(int matchDuration) {
        this->matchDuration = matchDuration;
    }

    inline void setPostMatchDuration(int postMatchDuration) {
        this->postMatchDuration = postMatchDuration;
    }

private:
    int preMatchDuration;
    int matchDuration;
    int postMatchDuration;
};
