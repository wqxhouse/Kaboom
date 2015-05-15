#pragma once

#include "TimerHandler.h"

class TimeBombTimerHandler : public TimerHandler {
public:
    virtual void handle(Game *game, Entity *entity);
};
