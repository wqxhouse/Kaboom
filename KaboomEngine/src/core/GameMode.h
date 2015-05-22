#pragma once

class GameMode {
public:
    enum class MatchState {
        ENTER_MAP,
        PRE_MATCH,
        IN_PROGRESS,
        POST_MATCH,
        LEAVE_MAP
    };

    virtual ~GameMode() {}
};
